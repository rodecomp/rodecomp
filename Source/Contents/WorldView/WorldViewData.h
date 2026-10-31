#ifndef RAGNAROK_SOURCE_CONTENTS_WORLDVIEW_WORLDVIEWDATA_H
#define RAGNAROK_SOURCE_CONTENTS_WORLDVIEW_WORLDVIEWDATA_H

#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

class CTexture;

struct WorldViewArea {
  /* 0000 */ int left;
  /* 0004 */ int top;
  /* 0008 */ int right;
  /* 000C */ int bottom;
  /* 0010 */ int unknown_0010;
};

class CWorldViewField {
public:
  CWorldViewField(WorldViewArea area, const char* name, const char* unknown2, const char* unknown3);

  /* 0000 */ int left = 0;
  /* 0004 */ int top = 0;
  /* 0008 */ int right = 0;
  /* 000C */ int bottom = 0;
  /* 0010 */ int unknown_0010 = 0;
  /* 0014 */ std::string name;
  /* 002C */ std::string unknown_002C;
  /* 0044 */ std::string unknown_0044;
};

class CWorldViewMap {
public:
  CWorldViewField* FindField(const char* name);

  /* 0000 */ std::string name;
  /* 0018 */ std::string displayName;
  /* 0030 */ int id = 0;
  /* 0034 */ int left = 0;
  /* 0038 */ int top = 0;
  /* 003C */ int right = 0;
  /* 0040 */ int bottom = 0;
  /* 0044 */ int unknown_0044 = 0;
  /* 0048 */ std::vector<CWorldViewField*> fields;
};

class CWorldViewWorld {
public:
  CWorldViewWorld(const char* name, const char* imageName);
  ~CWorldViewWorld();

  void AddField(int mapId, const char* name, CWorldViewField* field);
  bool LoadTexture();
  CWorldViewField* FieldAt(unsigned int& mapIndex, int x, int y);
  CWorldViewField* FindField(unsigned int& mapIndex, const char* name);
  CWorldViewField* FindField(const char* name);
  CWorldViewMap* FindMap(int id);
  CWorldViewMap* MapAt(unsigned int& mapIndex, int x, int y);
  void Draw(int unused, int x, int y, int width, int height);

  /* 0000 */ std::string name;
  /* 0018 */ std::string imageName;
  /* 0030 */ CTexture* texture = nullptr;
  /* 0034 */ std::vector<CWorldViewMap*> maps;
};

struct WorldViewRecord {
  /* 0000 */ int worldIndex;
  /* 0004 */ int mapIndex;
  /* 0008 */ int level;
  /* 000C */ int unknown_000C = 0;
  /* 0010 */ int unknown_0010 = 0;
  /* 0014 */ int unknown_0014 = 0;
  /* 0018 */ int unknown_0018 = 0;
  /* 001C */ int unknown_001C = 0;
  /* 0020 */ std::string name;
  /* 0038 */ int left = 0;
  /* 003C */ int top = 0;
  /* 0040 */ int right = 0;
  /* 0044 */ int bottom = 0;
  /* 0048 */ int unknown_0048 = 0;
  /* 004C */ std::list<std::string> unknown_004C;
};

class CWorldViewMgr {
public:
  CWorldViewMgr();

  bool ScaleToScreen();
  void sub_6168F0();
  unsigned int FindWorld(int mapId);
  void ResetSelection();
  bool SelectField(const char* name);
  void Draw(int unused, int x, int y, int width, int height);
  bool AddMark(const char* name);
  void ClearMarks();
  WorldViewRecord* MarkAt(unsigned int index, bool currentWorldOnly);
  int* MarkAreaAt(unsigned int index, bool currentWorldOnly);
  int MarkCount();
  void SelectCurrentWorld();
  bool AddLevel(const char* name, int level);
  void ClearLevels();
  int* LevelAreaAt(int& kind, unsigned int index, bool currentWorldOnly);
  int LevelCount();
  CWorldViewWorld* CurrentWorld();

  /* 0000 */ std::vector<WorldViewRecord> marks;
  /* 000C */ std::vector<WorldViewRecord> levels;
  /* 0018 */ WorldViewRecord selection;
  /* 006C */ int currentWorld;
  /* 0070 */ int screenScale;
  /* 0074 */ std::vector<CWorldViewWorld*> worlds;
  /* 0080 */ std::uint32_t dungeonColor;
  /* 0084 */ std::uint32_t unknown_0084;
  /* 0088 */ std::uint32_t dungeonFontColor;
  /* 008C */ std::uint32_t dungeonFontOutline;
  /* 0090 */ int dungeonFontSize[4];
  /* 00A0 */ std::uint32_t dungeonSelectColor;
  /* 00A4 */ std::uint32_t unknown_00A4;
  /* 00A8 */ std::uint32_t mobColor;
  /* 00AC */ std::uint32_t mobOutline;
  /* 00B0 */ int mobFont[4];
  /* 00C0 */ std::uint32_t mobLevelColor;
  /* 00C4 */ std::uint32_t mobLevelOutline;
  /* 00C8 */ int mobLevelFont[4];
  /* 00D8 */ std::uint32_t boxLineColor;
  /* 00DC */ std::uint32_t searchColors[16];
  /* 011C */ std::string searchImages[16];
  /* 029C */ std::uint32_t runColor;
  /* 02A0 */ std::uint32_t runFontColor;
  /* 02A4 */ std::uint32_t runFontOutline;
  /* 02A8 */ int runFont[4];
  /* 02B8 */ std::uint32_t selectBoxColor;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(WorldViewRecord) == 0x54);
static_assert(offsetof(WorldViewRecord, name) == 0x20);
static_assert(offsetof(WorldViewRecord, left) == 0x38);
static_assert(offsetof(WorldViewRecord, unknown_004C) == 0x4C);
static_assert(offsetof(CWorldViewMgr, selection) == 0x18);
static_assert(offsetof(CWorldViewMgr, currentWorld) == 0x6C);
static_assert(offsetof(CWorldViewMgr, worlds) == 0x74);
static_assert(offsetof(CWorldViewMgr, dungeonFontSize) == 0x90);
static_assert(offsetof(CWorldViewMgr, boxLineColor) == 0xD8);
static_assert(offsetof(CWorldViewMgr, searchImages) == 0x11C);
static_assert(offsetof(CWorldViewMgr, runColor) == 0x29C);
static_assert(offsetof(CWorldViewMgr, selectBoxColor) == 0x2B8);
static_assert(sizeof(WorldViewArea) == 0x14);
static_assert(sizeof(CWorldViewField) == 0x5C);
static_assert(offsetof(CWorldViewField, name) == 0x14);
static_assert(offsetof(CWorldViewField, unknown_0044) == 0x44);
static_assert(sizeof(CWorldViewMap) == 0x54);
static_assert(offsetof(CWorldViewMap, id) == 0x30);
static_assert(offsetof(CWorldViewMap, fields) == 0x48);
static_assert(sizeof(CWorldViewWorld) == 0x40);
static_assert(offsetof(CWorldViewWorld, texture) == 0x30);
static_assert(offsetof(CWorldViewWorld, maps) == 0x34);
#endif

#endif
