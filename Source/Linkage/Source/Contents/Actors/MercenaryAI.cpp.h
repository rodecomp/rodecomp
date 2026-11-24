#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

extern template bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

using InstantValueTree = std::_Tree<std::_Tmap_traits<
  int, std::int64_t, std::less<int>, std::allocator<std::pair<const int, std::int64_t>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, std::int64_t>, void*>*>
InstantValueTree::_Find_lower_bound<int>(const int&) const;
