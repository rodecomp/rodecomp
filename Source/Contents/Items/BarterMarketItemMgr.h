#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_BARTERMARKETITEMMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_BARTERMARKETITEMMGR_H

#include "Contents/Items/ItemInfo.h"

#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct BarterMarketItem {
  /* 0000 */ std::string name;
  /* 0018 */ int value = 0;
  /* 001C */ int field_1c = 0;
  /* 0020 */ int field_20 = 0;
  /* 0024 */ bool flag_24 = true;
  /* 0028 */ int field_28 = 0;
  /* 002C */ int field_2c = 0;
  /* 0030 */ int amount = 0;
  /* 0034 */ int field_34 = 0;
  /* 0038 */ bool flag_38 = false;
  /* 003C */ int id = -1;
  /* 0040 */ int field_40 = 0;
  /* 0044 */ int field_44 = 0;

  BarterMarketItem() {}
  BarterMarketItem(const BarterMarketItem& other);
};

static_assert(sizeof(BarterMarketItem) == 0x48);

class CBarterMarketItemMgr {
public:
  CBarterMarketItemMgr();
  void RefreshTotals();
  void Clear();
  static bool CreateInstance();
  void AddItem(const BarterMarketItem& item);
  void RemoveItem(const BarterMarketItem& item);
  bool HasItemByName(std::string name);
  std::pair<bool, BarterMarketItem> FindItem(int id);
  std::pair<bool, BarterMarketItem> FindAllItem(int id);
  int GetAllItemValueByName(std::string name);
  int GetValueById(int id);
  int GetAmountById(int id);
  int GetAllItemValueById(int id);
  float GetTotalAmount();
  std::list<BarterMarketItem>* GetItems();
  std::list<BarterMarketItem>* GetAllItems();
  int GetAllItemCount();
  std::map<int, int>* GetTotals();
  static void DestroyInstance();

  virtual ~CBarterMarketItemMgr();

  /* 0004 */ bool initialized;
  /* 0008 */ std::list<BarterMarketItem> items;
  /* 0010 */ std::list<BarterMarketItem> allItems;
  /* 0018 */ std::map<int, int> totals;
};

static_assert(sizeof(CBarterMarketItemMgr) == 0x20);

struct BarterChunk16 {
  /* 0000 */ int field_0;
  /* 0004 */ int field_4;
  /* 0008 */ int amount;
  /* 000C */ int field_c;
};

struct BarterMarketEntry {
  /* 0000 */ int field_0 = 0;
  /* 0004 */ int value = 0;
  /* 0008 */ int field_8 = 0;
  /* 000C */ std::string name;
  /* 0024 */ bool flag_24 = true;
  /* 0028 */ int field_28 = 0;
  /* 002C */ float factor = 0.0f;
  /* 0030 */ bool flag_30 = false;
  /* 0034 */ int id = 0;
  /* 0038 */ int field_38 = 0;
  /* 003C */ int field_3c = 0;
  /* 0040 */ std::vector<BarterChunk16> chunks;

  BarterMarketEntry() {}

  // OFFSET: 0x005D8DA0
  // FUNCTION: fn_03752
  BarterMarketEntry(const BarterMarketEntry& other) = default;

  // OFFSET: 0x00845020
  // FUNCTION: fn_10571
  BarterMarketEntry& operator=(const BarterMarketEntry& other) = default;
};

static_assert(sizeof(BarterMarketEntry) == 0x4C);

class CExpandedBarterMarketItemMgr {
public:
  CExpandedBarterMarketItemMgr();
  static bool CreateInstance();
  void RemoveEntry(const BarterMarketEntry& entry);
  void AddEntry(const BarterMarketEntry& entry);
  bool HasEntryByName(std::string name);
  std::pair<bool, BarterMarketEntry> FindItem(int id);
  std::pair<bool, BarterMarketEntry> FindEntry(int id);
  void AddItem(const BarterMarketEntry& source);
  void Clear();
  std::vector<BarterMarketEntry>* GetItems();
  int GetItemValueById(int id);
  float GetItemFactorById(int id);
  std::vector<BarterMarketEntry>* GetEntries();
  int GetEntryCount();
  int GetValueById(int id);
  std::vector<BarterChunk16>* GetChunks();
  float GetWeightedTotal();
  static void DestroyInstance();

  virtual ~CExpandedBarterMarketItemMgr();

  /* 0008 */ bool initialized;
  /* 000C */ std::vector<BarterMarketEntry> items;
  /* 0018 */ std::vector<BarterMarketEntry> entries;
  /* 0024 */ std::vector<BarterChunk16> chunks;
  /* 0030 */ std::multimap<int, ITEM_INFO> itemInfos;
  /* 0038 */ __int64 total;
};

static_assert(sizeof(CExpandedBarterMarketItemMgr) == 0x40);

extern CBarterMarketItemMgr* g_barterMarketItemMgr;
extern CExpandedBarterMarketItemMgr* g_expandedBarterMarketItemMgr;

#endif
