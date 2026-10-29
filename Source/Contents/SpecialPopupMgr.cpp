#include "Contents/SpecialPopupMgr.h"

extern "C" {
#include <lua.h>
}

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <new>

// GLOBAL: gl_24839
CSpecialPopupMgr* g_specialPopupMgr = nullptr;

// OFFSET: 0x007A7930
// FUNCTION: fn_09052
bool CSpecialPopupMgr::Create() {
  if (g_specialPopupMgr) {
    return false;
  }

  g_specialPopupMgr = new (std::nothrow) CSpecialPopupMgr();
  if (!g_specialPopupMgr) {
    return false;
  }

  if (!g_specialPopupMgr->m_initialized) {
    delete g_specialPopupMgr;
    g_specialPopupMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x007A7A20
// FUNCTION: fn_09053
void CSpecialPopupMgr::Destroy() {
  if (g_specialPopupMgr) {
    delete g_specialPopupMgr;
    g_specialPopupMgr = nullptr;
  }
}

// OFFSET: 0x007A7A40
// FUNCTION: fn_09054
PopupData* CSpecialPopupMgr::GetPopup(int id) {
  std::map<int, PopupData>::iterator it = this->m_popups.find(id);
  if (it == this->m_popups.end()) {
    return nullptr;
  }

  return &it->second;
}

// OFFSET: 0x007A7DB0
// FUNCTION: fn_09056
std::string FormatPopupString(const char* format, ...) {
  char buffer[2049];
  std::memset(buffer, 0, sizeof(buffer));

  va_list arguments;
  va_start(arguments, format);
  _vsnprintf(buffer, 2048, format, arguments);
  return std::string(buffer);
}

// OFFSET: 0x007A80B0
// FUNCTION: fn_09058
int CSpecialPopupMgr::LuaAddDesc(lua_State* state) {
  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatPopupString("2nd argment is must string").c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatPopupString("1st argment is must number").c_str());
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -2));
  std::string description = lua_tostring(state, -1);
  g_specialPopupMgr->AddDescription(id, description);
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatPopupString("good").c_str());
  return 2;
}

// OFFSET: 0x007A82D0
// FUNCTION: fn_09059
int CSpecialPopupMgr::LuaAddMsg(lua_State* state) {
  const int id = static_cast<int>(lua_tonumber(state, -3));
  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatPopupString("3th argment is must number [itemID:%d]", id).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatPopupString("2nd argment is must number [itemID:%d]", id).c_str());
    return 2;
  }

  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, FormatPopupString("1st argment is must number [itemID:%d]", id).c_str());
    return 2;
  }

  const int messageId = static_cast<int>(lua_tonumber(state, -2));
  const unsigned int color = static_cast<unsigned int>(lua_tonumber(state, -1));
  g_specialPopupMgr->m_popups[id].m_messageId = messageId;
  g_specialPopupMgr->m_popups[id].m_color = ((((color >> 8) & 0xFF) | ((color & 0xFF) << 8)) << 8) | ((color >> 16) & 0xFF);
  lua_pushboolean(state, 1);
  lua_pushstring(state, FormatPopupString("good").c_str());
  return 2;
}
