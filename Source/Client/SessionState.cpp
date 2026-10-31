#include "Client/Session.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/World/World.h"
#include "Script/LuaScript.h"

extern "C" {
void lua_settop(lua_State* L, int index);
int lua_type(lua_State* L, int index);
double lua_tonumber(lua_State* L, int index);
void lua_pushnil(lua_State* L);
void lua_pushnumber(lua_State* L, double value);
void lua_pushstring(lua_State* L, const char* text);
void lua_gettable(lua_State* L, int index);
void lua_getfield(lua_State* L, int index, const char* key);
int lua_next(lua_State* L, int index);
}

constexpr int LUA_GLOBALSINDEX = -10002;
constexpr int LUA_TTABLE = 5;

#include <cstddef>
#include <map>
#include <vector>

struct SkillDelayField {
  int type;
  const char* name;
};

// GLOBAL: gl_25057
static const SkillDelayField g_skillDelayFields[] = {
  {0, "SkillCastFixedDelay"},
  {1, "SkillCastStatDelay"},
  {2, "SkillGlobalPostDelay"},
  {3, "SkillSinglePostDelay"},
};

// OFFSET: 0x00E31650
// FUNCTION: fn_24173
void CSession::LoadSkillDelayList(int skillId, std::vector<std::map<int, int>>& delays, int& skillFlag) {
  delays.clear();
  int maxLevel = 0;
  skillFlag = 0;
  this->m_hotkeyScript->Call("GetSkillMaxLv", "d>d", skillId, &maxLevel);
  delays.resize(maxLevel);

  lua_State* L = this->m_hotkeyScript->GetState();
  lua_getfield(L, LUA_GLOBALSINDEX, "SKILL_DELAY_LIST");
  if (lua_type(L, -1) == LUA_TTABLE) {
    lua_pushnumber(L, skillId);
    lua_gettable(L, -2);
    if (lua_type(L, -1) == LUA_TTABLE) {
      lua_pushstring(L, "SkillFlag");
      lua_gettable(L, -2);
      if (lua_type(L, -1) == LUA_TTABLE) {
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
          skillFlag = static_cast<int>(skillFlag + lua_tonumber(L, -1));
          lua_settop(L, -2);
        }
      }
      lua_settop(L, -2);

      for (const SkillDelayField& field : g_skillDelayFields) {
        lua_pushstring(L, field.name);
        lua_gettable(L, -2);
        if (lua_type(L, -1) == LUA_TTABLE) {
          lua_pushnil(L);
          while (lua_next(L, -2) != 0) {
            const int level = static_cast<int>(lua_tonumber(L, -2) - 1.0);
            if (level >= 0 && static_cast<std::size_t>(level) < delays.size()) {
              const int delay = static_cast<int>(lua_tonumber(L, -1));
              delays[level][field.type] = delay;
            }
            lua_settop(L, -2);
          }
        }
        lua_settop(L, -2);
      }
    }
    lua_settop(L, -2);
  }
  lua_settop(L, -2);

  if (skillId == 2461) {
    if (delays.empty()) {
      delays.resize(1);
    }

    CGameActor* actor = static_cast<CGameActor*>(
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(this->unknown_5BC0)
    );
    int delay = 0;
    if (actor) {
      switch (actor->GetEffectiveJob(-1)) {
      case 2114:
      case 2117:
      case 2120:
      case 2123:
        delay = 3000;
        break;
      case 2115:
      case 2118:
      case 2121:
      case 2124:
        delay = 6000;
        break;
      case 2116:
      case 2119:
      case 2122:
      case 2125:
      case 20816:
      case 20817:
      case 20818:
      case 20819:
      case 20820:
        delay = 9000;
        break;
      }
    }
    delays[0][3] = delay;
  }
}

// OFFSET: 0x00E3D3D0
// FUNCTION: fn_24190
bool CSession::IsThirdJobOrHigher(int job) {
  if (g_session.IsTreeGroupThree(job) == true || g_session.IsTreeGroupTwo(job) == 1) {
    return true;
  }

  switch (job) {
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
  case JT_PORING_SNOVICE2:
  case JT_PORING_SNOVICE2_B:
  case JT_KAGEROU:
  case JT_OBORO:
  case JT_FROG_KAGEROU:
  case JT_FROG_OBORO:
  case JT_REBELLION:
  case JT_PECO_REBELLION:
  case JT_DO_SUMMONER:
  case JT_DO_CART_SUMMONER:
  case JT_DO_SUMMONER_B:
  case JT_DO_CART_SUMMONER_B:
  case JT_KAGEROU_B:
  case JT_OBORO_B:
  case JT_REBELLION_B:
  case JT_PECO_REBELLION_B:
  case JT_STAR_EMPEROR:
  case JT_SOUL_REAPER:
  case JT_STAR_EMPEROR_B:
  case JT_SOUL_REAPER_B:
  case JT_STAR2_EMPEROR:
  case JT_STAR2_EMPEROR_B:
  case JT_HAETAE_STAR_EMPEROR:
  case JT_HAETAE_SOUL_REAPER:
  case JT_HAETAE_STAR_EMPEROR_B:
  case JT_HAETAE_SOUL_REAPER_B:
  case JT_KARNOS:
  case JT_KARNOS_B:
    return true;
  default:
    return false;
  }
}
