#pragma once

#include <string>
#include <vector>

template __declspec(noinline) std::string::basic_string(const std::string&);
extern template void std::vector<char>::_Tidy() noexcept;
template __declspec(noinline) std::string& std::string::append(std::size_t, char);
