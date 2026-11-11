#pragma once

#include <cstdint>
#include <set>
#include <xtree>

using PendingActorNameTree = std::_Tree<std::_Tset_traits<
  std::uint32_t, std::less<std::uint32_t>, std::allocator<std::uint32_t>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::uint32_t, void*>*>
PendingActorNameTree::_Find_lower_bound<std::uint32_t>(const std::uint32_t&) const;
