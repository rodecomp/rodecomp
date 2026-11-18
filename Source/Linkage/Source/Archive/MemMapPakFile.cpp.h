#pragma once

#include <vector>
#include "Archive/MemMapPakFile.h"

extern template void std::vector<unsigned char>::resize(size_type);
template __declspec(noinline) std::vector<std::uint8_t>::~vector() noexcept;
