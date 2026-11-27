#pragma once

#include <cstddef>
#include <regex>
#include <string>
#include <vector>

template __declspec(noinline) std::string::size_type
std::string::find(const std::string&, std::string::size_type) const noexcept;
extern template std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
template __forceinline std::regex::~basic_regex() noexcept;
template __forceinline std::smatch::const_reference std::smatch::operator[](std::smatch::size_type) const noexcept;
