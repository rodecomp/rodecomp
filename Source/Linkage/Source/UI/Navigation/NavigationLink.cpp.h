#pragma once

#include <cstddef>
#include <string>
#include <vector>

extern template std::vector<std::string>::~vector();
extern template std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
