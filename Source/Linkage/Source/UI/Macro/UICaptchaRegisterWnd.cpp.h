#pragma once

#include <string>

template __declspec(noinline) std::string& std::string::operator=(std::string&&) noexcept;
