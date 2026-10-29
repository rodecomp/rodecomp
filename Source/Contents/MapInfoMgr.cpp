#include "Contents/MapInfoMgr.h"
#include "Render/DeviceContext/DeviceContext.h"

extern "C" {
#include <lua.h>
}

#include <algorithm>
#include <new>
#include <utility>

// GLOBAL: gl_24842
CMapInfoMgr* g_mapInfoMgr;

// OFFSET: 0x006CE6A0
// FUNCTION: fn_06483
CMapInfoMgr::~CMapInfoMgr() {
  this->m_maps.clear();
  if (this->m_display != nullptr) {
    delete this->m_display;
    this->m_display = nullptr;
  }
}

// OFFSET: 0x006CE820
// FUNCTION: fn_06484
void CMapInfoMgr::AddMapDisplayName(std::string map, std::string name, bool show) {
  this->m_maps[map].m_displayName = name;
  this->m_maps[map].m_showDisplayName = show;
}

// OFFSET: 0x006CE8C0
// FUNCTION: fn_06485
void CMapInfoMgr::AddMapSignName(std::string map, std::string name, std::string subName) {
  this->m_maps[map].m_sign = MapSignName{name, subName};
}

// OFFSET: 0x006CEAA0
// FUNCTION: fn_06486
bool CMapInfoMgr::Create() {
  if (g_mapInfoMgr == nullptr) {
    g_mapInfoMgr = new (std::nothrow) CMapInfoMgr();
    if (g_mapInfoMgr != nullptr) {
      if (g_mapInfoMgr->m_initialized) {
        return true;
      }

      delete g_mapInfoMgr;
      g_mapInfoMgr = nullptr;
    }
  }

  return false;
}

// OFFSET: 0x006CED70
// FUNCTION: fn_06488
void CMapInfoMgr::Destroy() {
  if (g_mapInfoMgr != nullptr) {
    delete g_mapInfoMgr;
    g_mapInfoMgr = nullptr;
  }
}

// OFFSET: 0x006CED90
// FUNCTION: fn_06489
const char* CMapInfoMgr::GetDisplayName(std::string map) {
  const MapInfo* info = this->FindMapInfo(map);
  if (info == nullptr) {
    return this->m_emptyName.c_str();
  }

  return info->m_displayName.c_str();
}

// OFFSET: 0x006CEE20
// FUNCTION: fn_06490
MapInfo* CMapInfoMgr::FindMapInfo(std::string map) {
  std::string name = map;
  const std::size_t at = name.rfind('@');
  if (at != std::string::npos && at > 3) {
    name.erase(at - 4, 3);
  }

  const auto found = std::find_if(this->m_maps.begin(), this->m_maps.end(), [name](std::pair<std::string, MapInfo> entry) {
    return entry.first.compare(name) == 0;
  });
  if (found == this->m_maps.end()) {
    return nullptr;
  }

  return &found->second;
}

// OFFSET: 0x006CEF60
// FUNCTION: fn_06491
const char* CMapInfoMgr::GetSignSubName(std::string map) {
  const MapInfo* info = this->FindMapInfo(map);
  if (info == nullptr) {
    return this->m_emptyName.c_str();
  }

  return info->m_sign.m_subName.c_str();
}

// OFFSET: 0x006CF600
// FUNCTION: fn_06495
int CMapInfoMgr::L_AddMapBackgroundBmp(lua_State* state) {
  if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argument 1nd is must string");
  } else {
    const std::string map = lua_tostring(state, -2);
    if (!lua_isstring(state, -1)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, "argument 2st is must string");
    } else {
      const std::string bmp = lua_tostring(state, -1);
      lua_pushboolean(state, 1);
      lua_pushstring(state, "good");
      g_mapInfoMgr->AddMapBackgroundBmp(map, bmp);
    }
  }

  return 2;
}

// OFFSET: 0x006CF860
// FUNCTION: fn_06496
int CMapInfoMgr::L_AddMapDisplayName(lua_State* state) {
  if (!lua_isstring(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argument 1st is must string");
  } else {
    const std::string map = lua_tostring(state, -3);
    if (!lua_isstring(state, -2)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, "argument 2nd is must string");
    } else {
      const std::string name = lua_tostring(state, -2);
      if (lua_type(state, -1) != LUA_TBOOLEAN) {
        lua_pushboolean(state, 0);
        lua_pushstring(state, "argument 3th is must boolean");
      } else {
        const bool show = lua_toboolean(state, -1) > 0;
        lua_pushboolean(state, 1);
        lua_pushstring(state, "good");
        g_mapInfoMgr->AddMapDisplayName(map, name, show);
      }
    }
  }

  return 2;
}

// OFFSET: 0x006CFB10
// FUNCTION: fn_06497
int CMapInfoMgr::L_AddMapSignName(lua_State* state) {
  if (!lua_isstring(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argument 1st is must string");
  } else {
    const std::string map = lua_tostring(state, -3);
    if (!lua_isstring(state, -2)) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, "argument 2nd is must string");
    } else {
      const std::string name = lua_tostring(state, -2);
      if (!lua_isstring(state, -1)) {
        lua_pushboolean(state, 0);
        lua_pushstring(state, "argument 3st is must string");
      } else {
        const std::string subName = lua_tostring(state, -1);
        lua_pushboolean(state, 1);
        lua_pushstring(state, "good");
        g_mapInfoMgr->AddMapSignName(map, name, subName);
      }
    }
  }

  return 2;
}
