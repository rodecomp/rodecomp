#pragma once

#include <cstddef>
#include <string>

extern template std::string::basic_string(const std::string&);
extern template std::size_t std::_Traits_find<std::char_traits<char>>(const char* const, const std::size_t, const std::size_t, const char* const, const std::size_t) noexcept;
