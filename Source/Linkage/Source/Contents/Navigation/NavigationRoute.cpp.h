#pragma once

#include <string>

extern template std::string& std::string::operator=(std::string&&) noexcept;
