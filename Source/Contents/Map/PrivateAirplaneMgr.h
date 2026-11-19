#ifndef RAGNAROK_SOURCE_CONTENTS_MAP_PRIVATEAIRPLANEMGR_H
#define RAGNAROK_SOURCE_CONTENTS_MAP_PRIVATEAIRPLANEMGR_H

#include <set>
#include <string>

struct lua_State;

class CPrivateAirplaneMgr {
public:
  static bool Create();
  static void Destroy();
  bool Init();
  static int L_InsertNeedItem(lua_State* state);
  static int L_InsertEndableMap(lua_State* state);
  static int L_InsertStartableMap(lua_State* state);

  /* 0004 */ bool m_initialized;
  /* 0008 */ std::set<std::string> m_startableMaps;
  /* 0010 */ std::set<std::string> m_endableMaps;
  /* 0018 */ std::set<int> m_needItems;

  // OFFSET: 0x006D00A0
  // FUNCTION: fn_46431
  CPrivateAirplaneMgr() {
    this->m_initialized = true;
  }
  virtual ~CPrivateAirplaneMgr();

  int GetNeedItem(int index);
  void InsertNeedItem(int item);
  bool IsNeedItem(int item);
  bool IsEndableMap(std::string map);
  bool IsStartableMap(std::string map);
  // OFFSET: 0x006D0800
  // FUNCTION: fn_46435
  void InsertStartableMap(std::string map) {
    this->m_startableMaps.insert(map);
  }
  // OFFSET: 0x006D06C0
  // FUNCTION: fn_46433
  void InsertEndableMap(std::string map) {
    this->m_endableMaps.insert(map);
  }
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CPrivateAirplaneMgr) == 0x20);
#endif

extern CPrivateAirplaneMgr* g_privateAirplaneItems;

#endif
