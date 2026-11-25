#pragma once

#include <vector>

extern template std::vector<int>::~vector();
template __declspec(noinline) std::vector<int>::vector(const std::vector<int>&);
