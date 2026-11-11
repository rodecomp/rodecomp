#pragma once

#include <string>
#include <vector>

extern template std::string& std::string::assign(const char*, std::string::size_type);
extern template std::vector<std::string>::~vector();
