#include "port_bind.h"
#include "cast_value.h"
#include "sysc/datatypes/fx/sc_fix.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
namespace
{

template<class T>
int valueLength(const T& value)
{
    return value.length();
}

#define VALUE_LENGTH_METHOD(T)\
template<>\
int valueLength(const T& value)\
{\
    return sizeof(value)*8;\
}

VALUE_LENGTH_METHOD(char)
VALUE_LENGTH_METHOD(unsigned char)
VALUE_LENGTH_METHOD(short)
VALUE_LENGTH_METHOD(unsigned short)
VALUE_LENGTH_METHOD(int)
VALUE_LENGTH_METHOD(unsigned int)
VALUE_LENGTH_METHOD(long)
VALUE_LENGTH_METHOD(unsigned long)
VALUE_LENGTH_METHOD(long long)
VALUE_LENGTH_METHOD(unsigned long long)

template<class T>
T lvToType(const sc_lv_base& src)
{
    return T(src);
}

#define LV_TO_NATIVE_UNSIGNED_METHOD(T)\
template<>\
T lvToType<T>(const sc_lv_base& src)\
{\
    return static_cast<T>(src.to_uint64());\
}

#define LV_TO_NATIVE_SIGNED_METHOD(T)\
template<>\
T lvToType<T>(const sc_lv_base& src)\
{\
    return static_cast<T>(src.to_int64());\
}

LV_TO_NATIVE_SIGNED_METHOD(char)
LV_TO_NATIVE_UNSIGNED_METHOD(unsigned char)
LV_TO_NATIVE_SIGNED_METHOD(short)
LV_TO_NATIVE_UNSIGNED_METHOD(unsigned short)
LV_TO_NATIVE_SIGNED_METHOD(int)
LV_TO_NATIVE_UNSIGNED_METHOD(unsigned int)
LV_TO_NATIVE_SIGNED_METHOD(long)
LV_TO_NATIVE_UNSIGNED_METHOD(unsigned long)
LV_TO_NATIVE_SIGNED_METHOD(long long)
LV_TO_NATIVE_UNSIGNED_METHOD(unsigned long long)

template<class T>
void setSignal(void* signal_ptr, sc_logic* data)
{
    auto* signal = static_cast<sc_signal<T>*>(signal_ptr);
    sc_lv_base caster(valueLength(signal->read()));
    caster = data;
    T new_value = lvToType<T>(caster);
    signal->write(new_value);
}

template<>
void setSignal<double>(void* signal_ptr, sc_logic* data)
{
    auto* signal = static_cast<sc_signal<double>*>(signal_ptr);
    double new_value(data[0].value());
    signal->write(new_value);
}

template<>
void setSignal<sc_logic>(void* signal_ptr, sc_logic* data)
{
    auto* signal = static_cast<sc_signal<sc_logic>*>(signal_ptr);
    signal->write(data[0]);
}

template<>
void setSignal<bool>(void* signal_ptr, sc_logic* data)
{
    auto* signal = static_cast<sc_signal<bool>*>(signal_ptr);
    signal->write(data[0].to_bool());
}

template<>
void setSignal<sc_bit>(void* signal_ptr, sc_logic* data)
{
    auto* signal = static_cast<sc_signal<sc_bit>*>(signal_ptr);
    sc_bit new_value(data[0]);
    signal->write(new_value);
}

template<>
void setSignal<const char*>(void* signal_ptr, sc_logic* data)
{
    auto* signal = static_cast<sc_signal<const char*>*>(signal_ptr);
    sc_lv_base caster(1);
    caster = data;
    std::string str = caster.to_string();
    signal->write(&str[0]);
}

template<>
void setSignal<char*>(void* signal_ptr, sc_logic* data)
{
    auto* signal = static_cast<sc_signal<char*>*>(signal_ptr);
    sc_lv_base caster(1);
    caster = data;
    std::string str = caster.to_string();
    signal->write(&str[0]);
}

template<class T>
void setSignal(void* signal_ptr, int data)
{
    auto* signal = static_cast<sc_signal<T>*>(signal_ptr);
    T new_value(data);
    signal->write(new_value);
}

template<class T>
void setSignal(void* signal_ptr, double data)
{
    setSignal<T>(signal_ptr, static_cast<int>(lround(data)));
}

template<>
void setSignal<double>(void* signal_ptr, double data)
{
    auto* signal = static_cast<sc_signal<double>*>(signal_ptr);
    double new_value(data);
    signal->write(new_value);
}

template<>
void setSignal<float>(void* signal_ptr, double data)
{
    auto* signal = static_cast<sc_signal<float>*>(signal_ptr);
    float new_value(data);
    signal->write(new_value);
}

template<class T>
void setSignal(void* signal_ptr, const char* data)
{
    auto* signal = static_cast<sc_signal<T>*>(signal_ptr);
    sc_lv_base caster(data);
    T new_value = lvToType<T>(caster);
    signal->write(new_value);
}

template<class D>
void setStrSignal(void* signal_ptr, D data)
{
    sc_dt::sc_fix caster(data);
    std::string str_data = caster.to_string();
    setSignal<const char*>(signal_ptr, str_data.c_str());
}

void setStrSignal(void* signal_ptr, const char* data)
{
    auto* signal = static_cast<sc_signal<const char*>*>(signal_ptr);
    signal->write(data);
}

template<>
void setSignal<const char*>(void* signal_ptr, const char* data)
{
    setStrSignal(signal_ptr, data);
}

template<>
void setSignal<const char*>(void* signal_ptr, int data)
{
    setStrSignal(signal_ptr, data);
}

template<>
void setSignal<const char*>(void* signal_ptr, double data)
{
    setStrSignal(signal_ptr, data);
}

template<>
void setSignal<char*>(void* signal_ptr, const char* data)
{
    setStrSignal(signal_ptr, data);
}

template<>
void setSignal<char*>(void* signal_ptr, int data)
{
    setStrSignal(signal_ptr, data);
}

template<>
void setSignal<char*>(void* signal_ptr, double data)
{
    setStrSignal(signal_ptr, data);
}

template<>
void setSignal<sc_logic>(void* signal_ptr, const char* data)
{
    auto* signal = static_cast<sc_signal<sc_logic>*>(signal_ptr);
    sc_logic new_value(data[0]);
    signal->write(new_value);
}

template<>
void setSignal<bool>(void* signal_ptr, const char* data)
{
    auto* signal = static_cast<sc_signal<bool>*>(signal_ptr);
    sc_bit new_value(data[0]);
    signal->write(new_value.to_bool());
}

template<>
void setSignal<sc_bit>(void* signal_ptr, const char* data)
{
    auto* signal = static_cast<sc_signal<sc_bit>*>(signal_ptr);
    sc_bit new_value(data[0]);
    signal->write(new_value);
}


template<class T>
void destroySignal(void* signal_ptr)
{
    auto* signal = static_cast<sc_signal<T>*>(signal_ptr);
    delete signal;
}


template<class T>
bool readSignal(void* signal_ptr, t_vpi_value& new_value, sc_dt::uint64 first_stamp, sc_dt::uint64 last_stamp)
{
    auto* signal = static_cast<sc_signal<T>*>(signal_ptr);
    sc_dt::uint64 signal_stamp = signal->change_stamp();
    if (signal_stamp < first_stamp || signal_stamp > last_stamp)
        return false;

    const T& value = signal->read();
    new_value = scValueToVpi(value);

    return true;
}

template<class T>
void keepScPortInfo(sc_signal<T>* signal)
{
    if (current_ports_keeper == nullptr || current_vpi_port == nullptr)
        return;

    dsccom::SignalSetter4s setter = setSignal<T>;
    dsccom::SignalSetterInt setter_int = setSignal<T>;
    dsccom::SignalSetterReal setter_real = setSignal<T>;
    dsccom::SignalSetterStr setter_str = setSignal<T>;
    dsccom::DestroySignal destructor = destroySignal<T>;
    dsccom::SignalReader reader = readSignal<T>;
    current_ports_keeper->emplace_back(current_vpi_port, signal, setter, setter_int, setter_real, setter_str, reader, destructor);
}

template<class T>
void connectSignalToPort(sc_inout_base<T>& port)
{
    auto* sig = new sc_signal<T>();
    port(*sig);
    keepScPortInfo(sig);
}

template<class T>
void connectSignalToPort(sc_in_base<T>& port)
{
    auto* sig = new sc_signal<T>();
    port(*sig);
    keepScPortInfo(sig);
}

}
#pragma GCC diagnostic pop

namespace sc_core
{
template<class T>
void sc_report_signal_type(std::ostream& out, T& port)
{
    connectSignalToPort(port);
}
}
