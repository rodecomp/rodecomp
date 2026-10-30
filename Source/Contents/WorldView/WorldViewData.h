#ifndef RAGNAROK_SOURCE_CONTENTS_WORLDVIEW_WORLDVIEWDATA_H
#define RAGNAROK_SOURCE_CONTENTS_WORLDVIEW_WORLDVIEWDATA_H

#include <cstddef>
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

#if defined(_WIN32) && !defined(_WIN64)
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
