#include "Contents/PrivateAirplaneMgr.h"
#include "Script/NLuaState.h"
#include "Utility/Text/FormatString.h"

#include <new>

#include <Windows.h>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

// GLOBAL: gl_24636
CPrivateAirplaneMgr* g_privateAirplaneItems;

// OFFSET: 0x006D04C0
// FUNCTION: fn_06504
bool CPrivateAirplaneMgr::Create() {
  if (g_privateAirplaneItems != nullptr) {
    return false;
  }

  g_privateAirplaneItems = new (std::nothrow) CPrivateAirplaneMgr();
  if (g_privateAirplaneItems != nullptr) {
    if (g_privateAirplaneItems->m_initialized) {
      if (CPrivateAirplaneMgr::Init()) {
        return true;
      }
    }

    delete g_privateAirplaneItems;
    g_privateAirplaneItems = nullptr;
  }

  return false;
}

// OFFSET: 0x006D0620
// FUNCTION: fn_06505
void CPrivateAirplaneMgr::Destroy() {
  if (g_privateAirplaneItems != nullptr) {
    delete g_privateAirplaneItems;
    g_privateAirplaneItems = nullptr;
  }
}

// OFFSET: 0x006D0640
// FUNCTION: fn_06506
int CPrivateAirplaneMgr::GetNeedItem(int index) {
  auto it = this->m_needItems.begin();
  for (int count = 0; count < index; ++count) {
    if (it == this->m_needItems.end()) {
      break;
    }
    ++it;
  }

  return *it;
}

// OFFSET: 0x006D0860
// FUNCTION: fn_06507
bool CPrivateAirplaneMgr::IsEndableMap(std::string map) {
  return this->m_endableMaps.find(map) != this->m_endableMaps.end();
}

// OFFSET: 0x006D0950
// FUNCTION: fn_06508
bool CPrivateAirplaneMgr::IsStartableMap(std::string map) {
  return this->m_startableMaps.find(map) != this->m_startableMaps.end();
}

// OFFSET: 0x006D0A00
// FUNCTION: fn_06509
bool CPrivateAirplaneMgr::Init() {
  NLuaState lua;
  if (!lua.IsValid()) {
    return false;
  }

  luaL_openlibs(lua.GetState());
  lua_register(lua.GetState(), "InsertStartableMap", CPrivateAirplaneMgr::L_InsertStartableMap);
  lua_register(lua.GetState(), "InsertEndableMap", CPrivateAirplaneMgr::L_InsertEndableMap);
  lua_register(lua.GetState(), "InsertNeedItem", CPrivateAirplaneMgr::L_InsertNeedItem);

  std::string filename = "System\\PrivateAirplane_True.lub";
  if (luaL_loadfile(lua.GetState(), filename.c_str()) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CPrivateAirplaneMgr", 0);
    return false;
  }

  if (lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CPrivateAirplaneMgr", 0);
    return false;
  }

  lua_getfield(lua.GetState(), LUA_GLOBALSINDEX, "main");
  if (lua_pcall(lua.GetState(), 0, 2, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CPrivateAirplaneMgr", 0);
    return false;
  }

  if (lua_isstring(lua.GetState(), -1) == 0) {
    MessageBoxA(nullptr, "second return value is not string...", "CPrivateAirplaneMgr", 0);
    return false;
  }

  if (lua_toboolean(lua.GetState(), -2) == 0) {
    std::string msg = FormatOwnedString("error : %s", lua_tostring(lua.GetState(), -1));
    MessageBoxA(nullptr, msg.c_str(), "CPrivateAirplaneMgr", 0);
    return false;
  }

  return true;
}

// OFFSET: 0x006D0EF0
// FUNCTION: fn_06511
int CPrivateAirplaneMgr::L_InsertNeedItem(lua_State* state) {
  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "[InsertNeedItem] argument 1st is must string");
  } else {
    const std::string name = lua_tostring(state, -1);
    const int item = static_cast<int>(lua_tonumber(state, -2));
    g_privateAirplaneItems->m_needItems.insert(item);
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x006D10E0
// FUNCTION: fn_06512
int CPrivateAirplaneMgr::L_InsertEndableMap(lua_State* state) {
  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "[InsertEndableMap] argument 1st is must string");
  } else {
    const std::string map = lua_tostring(state, -1) + std::string(".gat");
    g_privateAirplaneItems->InsertEndableMap(map);
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}

// OFFSET: 0x006D1260
// FUNCTION: fn_06513
int CPrivateAirplaneMgr::L_InsertStartableMap(lua_State* state) {
  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "[InsertStartableMap] argument 1st is must string");
  } else {
    const std::string map = lua_tostring(state, -1) + std::string(".gat");
    g_privateAirplaneItems->InsertStartableMap(map);
    lua_pushboolean(state, 1);
    lua_pushstring(state, "good");
  }

  return 2;
}
