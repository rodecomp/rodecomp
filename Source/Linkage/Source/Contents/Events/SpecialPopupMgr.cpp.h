#pragma once

#include "Contents/Events/SpecialPopupMgr.h"
#include "Script/LuaFileOperations.h"
#include "Script/NLuaState.h"
extern "C" {
#include <lua.h>
#include <lualib.h>
}
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <new>
#include <windows.h>

template __declspec(noinline) std::string std::operator+(std::string&&, const char* const);
