#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Contents/Items/ItemNaming.h"
#include "UI/Window/UIEnums.h"
#include "Contents/Items/ItemInfo.h"
#include "Client/Mode/ModeDragState.h"
#include "Contents/Items/ItemId.h"
#include "Client/Session/Session.h"
#include "Client/Input/InputCharset.h"
#include "File/File.h"
#include "Contents/Items/ItemDescription.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Shop/UIMerchantMirrorItemWnd.h"
#include <cstdlib>
#include <cstring>
#include <list>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
extern template bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
extern template std::vector<int>::vector(const std::vector<int>&);

using ItemCardNameTree = std::_Tree<std::_Tmap_traits<int, ItemCardName, std::less<int>, std::allocator<std::pair<const int, ItemCardName>>, false>>;
template __declspec(noinline) std::_Tree_find_result<std::_Tree_node<std::pair<const int, ItemCardName>, void*>*> ItemCardNameTree::_Find_lower_bound<int>(const int&) const;
template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) void std::list<int>::push_back(const int&);
template __declspec(noinline) std::vector<int>::~vector() noexcept;
template std::list<ITEM_INFO>::iterator std::list<ITEM_INFO>::erase(std::list<ITEM_INFO>::const_iterator) noexcept;
