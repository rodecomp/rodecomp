#ifndef RAGNAROK_SOURCE_CONTENTS_MONSTER_CHAMPIONMONSTERMGR_H
#define RAGNAROK_SOURCE_CONTENTS_MONSTER_CHAMPIONMONSTERMGR_H

#include <map>
#include <vector>

struct lua_State;
class CGameActor;

// VTABLE: vt_01613
class CChampionMonsterMgr {
public:
  // VTABLE: vt_01612
  class lcData {
  public:
    // INLINED
    lcData() {
    }
    // INLINED
    __forceinline lcData(const lcData& other) : m_value(other.m_value), m_effects(other.m_effects) {
    }
    virtual ~lcData();

    /* 0004 */ float m_value;
    /* 0008 */ std::vector<int> m_effects;
  };

  CChampionMonsterMgr();
  virtual ~CChampionMonsterMgr();

  static bool Create();
  static void Destroy();
  static int LuaAddItem(lua_State* state);
  static int LuaAddEffect(lua_State* state);

  bool Load();
  int ApplyEffects(int id, CGameActor* actor, int aid);
  void AddItem(int id, float value);
  void AddEffect(int id, int effect);
  float GetValue(int id);
  bool IsChampion(int id);

  /* 0004 */ bool m_isValid;
  /* 0008 */ std::map<int, lcData> m_items;
};

extern CChampionMonsterMgr* g_championMonsterMgr;

#endif
