#include "Contents/CheckAttendanceMgr.h"
#include "Script/NLuaState.h"
#include "Utility/Text/FormatString.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <new>
#include <string>
#include <utility>
#include <windows.h>

// GLOBAL: gl_24876
CCheckAttendanceMgr* g_checkAttendanceMgr;

// OFFSET: 0x00671040
// FUNCTION: fn_05349
bool CCheckAttendanceMgr::Create() {
  if (g_checkAttendanceMgr != nullptr) {
    return false;
  }

  g_checkAttendanceMgr = new (std::nothrow) CCheckAttendanceMgr();
  if (g_checkAttendanceMgr == nullptr) {
    return false;
  }

  if (!g_checkAttendanceMgr->m_isValid) {
    delete g_checkAttendanceMgr;
    g_checkAttendanceMgr = nullptr;
    return false;
  }

  if (!g_checkAttendanceMgr->Load()) {
    if (g_checkAttendanceMgr != nullptr) {
      delete g_checkAttendanceMgr;
    }
    g_checkAttendanceMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x00671150
// FUNCTION: fn_05350
void CCheckAttendanceMgr::Destroy() {
  if (g_checkAttendanceMgr != nullptr) {
    delete g_checkAttendanceMgr;
    g_checkAttendanceMgr = nullptr;
  }
}

// OFFSET: 0x00671170
// FUNCTION: fn_05351
std::pair<int, int> CCheckAttendanceMgr::GetReward(int day) {
  const auto found = this->m_rewards.find(day);
  if (found == this->m_rewards.end()) {
    return std::pair<int, int>(0, 0);
  }

  return std::pair<int, int>(found->second.m_item, found->second.m_count);
}

// OFFSET: 0x00671200
// FUNCTION: fn_05352
bool CCheckAttendanceMgr::Load() {
  NLuaState lua;
  if (lua.IsValid()) {
    luaL_openlibs(lua.GetState());
    lua_register(lua.GetState(), "InsertCheckAttendanceConfig", CCheckAttendanceMgr::L_InsertCheckAttendanceConfig);
    lua_register(lua.GetState(), "InsertCheckAttendanceReward", CCheckAttendanceMgr::L_InsertCheckAttendanceReward);
    if (luaL_loadfile(lua.GetState(), "System\\CheckAttendance.lub") != 0 || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
      MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CCheckAttendanceMgr", 0);
    } else {
      lua_getglobal(lua.GetState(), "main");
      if (lua_pcall(lua.GetState(), 0, 2, 0) != 0) {
        MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CCheckAttendanceMgr", 0);
      } else if (!lua_isstring(lua.GetState(), -1)) {
        MessageBoxA(nullptr, "second return value is not string...", "CCheckAttendanceMgr", 0);
      } else {
        if (lua_toboolean(lua.GetState(), -2)) {
          return true;
        }
        const std::string message = FormatOwnedString("error : %s", lua_tostring(lua.GetState(), -1));
        MessageBoxA(nullptr, message.c_str(), "CCheckAttendanceMgr", 0);
      }
    }
  }

  return false;
}

// OFFSET: 0x006713B0

// OFFSET: 0x00671960
// FUNCTION: fn_05355
int CCheckAttendanceMgr::L_InsertCheckAttendanceConfig(lua_State* state) {
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "[InsertCheckAttendanceConfig] argument 1st is must number");
    return 2;
  }
  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "[InsertCheckAttendanceConfig] argument 2nd is must number");
    return 2;
  }

  const int config0 = static_cast<int>(lua_tonumber(state, -2));
  const int config1 = static_cast<int>(lua_tonumber(state, -1));
  g_checkAttendanceMgr->m_config0 = config0;
  g_checkAttendanceMgr->m_config1 = config1;
  lua_pushboolean(state, 1);
  lua_pushstring(state, "good");
  return 2;
}

// OFFSET: 0x00671A20
// FUNCTION: fn_05356
int CCheckAttendanceMgr::L_InsertCheckAttendanceReward(lua_State* state) {
  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "[InsertCheckAttendanceReward] argument 1st is must number");
    return 2;
  }
  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "[InsertCheckAttendanceReward] argument 2nd is must string");
    return 2;
  }
  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "[InsertCheckAttendanceReward] argument 3rd is must number");
    return 2;
  }

  const int day = static_cast<int>(lua_tonumber(state, -3));
  const int item = static_cast<int>(lua_tonumber(state, -2));
  std::string message;
  if (item == 0) {
    lua_pushboolean(state, 0);
    message = FormatOwnedString("[InsertCheckAttendanceReward] Check ItemID : %d", 0);
  } else {
    const int count = static_cast<int>(lua_tonumber(state, -1));
    if (count < 1) {
      lua_pushboolean(state, 0);
      message = FormatOwnedString("[InsertCheckAttendanceReward] Check ItemCnt : [%d] %d", item, count);
    } else if (g_checkAttendanceMgr->m_rewards.insert(std::make_pair(day, lcReward(item, count))).second) {
      lua_pushboolean(state, 1);
      lua_pushstring(state, "good");
      return 2;
    } else {
      lua_pushboolean(state, 0);
      message = FormatOwnedString("[InsertCheckAttendanceReward] Check RewardInfo : %d [%s] %d", day, item, count);
    }
  }

  lua_pushstring(state, message.c_str());
  return 2;
}
