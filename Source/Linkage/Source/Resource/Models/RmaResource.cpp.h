#pragma once

#include <string>
#include <vector>

extern template std::vector<int>::~vector();

template __declspec(noinline) void std::vector<std::string>::resize(std::size_t);
template __declspec(noinline) void std::vector<int>::resize(std::size_t);
