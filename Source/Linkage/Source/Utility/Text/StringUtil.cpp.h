#pragma once

#include <string>

template __declspec(noinline) std::string::basic_string(const std::string&);
template __declspec(noinline) std::string::size_type std::string::find_last_of(const char* const, const std::string::size_type) const noexcept;
template __declspec(noinline) std::string std::string::substr(const std::string::size_type, const std::string::size_type) const;
