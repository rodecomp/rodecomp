#pragma once

#include <string>
#include <vector>

extern template std::vector<std::string>::~vector();

extern template std::string std::operator+(std::string&&, const char* const);
