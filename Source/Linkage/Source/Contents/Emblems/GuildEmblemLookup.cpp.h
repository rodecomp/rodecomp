#pragma once

#include <string>

#include "Contents/Guild/GuildMgr.h"

extern template std::string::basic_string(const std::string&);

using GuildTree = std::_Tree<std::_Tmap_traits<unsigned int, CGuild, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, CGuild>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const unsigned int, CGuild>, void*>*> GuildTree::_Find_lower_bound<unsigned int>(const unsigned int&) const;
