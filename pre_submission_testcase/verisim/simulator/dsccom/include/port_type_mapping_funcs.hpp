/* ***************************************************************************
   ***************************************************************************
   *********  Generate type cast procedures                          *********
   *********  by cosim_default_ports.map file                        *********
   ***************************************************************************
   *************************************************************************** */

using namespace sc_dt;
namespace
{

template<class T>
void internal_func(std::ostream& out, int size);

#define STATIC_VECTOR_CAST(verilog, sc)\
template<>\
void internal_func<sc<0>>(std::ostream& out, int size)\
{\
    out << #verilog << " [" << size - 1 << ":0]";\
}

#define SIMPLE_VECTOR_CAST(verilog, sc)\
void internal_func(std::ostream& out, sc const & val)\
{\
    out << #verilog << " [" << sizeof(sc)*8 - 1 << ":0]";\
}

#define VECTOR_CAST(verilog, sc)\
void internal_func(std::ostream& out, sc const & val)\
{\
    out << #verilog << " [" << val.length() - 1 << ":0]";\
}

#define SIMPLE_CAST(verilog, sc)\
void internal_func(std::ostream& out, sc const & val)\
{\
    out << #verilog;\
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#include "cosim_default_ports.map"
#pragma GCC diagnostic pop
#undef SIMPLE_CAST
#undef SIMPLE_VECTOR_CAST
#undef STATIC_VECTOR_CAST
#undef VECTOR_CAST

template<class T>
void do_report_signal_type(std::ostream& out, const T& val)
{
    internal_func(out, val);
}

template<template<int> typename T, int W>
void do_report_signal_type(std::ostream& out, const T<W>& val)
{
    internal_func<T<0>>(out, W);
}
}

namespace sc_core
{
template<class T>
void sc_report_signal_type(std::ostream& out, sc_inout_base<T>& port)
{
    T val;
    do_report_signal_type(out, val);
}
template<class T>
void sc_report_signal_type(std::ostream& out, sc_in_base<T>& port)
{
    T val;
    do_report_signal_type(out, val);
}
}
