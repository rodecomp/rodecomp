#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_SELECTPACKAGEITEMMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_SELECTPACKAGEITEMMGR_H

#include "Contents/Items/ItemInfo.h"
#include "Utility/Singleton.h"

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <vector>

struct lua_State;

extern short g_itemOptionPlaceholderId;

struct SelectPackageTab {
  /* 0000 */ std::string name;
  /* 0018 */ std::vector<ITEM_INFO> items;
};

class CSelectPackageItemMgr : public Singleton<CSelectPackageItemMgr> {
public:
  static int LuaAddTab(lua_State* L);
  static int LuaAddItem(lua_State* L);
  static bool LoadScript();

  /* 0004 */ std::map<int, std::map<int, SelectPackageTab>> m_packages;

  CSelectPackageItemMgr();

  void AddTab(int packageId, int tabIndex, const std::string& name, int itemCount);
  std::optional<std::vector<ITEM_INFO>*> FindItems(int packageId, int tabIndex);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SelectPackageTab) == 0x24);
static_assert(offsetof(SelectPackageTab, items) == 0x18);
static_assert(sizeof(CSelectPackageItemMgr) == 0xC);
#endif

#endif
