#ifndef CAST_VALUE_H
#define CAST_VALUE_H

#include "systemc.h"
#include "vpi_user.h"

t_vpi_value scValueToVpi(const bool& val);
t_vpi_value scValueToVpi(const sc_dt::sc_bit& val);
t_vpi_value scValueToVpi(const sc_dt::sc_logic& val);
t_vpi_value scValueToVpi(const int8_t& val);
t_vpi_value scValueToVpi(const uint8_t& val);
t_vpi_value scValueToVpi(const int16_t& val);
t_vpi_value scValueToVpi(const uint16_t& val);
t_vpi_value scValueToVpi(const int32_t& val);
t_vpi_value scValueToVpi(const uint32_t& val);
t_vpi_value scValueToVpi(const int64_t& val);
t_vpi_value scValueToVpi(const uint64_t& val);
t_vpi_value scValueToVpi(const long long& val);
t_vpi_value scValueToVpi(const unsigned long long& val);
t_vpi_value scValueToVpi(const float& val);
t_vpi_value scValueToVpi(const double& val);
t_vpi_value scValueToVpi(const sc_dt::sc_int_base& val);
t_vpi_value scValueToVpi(const sc_dt::sc_uint_base& val);
t_vpi_value scValueToVpi(const sc_dt::sc_signed& val);
t_vpi_value scValueToVpi(const sc_dt::sc_unsigned& val);
t_vpi_value scValueToVpi(const sc_dt::sc_lv_base& val);
t_vpi_value scValueToVpi(const sc_dt::sc_bv_base& val);
t_vpi_value scValueToVpi(const char* val);


template<typename T>
T readVpiToSc(vpiHandle arg);

template<> bool readVpiToSc(vpiHandle arg);
template<> sc_dt::sc_bit readVpiToSc(vpiHandle arg);
template<> sc_dt::sc_logic readVpiToSc(vpiHandle arg);
template<> int8_t readVpiToSc(vpiHandle arg);
template<> uint8_t readVpiToSc(vpiHandle arg);
template<> int16_t readVpiToSc(vpiHandle arg);
template<> uint16_t readVpiToSc(vpiHandle arg);
template<> int32_t readVpiToSc(vpiHandle arg);
template<> uint32_t readVpiToSc(vpiHandle arg);
template<> int64_t readVpiToSc(vpiHandle arg);
template<> uint64_t readVpiToSc(vpiHandle arg);
template<> long long readVpiToSc(vpiHandle arg);
template<> unsigned long long readVpiToSc(vpiHandle arg);
template<> float readVpiToSc(vpiHandle arg);
template<> double readVpiToSc(vpiHandle arg);
template<> char* readVpiToSc(vpiHandle arg);

#endif // CAST_VALUE_H
