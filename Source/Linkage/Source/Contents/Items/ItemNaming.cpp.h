#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>

extern template void std::string::reserve(std::size_t);
extern template std::string& std::string::operator+=(const char*);

using BookItemNameTree = std::_Tree<std::_Tmap_traits<int, std::vector<const char*>, std::less<int>, std::allocator<std::pair<const int, std::vector<const char*>>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, std::vector<const char*>>, void*>*>
BookItemNameTree::_Find_lower_bound<int>(const int&) const;
