#pragma once

#include <map>
#include <xtree>

using IntIntTree = std::_Tree<std::_Tmap_traits<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, int>, void*>*>
IntIntTree::_Find_lower_bound<int>(const int&) const;
