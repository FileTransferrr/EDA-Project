#ifndef NCSC_H
#define NCSC_H

#include "vpi_user.h"
#include "cast_value.h"

#include <functional>

#pragma GCC diagnostic ignored "-Wunused-variable"

void dsc_get_param(const char* param_name, int& p);
void dsc_get_param(const char* param_name, double& p);
void dsc_get_param(const char* param_name, std::string& p);

inline void ncsc_get_param(const char* param_name, int& p) { dsc_get_param(param_name, p); }
inline void ncsc_get_param(const char* param_name, double& p) { dsc_get_param(param_name, p); }
inline void ncsc_get_param(const char* param_name, std::string& p)  { dsc_get_param(param_name, p); }

typedef ::sc_core::sc_module*(*ScModuleCreator)(const char*);

namespace dsccom
{
    bool associate_module(const char* name, ScModuleCreator);
}

#define NCSC_MODULE_EXPORT(module_name)\
bool dummy_##module_name = dsccom::associate_module(#module_name ,\
     [](const char* name)->::sc_core::sc_module* { return new module_name(name); });

namespace dsccom
{
    template<class T>
    void writeSysFuncRes(vpiHandle, const T& value);

    const char* registerFunction(std::string&& function_name, std::function<bool(vpiHandle)> func);
}

template<class T, class R, class ...Args, size_t... I>
void funcCaller(
    const std::vector<vpiHandle>& call_args,
    R (T::*func)(Args... args),
    T* obj,
    std::index_sequence<I...>)
{
    assert(call_args.size() >= (sizeof...(Args) + 1));
    dsccom::writeSysFuncRes(call_args[0], (obj->*func)(readVpiToSc<Args>(call_args[I+1])...));
}

template<class T, class ...Args, size_t... I>
void funcCaller(
    const std::vector<vpiHandle>& call_args,
    void (T::*func)(Args... args),
    T* obj,
    std::index_sequence<I...>)
{
    assert(call_args.size() >= sizeof...(Args));
    (obj->*func)(readVpiToSc<Args>(call_args[I])...);
}

template<class T, class R, class... Args>
bool binderFunc(
    const char* name,
    R (T::*func)(Args... args),
    T* obj,
    vpiHandle call_args_iterator)
{
    constexpr size_t expected_args_num = sizeof...(Args)
                                        + (std::is_same<R, void>::value ? 0 : 1);
    std::vector<vpiHandle> call_args;
    call_args.reserve(expected_args_num);
    vpiHandle call_arg;
    while ((call_arg = vpi_scan( call_args_iterator )) != nullptr)
    {
        call_args.push_back(call_arg);
    }

    if (call_args.size() < expected_args_num )
    {
        std::cerr << "Call ignored. Exported function \"" << name << "\" expected " << expected_args_num << " arguments" << std::endl;
        return false;
    }

    funcCaller(call_args, func, obj, std::make_index_sequence<sizeof...(Args)>{});

    return true;
}

template<class T, class R, class... Args>
const char* ncsc_register_function(
  const char* name,
  R (T::*func)(Args...),
  sc_object* name_scope,
  T* obj
)
{
    std::string function_name = name_scope->name();
    function_name += SC_HIERARCHY_CHAR;
    function_name += name;

    return dsccom::registerFunction(std::move(function_name),
        std::bind(binderFunc<T,R,Args...>, name, func, obj, std::placeholders::_1));
}

#define NCSC_REGISTER_MEMBER(model, func) \
ncsc_register_function(#func, &model::func, this, this);

namespace dsccom
{
    ::sc_core::sc_module* getScScope();
}

extern "C" void registerScAlias(const char* name, void* execute);

template<class R>
inline R wrongReturn()
{
    return R{};
}

template<>
inline void wrongReturn() {}

template<class FuncType, FuncType func, class T, class R, class... Args>
R ncsc_caller(Args... args)
{
    ::sc_core::sc_module* scope = dsccom::getScScope();
    T* obj = dynamic_cast<T*>(scope);
    if (obj == nullptr)
        return wrongReturn<R>();

    return (obj->*func)(args...);
};

template<class FuncType, FuncType func, class T, class R, class... Args>
auto createFunc(
    R (T::*)(Args...)
    )
{
    return &ncsc_caller<FuncType, func, T, R, Args...>;
}

template<class FuncType, FuncType func>
int register_dpi_function(
  const char* name)
{
    registerScAlias(name, reinterpret_cast<void*>(createFunc<FuncType, func>(func)));

    return 0;
}

#define NCSC_REGISTER_DPI_MEMBER_ALIAS(alias,model,func)\
static int register_dpi_function_##alias = \
    register_dpi_function<decltype(&model::func), &model::func>(#alias);

#endif // NCSC_H