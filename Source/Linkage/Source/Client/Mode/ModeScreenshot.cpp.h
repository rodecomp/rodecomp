#pragma once

#include <string>

template __forceinline std::string std::operator+(const std::string&, std::string&&);
template __forceinline std::string& std::string::insert(const std::size_t, const std::string&);
template __declspec(noinline) std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char* const, const std::size_t, const char* const, const std::size_t);
