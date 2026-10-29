#include "Contents/MapInfoMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Render/DeviceContext/DCBitmap.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Text/TextContext.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Script/NLuaState.h"
#include "Utility/Text/FormatString.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <timeapi.h>

#include <algorithm>
#include <cstring>
#include <new>
#include <utility>

// GLOBAL: gl_24842
CMapInfoMgr* g_mapInfoMgr;

// OFFSET: 0x006CDE30
// FUNCTION: fn_06480
MapNameDisplay::MapNameDisplay(std::string name, std::string subName, std::string bmp) {
  const std::string path = FormatOwnedString("유저인터페이스\\display_mapname\\%s.png", bmp.c_str());
  this->m_texture = g_texMgr.Load(path.c_str(), 0, false, true, false);
  if (this->m_texture != nullptr) {
    this->m_width = static_cast<unsigned short>(g_renderer->viewport.width);
    this->m_height = static_cast<unsigned short>(static_cast<float>(g_renderer->viewport.height) * 0.2f);
    const float scale = static_cast<float>(this->m_height) / static_cast<float>(this->m_texture->Height());

    this->m_dc = CreateBitmapDC(this->m_width, this->m_height, false);
    this->m_dc->Fill(nullptr, 0);
    static_cast<CDCBitmap*>(this->m_dc)->SetAlpha(true);

    TextContext text(*this->m_dc);
    text.SetColor(0xFFFFFF);
    const int family = g_serviceType == 0 ? 29 : 0;

    text.SetFont(family, static_cast<int>(scale * 16.0f), 1, 0);
    SIZE size = text.Measure(name.c_str(), static_cast<int>(std::strlen(name.c_str())));
    text.Draw(
      static_cast<int>(static_cast<float>(this->m_width - size.cx) * 0.5f), static_cast<int>(scale * 72.0f), name.c_str(),
      static_cast<int>(std::strlen(name.c_str()))
    );

    text.SetFont(family, static_cast<int>(scale * 44.0f), 1, 0);
    size = text.Measure(subName.c_str(), static_cast<int>(std::strlen(subName.c_str())));
    text.Draw(
      static_cast<int>(static_cast<float>(this->m_width - size.cx) * 0.5f), static_cast<int>(scale * 95.0f), subName.c_str(),
      static_cast<int>(std::strlen(subName.c_str()))
    );

    static_cast<CDCBitmap*>(this->m_dc)->SetAlpha(static_cast<unsigned char>(0xFF));
    this->m_startTime = timeGetTime();
  }
}

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

// OFFSET: 0x006CEBB0
// FUNCTION: fn_06487
void CMapInfoMgr::ShowMapName(std::string map) {
  const MapInfo* info = this->FindMapInfo(map);
  if (info == nullptr || !info->m_showDisplayName) {
    return;
  }

  std::string bmp = info->m_backgroundBmp;
  if (bmp.size() == 0) {
    bmp.assign("noname", 6);
  }

  std::string name = info->m_sign.m_name;
  std::string subName = info->m_sign.m_subName;
  if (name.size() == 0 && subName.size() == 0) {
    subName = this->m_emptyName;
  }

  if (this->m_display != nullptr) {
    delete this->m_display;
    this->m_display = nullptr;
  }

  this->m_display = new MapNameDisplay(name, subName, bmp);
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

// OFFSET: 0x006CEFF0
// FUNCTION: fn_06492
bool CMapInfoMgr::LoadScript() {
  this->m_emptyName = LookupMessage(187);

  NLuaState lua;
  if (lua.IsValid()) {
    luaL_openlibs(lua.GetState());
    lua_register(lua.GetState(), "AddMapDisplayName", CMapInfoMgr::L_AddMapDisplayName);
    lua_register(lua.GetState(), "AddMapSignName", CMapInfoMgr::L_AddMapSignName);
    lua_register(lua.GetState(), "AddMapBackgroundBmp", CMapInfoMgr::L_AddMapBackgroundBmp);

    if (luaL_loadfile(lua.GetState(), "system\\mapInfo_true.lub") == 0 && lua_pcall(lua.GetState(), 0, 0, 0) == 0) {
      lua_getglobal(lua.GetState(), "main");
      if (lua_pcall(lua.GetState(), 0, 2, 0) == 0) {
        return true;
      }
    }

    MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CMapInfoMgr", MB_OK);
  }

  return false;
}

// OFFSET: 0x006CF290
// FUNCTION: fn_06493
void CMapInfoMgr::DrawMapName(CRenderSystemBase& system) {
  MapNameDisplay* display = this->m_display;
  if (display == nullptr) {
    return;
  }

  if (display->m_texture == nullptr) {
    delete this->m_display;
    this->m_display = nullptr;
    return;
  }

  int alpha = 255;
  const DWORD elapsed = timeGetTime() - display->m_startTime;
  if (elapsed > 5000) {
    alpha = 5255 - elapsed;
    if (alpha <= 0) {
      delete this->m_display;
      this->m_display = nullptr;
      return;
    }
  }

  const std::uint32_t color = (alpha << 24) | 0xFFFFFF;
  const float scale = static_cast<float>(display->m_height) / static_cast<float>(display->m_texture->Height());
  const int width = static_cast<int>(static_cast<float>(display->m_texture->Width()) * scale);
  display->m_texture->StretchTintedTo(
    g_renderer->primitives, g_renderer->queues, static_cast<int>(static_cast<float>(display->m_width - width) * 0.5f),
    static_cast<int>(static_cast<float>(g_renderer->viewport.height) * 0.12f), width, display->m_height, color, g_renderer->fog.supported != 0,
    g_depthStart, g_depthEnd
  );
  display->m_dc->Draw(system, 0, static_cast<int>(static_cast<float>(g_renderer->viewport.height) * 0.12f), display->m_width, display->m_height, color);
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
