#pragma once

#include "Contents/Monster/ChampionMonsterMgr.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Script/NLuaState.h"
extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <new>
#include <string>
#include <utility>
#include <windows.h>

extern template std::vector<int>::~vector();

template __declspec(noinline) std::string std::operator+(std::string&&, const char* const);
