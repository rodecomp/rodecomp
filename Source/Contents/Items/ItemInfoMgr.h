#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMINFOMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMINFOMGR_H

#include <map>
#include <string>
#include <vector>

class ITEM_DESC;
struct lua_State;

// VTABLE: vt_01615
class CItemInfoMgr {
public:
  // VTABLE: vt_01614
  class lcData {
  public:
    // INLINED
    lcData() : slotCount(0), classNum(0), isCostume(0), effectId(0), packageId(0) {
    }
    // INLINED
    virtual ~lcData() {
    }

    /* 0004 */ std::string unidentifiedDisplayName;
    /* 001C */ std::string unidentifiedResourceName;
    /* 0034 */ std::vector<std::string> unidentifiedDescription;
    /* 0040 */ std::string identifiedDisplayName;
    /* 0058 */ std::string identifiedResourceName;
    /* 0070 */ std::vector<std::string> identifiedDescription;
    /* 007C */ int slotCount;
    /* 0080 */ int classNum;
    /* 0084 */ int isCostume;
    /* 0088 */ int effectId;
    /* 008C */ int packageId;
  };

  // INLINED
  CItemInfoMgr() : m_isValid(true) {
  }
  // INLINED
  virtual ~CItemInfoMgr() {
  }

  static bool Create();
  static void Destroy();
  static const char* GetFileName();
  static int LuaAddItem(lua_State* state);
  static int LuaAddItemEffectInfo(lua_State* state);
  static int LuaAddItemIdentifiedDesc(lua_State* state);
  static int LuaAddItemIsCostume(lua_State* state);
  static int LuaAddItemPackageID(lua_State* state);
  static int LuaAddItemUnidentifiedDesc(lua_State* state);

  // INLINED
  void SetUnidentifiedName(int id, std::string displayName, std::string resourceName) {
    this->m_items[id].unidentifiedDisplayName = displayName;
    this->m_items[id].unidentifiedResourceName = resourceName;
  }
  // INLINED
  void SetIdentifiedName(int id, std::string displayName, std::string resourceName) {
    this->m_items[id].identifiedDisplayName = displayName;
    this->m_items[id].identifiedResourceName = resourceName;
  }

  bool Load();
  int IsCostume(int id);
  int GetEffectId(int id);
  int GetPackageId(int id);
  void SetItemInfo(std::map<int, ITEM_DESC>& descriptions) const;

  /* 0004 */ bool m_isValid;
  /* 0008 */ std::map<int, lcData> m_items;
};

extern CItemInfoMgr* g_itemInfoMgr;

#endif
