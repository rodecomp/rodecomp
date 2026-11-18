#pragma once

#include <string>

extern template std::string& std::string::operator=(const std::string&);
extern template std::string& std::string::operator+=(const char*);
