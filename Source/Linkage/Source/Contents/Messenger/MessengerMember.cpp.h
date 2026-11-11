#pragma once

#include <cstddef>
#include <string>
#include <vector>

extern template std::string::basic_string(const std::string&);
extern template std::vector<unsigned char>::vector(std::size_t, const unsigned char&, const std::allocator<unsigned char>&);
