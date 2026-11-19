#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

extern template std::string& std::string::assign(const char*, std::string::size_type);
extern template std::vector<std::string>::~vector();
template __declspec(noinline) std::_Tree_find_result<std::map<int, std::int64_t>::_Mybase::_Nodeptr> std::map<int, std::int64_t>::_Mybase::_Find_lower_bound<int>(const int&) const;
