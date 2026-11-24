#pragma once

#include <cstddef>
#include <string>
#include "Contents/Map/TownInfoMgr.h"
#include "Script/NLuaState.h"
extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}
#include <windows.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <new>
#include <utility>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) std::string std::operator+(std::string&&, const char* const);
template __forceinline std::_Tree<std::_Tmap_traits<std::string, std::vector<TownInfo>, std::less<std::string>, std::allocator<std::pair<const std::string, std::vector<TownInfo>>>, false>>::iterator
std::_Tree<std::_Tmap_traits<std::string, std::vector<TownInfo>, std::less<std::string>, std::allocator<std::pair<const std::string, std::vector<TownInfo>>>, false>>::find(const std::string&);
