#pragma once

#include <string>

extern template std::string std::operator+(std::string&&, const char* const);
extern template std::string& std::string::operator=(std::string&&) noexcept;
