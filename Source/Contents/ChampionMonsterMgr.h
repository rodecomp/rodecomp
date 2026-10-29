#ifndef RAGNAROK_SOURCE_CONTENTS_CHAMPIONMONSTERMGR_H
#define RAGNAROK_SOURCE_CONTENTS_CHAMPIONMONSTERMGR_H

#include <map>
#include <vector>

struct lua_State;

// VTABLE: vt_01613
class CChampionMonsterMgr {
public:
  // VTABLE: vt_01612
  class lcData {
  public:
    // INLINED
    explicit lcData(float value) : m_value(value) {
    }
    // INLINED
    virtual ~lcData() {
    }

    /* 0004 */ float m_value;
    /* 0008 */ std::vector<int> m_effects;
  };

  // INLINED
  CChampionMonsterMgr() : m_isValid(true) {
  }
  // INLINED
  virtual ~CChampionMonsterMgr() {
  }

  static bool Create();
  static void Destroy();
  static int LuaAddItem(lua_State* state);
  static int LuaAddEffect(lua_State* state);

  void AddItem(int id, float value);
  // INLINED
  void AddEffect(int id, int effect) {
    if (!this->m_items.empty()) {
      std::map<int, lcData>::iterator it = this->m_items.find(id);
      if (it != this->m_items.end()) {
        it->second.m_effects.push_back(effect);
      }
    }
  }
  float GetValue(int id);
  bool IsChampion(int id);

  /* 0004 */ bool m_isValid;
  /* 0008 */ std::map<int, lcData> m_items;
};

extern CChampionMonsterMgr* g_championMonsterMgr;

#endif
