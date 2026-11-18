#pragma once

#include "Resource/ResourceMgr.h"
#include "File/File.h"
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>

using AliasTree = std::map<std::string, std::string>::_Mybase;
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char* const, const std::size_t, const char* const, const std::size_t) noexcept;
template __declspec(noinline) std::_Tree_find_result<AliasTree::_Nodeptr> AliasTree::_Find_lower_bound<std::string>(const std::string&) const;
