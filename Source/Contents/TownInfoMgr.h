#ifndef RAGNAROK_SOURCE_CONTENTS_TOWNINFOMGR_H
#define RAGNAROK_SOURCE_CONTENTS_TOWNINFOMGR_H

#include <map>
#include <string>
#include <vector>

struct lua_State;

struct TownInfo {
  TownInfo();
  TownInfo(int type, std::string name, int x, int y, std::string image);

  /* 0000 */ int type;
  /* 0004 */ std::string name;
  /* 001C */ int x;
  /* 0020 */ int y;
  /* 0024 */ std::string image;
};

// VTABLE: vt_01617
class CTownInfoMgr {
public:
  // INLINED
  CTownInfoMgr() : m_isValid(true) {
  }
  // INLINED
  virtual ~CTownInfoMgr() {
  }

  static bool Create();
  static void Destroy();
  static int L_AddTownInfo(lua_State* state);

  /* 0004 */ bool m_isValid;
  /* 0008 */ std::map<std::string, std::vector<TownInfo>> m_towns;
  /* 0010 */ std::vector<TownInfo> m_current;

  void AddTownInfo(std::string mapName, std::string name, int x, int y, int type);
  bool LoadScript();
  TownInfo GetTownInfo(std::string mapName, unsigned int index);
};

extern CTownInfoMgr* g_townInfoMgr;

#endif
