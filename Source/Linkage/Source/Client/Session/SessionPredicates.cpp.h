#pragma once

#include <cstdint>
#include <map>
#include <xtree>

using SummonHpTree = std::_Tree<std::_Tmap_traits<
  std::int64_t, std::int64_t, std::less<std::int64_t>, std::allocator<std::pair<const std::int64_t, std::int64_t>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const std::int64_t, std::int64_t>, void*>*>
SummonHpTree::_Find_lower_bound<std::int64_t>(const std::int64_t&) const;
