#pragma once

#include <cstddef>
#include <set>
#include <string>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

using IntSetTree = std::_Tree<std::_Tset_traits<int, std::less<int>, std::allocator<int>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<int, void*>*> IntSetTree::_Find_lower_bound<int>(const int&) const;
