#pragma once

#include "Script/LuaScript.h"

#include <cstddef>
#include <map>
#include <string>
#include <vector>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
using KafraServiceMapTree = std::_Tree<std::_Tmap_traits<
  std::string, std::vector<std::string>, std::less<std::string>, std::allocator<std::pair<const std::string, std::vector<std::string>>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const std::string, std::vector<std::string>>, void*>*>
KafraServiceMapTree::_Find_lower_bound<std::string>(const std::string&) const;
using MiniMapPositionTree = std::_Tree<std::_Tmap_traits<
  std::string, MiniMapPosition, std::less<std::string>, std::allocator<std::pair<const std::string, MiniMapPosition>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const std::string, MiniMapPosition>, void*>*>
MiniMapPositionTree::_Find_lower_bound<std::string>(const std::string&) const;
extern template MiniMapPosition& std::map<std::string, MiniMapPosition>::operator[](const std::string&);
