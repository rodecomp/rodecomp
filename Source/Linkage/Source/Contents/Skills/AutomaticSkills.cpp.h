#pragma once

#include <cstdint>
#include <map>

using InstantValueTree = std::_Tree<std::_Tmap_traits<
  int, std::int64_t, std::less<int>, std::allocator<std::pair<const int, std::int64_t>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, std::int64_t>, void*>*>
InstantValueTree::_Find_lower_bound<int>(const int&) const;
