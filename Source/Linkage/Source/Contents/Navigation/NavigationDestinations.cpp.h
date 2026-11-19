#pragma once

#include <string>
#include <vector>

extern template std::string::basic_string(const std::string&);
extern template void std::vector<char>::_Tidy() noexcept;
extern template std::string& std::string::append(std::size_t, char);
