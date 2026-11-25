#pragma once

#include "Contents/Items/ItemInfo.h"

#include <cstddef>
#include <list>
#include <string>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __forceinline void std::_List_node<ITEM_INFO, void*>::_Free_non_head<std::allocator<std::_List_node<ITEM_INFO, void*>>>(
  std::allocator<std::_List_node<ITEM_INFO, void*>>&, std::_List_node<ITEM_INFO, void*>*) noexcept;
