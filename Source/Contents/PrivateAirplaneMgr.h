#ifndef RAGNAROK_SOURCE_CONTENTS_PRIVATEAIRPLANEMGR_H
#define RAGNAROK_SOURCE_CONTENTS_PRIVATEAIRPLANEMGR_H

#include <set>
#include <string>

struct lua_State;

class CPrivateAirplaneMgr {
public:
  static bool Create();
  static void Destroy();
  static bool Init();
  static int L_InsertNeedItem(lua_State* state);
  static int L_InsertEndableMap(lua_State* state);
  static int L_InsertStartableMap(lua_State* state);

  /* 0004 */ bool m_initialized;
  /* 0008 */ std::set<std::string> m_startableMaps;
  /* 0010 */ std::set<std::string> m_endableMaps;
  /* 0018 */ std::set<int> m_needItems;

  CPrivateAirplaneMgr() : m_initialized(true) {}
  virtual ~CPrivateAirplaneMgr() = default;

  int GetNeedItem(int index);
  bool IsEndableMap(std::string map);
  bool IsStartableMap(std::string map);
  // INLINED
  void InsertStartableMap(std::string map) {
    this->m_startableMaps.insert(map);
  }
  // INLINED
  void InsertEndableMap(std::string map) {
    this->m_endableMaps.insert(map);
  }
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CPrivateAirplaneMgr) == 0x20);
#endif

extern CPrivateAirplaneMgr* g_privateAirplaneItems;

#endif
