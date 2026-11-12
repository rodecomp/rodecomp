#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Contents/Items/ItemNaming.h"

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
extern template bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
extern template std::vector<int>::vector(const std::vector<int>&);

using ItemCardNameTree = std::_Tree<std::_Tmap_traits<int, ItemCardName, std::less<int>, std::allocator<std::pair<const int, ItemCardName>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, ItemCardName>, void*>*> ItemCardNameTree::_Find_lower_bound<int>(const int&) const;
