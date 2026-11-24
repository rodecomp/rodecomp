#pragma once

#include "Contents/Items/BarterMarketItemMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <list>
#include <map>
#include <new>
#include <string>
#include <utility>
#include <vector>

template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) std::multimap<int, ITEM_INFO>::iterator std::multimap<int, ITEM_INFO>::_Mybase::erase(std::multimap<int, ITEM_INFO>::const_iterator) noexcept;
template __forceinline std::_Tree_node<std::pair<const int, ITEM_INFO>, void*>* std::multimap<int, ITEM_INFO>::_Mybase::_Erase_unchecked(
  std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const int, ITEM_INFO>>>, std::_Iterator_base0>
) noexcept;
