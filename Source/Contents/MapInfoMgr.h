#ifndef RAGNAROK_SOURCE_CONTENTS_MAPINFOMGR_H
#define RAGNAROK_SOURCE_CONTENTS_MAPINFOMGR_H

#include <map>
#include <string>

#include <windows.h>

class CDC;
class CRenderSystemBase;
class CTexture;
struct lua_State;

struct MapSignName {
  /* 0000 */ std::string m_name;
  /* 0018 */ std::string m_subName;
};

struct MapInfo {
  /* 0000 */ std::string m_displayName;
  /* 0018 */ bool m_showDisplayName;
  /* 001C */ MapSignName m_sign;
  /* 004C */ std::string m_backgroundBmp;
};

struct MapNameDisplay {
  /* 0000 */ CTexture* m_texture;
  /* 0004 */ CDC* m_dc;
  /* 0008 */ unsigned short m_width;
  /* 000A */ unsigned short m_height;
  /* 000C */ DWORD m_startTime;

  MapNameDisplay(std::string name, std::string subName, std::string bmp);
  // INLINED
  ~MapNameDisplay() {
    if (this->m_dc != nullptr) {
      delete this->m_dc;
      this->m_dc = nullptr;
    }
  }
};

class CMapInfoMgr {
public:
  static bool Create();
  static void Destroy();
  static int L_AddMapBackgroundBmp(lua_State* state);
  static int L_AddMapDisplayName(lua_State* state);
  static int L_AddMapSignName(lua_State* state);

  /* 0004 */ bool m_initialized;
  /* 0008 */ std::map<std::string, MapInfo> m_maps;
  /* 0010 */ std::string m_emptyName;
  /* 0028 */ MapNameDisplay* m_display;

  virtual ~CMapInfoMgr();

  // INLINED
  CMapInfoMgr() : m_display(nullptr) {
    this->m_initialized = true;
  }

  void AddMapDisplayName(std::string map, std::string name, bool show);
  void AddMapSignName(std::string map, std::string name, std::string subName);
  // INLINED
  void AddMapBackgroundBmp(std::string map, std::string bmp) {
    this->m_maps[map].m_backgroundBmp = bmp;
  }
  void ShowMapName(std::string map);
  MapInfo* FindMapInfo(std::string map);
  const char* GetDisplayName(std::string map);
  const char* GetSignSubName(std::string map);
  bool LoadScript();
  void DrawMapName(CRenderSystemBase& system);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MapInfo) == 0x64);
static_assert(sizeof(MapNameDisplay) == 0x10);
static_assert(sizeof(CMapInfoMgr) == 0x2C);
#endif

extern CMapInfoMgr* g_mapInfoMgr;

#endif
