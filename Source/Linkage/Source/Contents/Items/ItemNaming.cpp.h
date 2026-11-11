#pragma once

#include <cstddef>
#include <string>

extern template void std::string::reserve(std::size_t);
extern template std::string& std::string::operator+=(const char*);
