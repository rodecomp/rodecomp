#ifndef RAGNAROK_SOURCE_CONTENTS_MAPPROPERTYMGR_H
#define RAGNAROK_SOURCE_CONTENTS_MAPPROPERTYMGR_H

#include <cstddef>
#include <map>

class CMapProperty {
public:
  CMapProperty();
  virtual ~CMapProperty();

  void SetValue(int key, int value);
  int GetValue(int key);

  /* 0004 */ std::map<int, int> m_values;
};

class CMapPropertyMgr {
public:
  // INLINED
  CMapPropertyMgr() : m_valid(true) {
  }

  // INLINED
  virtual ~CMapPropertyMgr() {
  }

  static bool Create();
  static void Destroy();
  void Clear();
  int GetValue(int mapId, int key);

  /* 0004 */ bool m_valid;
  /* 0008 */ std::map<int, CMapProperty> m_properties;
};

extern CMapPropertyMgr* g_mapPropertyMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMapProperty) == 0xC);
static_assert(sizeof(CMapPropertyMgr) == 0x10);
static_assert(offsetof(CMapPropertyMgr, m_properties) == 0x8);
#endif

#endif
