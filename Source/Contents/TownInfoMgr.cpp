#include "Contents/TownInfoMgr.h"
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

// GLOBAL: gl_24817
CTownInfoMgr* g_townInfoMgr;

// OFFSET: 0x00AE1A60
// FUNCTION: fn_18003
TownInfo::TownInfo(int type, std::string name, int x, int y, std::string image)
    : type(type), name(name), x(x), y(y), image(image) {
}

// OFFSET: 0x00AE1B00
// FUNCTION: fn_18004
TownInfo::TownInfo() : type(-1), name(""), x(-1), y(-1), image("") {
}

// OFFSET: 0x00AE1F70
// FUNCTION: fn_18007
void CTownInfoMgr::AddTownInfo(std::string mapName, std::string name, int x, int y, int type) {
  // GLOBAL: gl_24866
  static std::string icons[10] = {
    "유저인터페이스\\Information\\Store.bmp",
    "유저인터페이스\\Information\\Weaponshop.bmp",
    "유저인터페이스\\Information\\ArmorShops.bmp",
    "유저인터페이스\\Information\\Smithy.bmp",
    "유저인터페이스\\Information\\Guide.bmp",
    "유저인터페이스\\Information\\Inn.bmp",
    "유저인터페이스\\Information\\kafra.bmp",
    "유저인터페이스\\Information\\style.bmp",
    "유저인터페이스\\Information\\warp.bmp",
    "유저인터페이스\\minimap\\Quest.bmp",
  };

  std::map<std::string, std::vector<TownInfo>>::iterator it = this->m_towns.find(mapName);
  if (it == this->m_towns.end()) {
    TownInfo info(type, name, x, y, icons[type]);
    std::vector<TownInfo> list;
    list.push_back(info);
    this->m_towns.insert(std::make_pair(mapName, list));
  } else {
    TownInfo info(type, name, x, y, icons[type]);
    it->second.push_back(info);
  }
}

// OFFSET: 0x00AE2740
// FUNCTION: fn_18008
bool CTownInfoMgr::Create() {
  if (g_townInfoMgr == nullptr) {
    g_townInfoMgr = new (std::nothrow) CTownInfoMgr();
    if (g_townInfoMgr != nullptr) {
      if (g_townInfoMgr->m_isValid) {
        return true;
      }
      delete g_townInfoMgr;
      g_townInfoMgr = nullptr;
    }
  }
  return false;
}

// OFFSET: 0x00AE2840
// FUNCTION: fn_18009
void CTownInfoMgr::Destroy() {
  if (g_townInfoMgr != nullptr) {
    delete g_townInfoMgr;
    g_townInfoMgr = nullptr;
  }
}

// OFFSET: 0x00AE2860
// FUNCTION: fn_18010
bool CTownInfoMgr::LoadScript() {
  NLuaState lua;
  if (lua.IsValid()) {
    luaL_openlibs(lua.GetState());
    lua_register(lua.GetState(), "AddTownInfo", CTownInfoMgr::L_AddTownInfo);

    if (luaL_loadfile(lua.GetState(), "System\\Towninfo.lub") == 0 && lua_pcall(lua.GetState(), 0, 0, 0) == 0) {
      lua_getglobal(lua.GetState(), "main");
      if (lua_pcall(lua.GetState(), 0, 2, 0) == 0) {
        if (!lua_isstring(lua.GetState(), -1)) {
          MessageBoxA(nullptr, "second return value is not string...", "CTownInfoMgr", MB_OK);
          return false;
        }

        if (lua_toboolean(lua.GetState(), -2)) {
          return true;
        }

        MessageBoxA(nullptr, (std::string("error found :") + lua_tostring(lua.GetState(), -1)).c_str(), "CTownInfoMgr", MB_OK);
        return false;
      }
    }

    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CTownInfoMgr", MB_OK);
  }

  return false;
}

// OFFSET: 0x00AE2A90
// FUNCTION: fn_18011
std::string FormatTownInfoString(const char* format, ...) {
  char buffer[2049];
  std::memset(buffer, 0, sizeof(buffer));

  va_list arguments;
  va_start(arguments, format);
  _vsnprintf(buffer, 2048, format, arguments);
  return std::string(buffer);
}

// OFFSET: 0x00AE2F40
// FUNCTION: fn_18018
TownInfo CTownInfoMgr::GetTownInfo(std::string mapName, unsigned int index) {
  std::string key = mapName;
  const std::string::size_type dot = key.find_first_of(".");
  if (dot != std::string::npos) {
    key = key.substr(0, dot);
  }

  std::map<std::string, std::vector<TownInfo>>::iterator it = this->m_towns.find(key);
  if (it != this->m_towns.end() && index < it->second.size()) {
    return it->second[index];
  }

  return TownInfo();
}

// OFFSET: 0x00AE33A0
// FUNCTION: fn_18020
int CTownInfoMgr::L_AddTownInfo(lua_State* state) {
  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatTownInfoString("5th argment is must number").c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatTownInfoString("4th argment is must number").c_str());
    return 2;
  }

  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatTownInfoString("3th argment is must number").c_str());
    return 2;
  }

  if (!lua_isstring(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatTownInfoString("2nd argment is must string").c_str());
    return 2;
  }

  if (!lua_isstring(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatTownInfoString("1st argment is must string").c_str());
    return 2;
  }

  std::string mapName = lua_tostring(state, -5);
  std::string name = lua_tostring(state, -4);
  const int x = static_cast<int>(lua_tonumber(state, -3));
  const int y = static_cast<int>(lua_tonumber(state, -2));
  const int type = static_cast<int>(lua_tonumber(state, -1));
  g_townInfoMgr->AddTownInfo(mapName, name, x, y, type);
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatTownInfoString("good").c_str());
  return 2;
}
