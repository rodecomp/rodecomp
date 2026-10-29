#include "Contents/ChampionMonsterMgr.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
}
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <new>
#include <string>
#include <utility>

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
