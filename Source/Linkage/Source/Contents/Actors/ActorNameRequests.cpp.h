#pragma once

#include "Contents/Actors/ActorNameCard.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include <algorithm>
#include <cstdint>
#include <windows.h>
#include <timeapi.h>

using ActorNameCardTree = std::_Tree<std::_Tmap_traits<
  std::uint32_t, CActorNameCard, std::less<std::uint32_t>,
  std::allocator<std::pair<const std::uint32_t, CActorNameCard>>, false>>;
template __declspec(noinline) std::_Tree_find_result<std::_Tree_node<std::pair<const std::uint32_t, CActorNameCard>, void*>*>
ActorNameCardTree::_Find_lower_bound<std::uint32_t>(const std::uint32_t&) const;
template __declspec(noinline) void std::deque<std::uint32_t>::pop_front() noexcept;
template __declspec(noinline) ActorNameCardTree::iterator ActorNameCardTree::find(const std::uint32_t&);
