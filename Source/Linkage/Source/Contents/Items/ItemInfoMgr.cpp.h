#pragma once

#include "Contents/Items/ItemInfoMgr.h"
#include "Contents/Items/ItemDescription.h"
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

template __declspec(noinline) std::string std::operator+(std::string&&, const char* const);
