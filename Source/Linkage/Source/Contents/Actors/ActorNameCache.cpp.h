#pragma once

#include "Contents/Actors/ActorNameCard.h"
#include <cstring>
#include <cstddef>

using ActorNameCardTree = std::_Tree<std::_Tmap_traits<
  std::uint32_t, CActorNameCard, std::less<std::uint32_t>,
  std::allocator<std::pair<const std::uint32_t, CActorNameCard>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const std::uint32_t, CActorNameCard>, void*>*>
ActorNameCardTree::_Find_lower_bound<std::uint32_t>(const std::uint32_t&) const;
template __declspec(noinline) std::_Tree_find_result<std::_Tree_node<std::pair<const std::uint32_t, CActorNameCard>, void*>*>
ActorNameCardTree::_Find_lower_bound<std::uint32_t>(const std::uint32_t&) const;
template __declspec(noinline) std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const std::uint32_t, CActorNameCard>>>, std::_Iterator_base0>&
std::_Tree_unchecked_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const std::uint32_t, CActorNameCard>>>, std::_Iterator_base0>::operator++() noexcept;
