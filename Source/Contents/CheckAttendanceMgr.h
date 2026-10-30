#ifndef RAGNAROK_SOURCE_CONTENTS_CHECKATTENDANCEMGR_H
#define RAGNAROK_SOURCE_CONTENTS_CHECKATTENDANCEMGR_H

#include <map>
#include <string>
#include <utility>

struct lua_State;

// VTABLE: vt_00354
class CCheckAttendanceMgr {
public:
  // VTABLE: vt_00353
  class lcReward {
  public:
    // INLINED
    lcReward(int item, int count) : m_item(item), m_count(count) {
    }
    // INLINED
    virtual ~lcReward() {
    }

    /* 0004 */ int m_item;
    /* 0008 */ int m_count;
  };

  // INLINED
  CCheckAttendanceMgr() : m_isValid(true), m_config0(0), m_config1(0) {
  }
  // INLINED
  virtual ~CCheckAttendanceMgr() {
  }

  static bool Create();
  static void Destroy();
  static int L_InsertCheckAttendanceConfig(lua_State* state);
  static int L_InsertCheckAttendanceReward(lua_State* state);

  std::pair<int, int> GetReward(int day);
  bool Load();
  bool LoadLocal(std::string suffix);

  /* 0004 */ bool m_isValid;
  /* 0008 */ int m_config0;
  /* 000C */ int m_config1;
  /* 0010 */ std::map<int, lcReward> m_rewards;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCheckAttendanceMgr) == 0x18);
#endif

extern CCheckAttendanceMgr* g_checkAttendanceMgr;

#endif
