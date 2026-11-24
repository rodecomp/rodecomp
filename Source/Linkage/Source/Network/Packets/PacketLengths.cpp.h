#pragma once

#include "Network/Packets/PacketLengths.h"
#include <map>
#include <memory>

using PacketLengthNodeAllocator = std::allocator<std::_Tree_node<std::pair<const int, PacketLengthValue>, void*>>;

template __forceinline std::map<int, PacketLengthValue>::~map() noexcept;
template __forceinline std::_Tree<std::_Tmap_traits<int, PacketLengthValue, std::less<int>, std::allocator<std::pair<const int, PacketLengthValue>>, false>>::~_Tree() noexcept;
template __forceinline void std::_Tree_val<std::_Tree_simple_types<std::pair<const int, PacketLengthValue>>>::_Erase_head<PacketLengthNodeAllocator>(PacketLengthNodeAllocator&) noexcept;
template __forceinline void std::_Tree_val<std::_Tree_simple_types<std::pair<const int, PacketLengthValue>>>::_Erase_tree<PacketLengthNodeAllocator>(PacketLengthNodeAllocator&, std::_Tree_node<std::pair<const int, PacketLengthValue>, void*>*) noexcept;
