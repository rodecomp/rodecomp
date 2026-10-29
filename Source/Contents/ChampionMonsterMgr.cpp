#include "Contents/ChampionMonsterMgr.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
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

// GLOBAL: gl_24813
CChampionMonsterMgr* g_championMonsterMgr;

// OFFSET: 0x00ADDF50
// FUNCTION: fn_17951
__declspec(noinline) void CChampionMonsterMgr::AddItem(int id, float value) {
  this->m_items.insert(std::make_pair(id, lcData(value)));
}

// OFFSET: 0x00ADE1B0
// FUNCTION: fn_17952
bool CChampionMonsterMgr::Create() {
  if (g_championMonsterMgr == nullptr) {
    g_championMonsterMgr = new (std::nothrow) CChampionMonsterMgr();
    if (g_championMonsterMgr != nullptr) {
      if (g_championMonsterMgr->m_isValid) {
        return true;
      }
      delete g_championMonsterMgr;
      g_championMonsterMgr = nullptr;
    }
  }
  return false;
}

// OFFSET: 0x00ADE2A0
// FUNCTION: fn_17953
void CChampionMonsterMgr::Destroy() {
  if (g_championMonsterMgr != nullptr) {
    delete g_championMonsterMgr;
    g_championMonsterMgr = nullptr;
  }
}

// OFFSET: 0x00ADE2C0
// FUNCTION: fn_17954
bool CChampionMonsterMgr::Load() {
  NLuaState lua;
  if (lua.IsValid()) {
    luaL_openlibs(lua.GetState());
    lua_register(lua.GetState(), "AddItem", CChampionMonsterMgr::LuaAddItem);
    lua_register(lua.GetState(), "AddEffect", CChampionMonsterMgr::LuaAddEffect);
    if (luaL_loadfile(lua.GetState(), "System\\monster_size_effect_new.lub") != 0 || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
      MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "ChampionMonsterMgr", 0);
    } else {
      lua_getglobal(lua.GetState(), "main");
      if (lua_pcall(lua.GetState(), 0, 2, 0) != 0) {
        MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "ChampionMonsterMgr", 0);
      } else if (!lua_isstring(lua.GetState(), -1)) {
        MessageBoxA(nullptr, "second return value is not string...", "ChampionMonsterMgr", 0);
      } else {
        if (lua_toboolean(lua.GetState(), -2)) {
          return true;
        }
        std::string message = std::string("error found :") + lua_tostring(lua.GetState(), -1);
        MessageBoxA(nullptr, message.c_str(), "ChampionMonsterMgr", 0);
      }
    }
  }

  return false;
}

// OFFSET: 0x00ADE510
// FUNCTION: fn_17955
int CChampionMonsterMgr::ApplyEffects(int id, CGameActor* actor, int aid) {
  if (this->m_items.empty()) {
    return -1;
  }

  std::map<int, lcData>::iterator it = this->m_items.find(id);
  if (it == this->m_items.end() || actor == nullptr) {
    return -1;
  }

  std::vector<int>& effects = it->second.m_effects;
  if (actor->render.visible && actor->render.enabled && !StateFlag02(actor->Slot13())) {
    for (unsigned int i = 0; i < effects.size(); ++i) {
      if (g_effectMgr->Find(aid, effects[i]) == nullptr) {
        const vector3d& position = actor->render.position;
        if (g_effectMgr->Spawn(aid, effects[i], position.x, position.y, position.z, 0, -1, 0, 0) == nullptr &&
            !actor->state.Attachments().HasId(effects[i])) {
          actor->LaunchEffect(effects[i], actor->render.position, 0.0f);
        }
      }
    }
  } else {
    for (unsigned int i = 0; i < effects.size(); ++i) {
      const int effect = effects[i];
      actor->SendMsg(actor, 109, 0, effect, effect >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }

  return -1;
}

// OFFSET: 0x00ADE6D0
// FUNCTION: fn_17956
static __declspec(noinline) std::string FormatChampionMessage(const char* format, ...) {
  char buffer[2049];
  std::memset(buffer, 0, sizeof(buffer));
  va_list arguments;
  va_start(arguments, format);
  _vsnprintf(buffer, 2048, format, arguments);
  va_end(arguments);
  return std::string(buffer);
}

// OFFSET: 0x00ADE9D0
// FUNCTION: fn_17958
float CChampionMonsterMgr::GetValue(int id) {
  if (!this->m_items.empty()) {
    std::map<int, lcData>::iterator it = this->m_items.find(id);
    if (it != this->m_items.end()) {
      return it->second.m_value;
    }
  }
  return 0.0f;
}

// OFFSET: 0x00ADEA20
// FUNCTION: fn_17959
bool CChampionMonsterMgr::IsChampion(int id) {
  if (!this->m_items.empty()) {
    std::map<int, lcData>::iterator it = this->m_items.find(id);
    if (it != this->m_items.end()) {
      return true;
    }
  }
  return false;
}

// OFFSET: 0x00ADEA70
// FUNCTION: fn_17960
int CChampionMonsterMgr::LuaAddItem(lua_State* state) {
  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatChampionMessage("2nd argment is must number").c_str());
  } else if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatChampionMessage("1st argment is must number").c_str());
  } else {
    int id = static_cast<int>(lua_tonumber(state, -2));
    float value = static_cast<float>(lua_tonumber(state, -1));
    g_championMonsterMgr->AddItem(id, value);
    lua_pushboolean(state, 1);
    lua_pushstring(state, FormatChampionMessage("good").c_str());
  }
  return 2;
}

// OFFSET: 0x00ADEBA0
// FUNCTION: fn_17961
int CChampionMonsterMgr::LuaAddEffect(lua_State* state) {
  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatChampionMessage("2nd argment is must number").c_str());
  } else if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatChampionMessage("1st argment is must number").c_str());
  } else {
    int id = static_cast<int>(lua_tonumber(state, -2));
    int effect = static_cast<int>(lua_tonumber(state, -1));
    g_championMonsterMgr->AddEffect(id, effect);
    lua_pushboolean(state, 1);
    lua_pushstring(state, FormatChampionMessage("good").c_str());
  }
  return 2;
}
