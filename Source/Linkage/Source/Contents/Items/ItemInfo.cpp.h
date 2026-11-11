#pragma once

#include <cstddef>
#include <string>
#include <vector>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
extern template bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
extern template std::vector<int>::vector(const std::vector<int>&);
