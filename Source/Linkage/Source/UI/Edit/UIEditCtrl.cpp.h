#pragma once

#include <string>
#include <vector>

extern template std::string std::operator+(const char* const, const std::string&);
extern template std::string std::operator+(const std::string&, const std::string&);
extern template void std::vector<unsigned char>::_Tidy() noexcept;
