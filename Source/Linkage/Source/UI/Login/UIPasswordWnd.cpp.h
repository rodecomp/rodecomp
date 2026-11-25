#pragma once

#include <cstddef>
#include <string>

template __declspec(noinline) std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
template std::string& std::string::operator=(std::string&&) noexcept;
