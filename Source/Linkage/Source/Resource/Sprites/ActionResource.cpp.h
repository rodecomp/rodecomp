#pragma once

#include <string>
#include <vector>

template __declspec(noinline) void std::vector<std::string>::resize(const std::vector<std::string>::size_type);
template __forceinline std::string std::operator+(std::string&&, std::string&&);
