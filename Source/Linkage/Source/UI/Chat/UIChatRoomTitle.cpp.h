#pragma once

#include <string>

extern template std::string std::string::substr(std::string::size_type, std::string::size_type) const;
extern template std::string& std::string::operator=(std::string&&) noexcept;
