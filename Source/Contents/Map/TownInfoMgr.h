#ifndef RAGNAROK_SOURCE_CONTENTS_MAP_TOWNINFOMGR_H
#define RAGNAROK_SOURCE_CONTENTS_MAP_TOWNINFOMGR_H

#include <map>
#include <string>
#include <vector>

struct lua_State;

enum TownInfoType : int {
  TOWN_INFO_STORE = 0,
  TOWN_INFO_WEAPON_SHOP = 1,
  TOWN_INFO_ARMOR_SHOP = 2,
  TOWN_INFO_SMITHY = 3,
  TOWN_INFO_GUIDE = 4,
  TOWN_INFO_INN = 5,
  TOWN_INFO_KAFRA = 6,
  TOWN_INFO_STYLE = 7,
  TOWN_INFO_WARP = 8,
  TOWN_INFO_QUEST = 9,
};

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
  // OFFSET: 0x00AE1870
  // FUNCTION: fn_47493
  CTownInfoMgr() {
    this->m_isValid = true;
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
