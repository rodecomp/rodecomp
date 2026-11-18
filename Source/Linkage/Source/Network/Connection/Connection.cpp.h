#pragma once

#include <cstdint>
#include <map>
#include "Client/Application/ErrorDialog.h"
#include "Client/Session/Session.h"
#include "Network/Connection/Connection.h"
#include "Network/Connection/ConnectionRuntime.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/PacketId.h"
#include "Render/DeviceContext/DCSurface.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <timeapi.h>
#include <memory>
#include <string>

using InstantValueTree = std::_Tree<std::_Tmap_traits<
  int, std::int64_t, std::less<int>, std::allocator<std::pair<const int, std::int64_t>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, std::int64_t>, void*>*>
InstantValueTree::_Find_lower_bound<int>(const int&) const;
template __declspec(noinline) std::_Tree_find_result<std::_Tree_node<std::pair<const int, std::int64_t>, void*>*>
InstantValueTree::_Find_lower_bound<int>(const int&) const;
