#pragma once

#include <string>

extern template std::string::basic_string(const std::string&);
extern template std::string& std::string::replace(std::string::size_type, std::string::size_type, const char*, std::string::size_type);
