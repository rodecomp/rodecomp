#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMREFORMMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMREFORMMGR_H

#include "Script/LuaInstance.h"

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct ItemReformInfo {
  // OFFSET: 0x005F92A0
  // FUNCTION: fn_04074
  ItemReformInfo() {
  }

  /* 0000 */ int baseItem;
  /* 0004 */ int resultItem;
  /* 0008 */ std::map<int, std::uint64_t> materials;
  /* 0010 */ std::uint64_t needRefineMin;
  /* 0018 */ std::uint64_t needRefineMax;
  /* 0020 */ std::uint64_t needOptionNumMin;
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
  static CItemReformMgr* Instance();
  void NullSub();
  std::vector<int> GetReformBaseItems(int itemId);
  ItemReformInfo* FindReform(int itemId, int baseItem);
  bool Load();
  bool HasItem(int itemId);
  void LoadReformInfo();
  void LoadReformItemList();

  /* 0004 */ std::map<std::uint64_t, ItemReformInfo> m_reformInfo;
  /* 000C */ std::map<int, std::vector<std::uint64_t>> m_reformItemList;
  /* 0014 */ CLuaInstance m_lua;
  /* 001C */ unsigned int m_maxMaterials;
};

extern CItemReformMgr* g_itemReformMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ItemReformInfo) == 0x70);
static_assert(sizeof(CItemReformMgr) == 0x20);
#endif

#endif
