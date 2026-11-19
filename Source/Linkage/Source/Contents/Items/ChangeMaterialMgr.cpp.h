#pragma once

#include <string>
#include "Contents/Items/ChangeMaterialMgr.h"
#include "Script/NLuaState.h"
extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <new>
#include <windows.h>

extern template std::string& std::string::assign(const char*, std::string::size_type);
template __declspec(noinline) std::string std::operator+(std::string&&, const char* const);
extern template std::vector<int>::~vector();
