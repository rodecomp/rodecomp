#pragma once

#include <cstddef>
#include <string>

template __declspec(noinline) std::size_t std::_Traits_find<std::char_traits<char>>(const char* const, const std::size_t, const std::size_t, const char* const, const std::size_t) noexcept;
template __forceinline std::string& std::string::operator=(std::string&&) noexcept;
