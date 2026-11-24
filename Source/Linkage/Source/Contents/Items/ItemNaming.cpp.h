#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include "Client/Replay/Replay.h"
#include "Contents/Items/ItemType.h"
#include "Contents/CombatTypes.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include <cstdlib>
#include <cstring>
#include "Resource/MSGSTRINGID.h"

extern template void std::string::reserve(std::size_t);
extern template std::string& std::string::operator+=(const char*);

using BookItemNameTree = std::_Tree<std::_Tmap_traits<int, std::vector<const char*>, std::less<int>, std::allocator<std::pair<const int, std::vector<const char*>>>, false>>;
template __declspec(noinline) std::_Tree_find_result<std::_Tree_node<std::pair<const int, std::vector<const char*>>, void*>*>
BookItemNameTree::_Find_lower_bound<int>(const int&) const;
template __declspec(noinline) std::_Tree_find_result<std::map<int, ItemCardName>::_Mybase::_Nodeptr> std::map<int, ItemCardName>::_Mybase::_Find_lower_bound<int>(const int&) const;
template __declspec(noinline) void std::vector<unsigned char>::_Tidy() noexcept;
