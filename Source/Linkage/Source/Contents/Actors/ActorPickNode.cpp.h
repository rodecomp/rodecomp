#pragma once

#include <cstddef>
#include <string>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
extern template std::string::basic_string(const std::string&);
