#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMREFORMMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMREFORMMGR_H

#include "Script/LuaInstance.h"

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct ItemReformMaterial {
  /* 0000 */ int count;
  /* 0004 */ int unknown_0004;
};

struct ItemReformInfo {
  ItemReformInfo();

  /* 0000 */ int baseItem;
  /* 0004 */ int resultItem;
  /* 0008 */ std::map<int, ItemReformMaterial> materials;
  /* 0010 */ std::int64_t needRefineMin;
  /* 0018 */ std::int64_t needRefineMax;
  /* 0020 */ std::int64_t needOptionNumMin;
  /* 0028 */ bool isEmptySocket;
  /* 0030 */ std::int64_t changeRefineValue;
  /* 0038 */ std::string randomOptionCode;
  /* 0050 */ bool preserveSocketItem;
  /* 0051 */ bool preserveGrade;
  /* 0054 */ std::string informationString;
};

class CItemReformMgr {
public:
  CItemReformMgr();
  virtual ~CItemReformMgr();

  void NullSub();
  std::vector<int> GetReformBaseItems(int itemId);
  ItemReformInfo* FindReform(int itemId, int baseItem);
  bool Load();
  bool HasItem(int itemId);
  void LoadReformInfo();
  void LoadReformItemList();

  /* 0004 */ std::map<std::uint64_t, ItemReformInfo> m_reformInfo;
  /* 000C */ std::map<int, std::vector<std::pair<std::uint32_t, std::uint32_t>>> m_reformItemList;
  /* 0014 */ CLuaInstance m_lua;
  /* 001C */ unsigned int m_maxMaterials;
};

extern CItemReformMgr* g_itemReformMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ItemReformMaterial) == 0x8);
static_assert(sizeof(ItemReformInfo) == 0x70);
static_assert(sizeof(CItemReformMgr) == 0x20);
#endif

#endif
