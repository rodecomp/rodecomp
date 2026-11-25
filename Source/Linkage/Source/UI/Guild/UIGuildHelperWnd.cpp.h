#pragma once

#include <cstddef>
#include <string>
#include <vector>

extern template std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);

template std::basic_string<char>& std::basic_string<char>::operator=(std::basic_string<char>&&) noexcept;

template __declspec(noinline) std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);

template __declspec(noinline) std::vector<std::string>::~vector() noexcept;
template __forceinline std::vector<std::vector<std::string>>::~vector() noexcept;
template __forceinline void std::vector<std::vector<std::string>>::_Tidy() noexcept;
template __forceinline void std::vector<std::vector<std::string>>::_Destroy(std::vector<std::string>*, std::vector<std::string>*);
template __forceinline void std::_Destroy_range<std::allocator<std::vector<std::string>>>(
  std::vector<std::string>*, std::vector<std::string>* const, std::allocator<std::vector<std::string>>&) noexcept;
