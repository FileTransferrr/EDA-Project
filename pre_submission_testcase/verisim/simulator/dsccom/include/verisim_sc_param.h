#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>

void dsc_get_param(const char* param_name, int& p);
void dsc_get_param(const char* param_name, double& p);
void dsc_get_param(const char* param_name, std::string& p);

inline void ncsc_get_param(const char* param_name, int& p) { dsc_get_param(param_name, p); }
inline void ncsc_get_param(const char* param_name, double& p) { dsc_get_param(param_name, p); }
inline void ncsc_get_param(const char* param_name, std::string& p)  { dsc_get_param(param_name, p); }

#endif // PARAMETERS_H