#pragma once

#include "Contents/Items/ItemInfo.h"
#include <string>
#include <vector>

extern template std::vector<std::string>::~vector();
extern template void std::_Destroy_range<std::allocator<ITEM_INFO>>(ITEM_INFO*, ITEM_INFO* const, std::allocator<ITEM_INFO>&) noexcept;
