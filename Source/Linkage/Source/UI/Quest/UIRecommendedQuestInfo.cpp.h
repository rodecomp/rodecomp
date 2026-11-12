#pragma once

#include "Contents/Quest/RenewQuestMgr.h"
#include <cstddef>
#include <map>
#include <string>

extern template std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
using RenewQuestRecommendedTree = std::_Tree<std::_Tmap_traits<
  unsigned int, RenewQuestRecommendedInfo, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, RenewQuestRecommendedInfo>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const unsigned int, RenewQuestRecommendedInfo>, void*>*>
RenewQuestRecommendedTree::_Find_lower_bound<unsigned int>(const unsigned int&) const;
