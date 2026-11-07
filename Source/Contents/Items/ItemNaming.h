#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMNAMING_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMNAMING_H

#include "Contents/Items/ItemDescription.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <windows.h>

struct ITEM_INFO;

struct ItemCardName {
  /* 0000 */ const char* name;
  /* 0004 */ const char* prefix;
  /* 0008 */ const char* suffix;
};

struct ItemMoveInfo {
  /* 0000 */ int itemId;
  /* 0004 */ bool drop;
  /* 0005 */ bool trade;
  /* 0006 */ bool storage;
  /* 0007 */ bool cart;
  /* 0008 */ bool sellToNpc;
  /* 0009 */ bool mail;
  /* 000A */ bool auction;
  /* 000B */ bool guildStorage;
};

extern std::map<int, ItemCardName> g_itemCardNames;
extern std::map<int, ItemMoveInfo> g_itemMoveInfo;

ItemCardName& FindItemCardName(int id, std::map<int, ItemCardName>& values);
ItemCardName& FindItemCardName(int id);
ItemMoveInfo& FindItemMoveInfo(int id, std::map<int, ItemMoveInfo>& values);
ItemMoveInfo& FindItemMoveInfo(int id);
bool IsItemCard(int id);
bool Equipment(const ITEM_INFO& item);
std::uint32_t __fastcall ItemCardColor(const ITEM_INFO* item);
std::string ItemDisplayName(int id);
const char* ItemResourcePath(const ITEM_INFO& item);
int ItemSlotsCount(const ITEM_INFO& item);
int ItemUnidentifiedName(const ITEM_INFO& item, char* output, int& capacity, unsigned char consume);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ItemCardName) == 12);
static_assert(offsetof(ItemCardName, name) == 0x0000);
static_assert(offsetof(ItemCardName, prefix) == 0x0004);
static_assert(offsetof(ItemCardName, suffix) == 0x0008);
static_assert(sizeof(ItemMoveInfo) == 12);
static_assert(offsetof(ItemMoveInfo, drop) == 0x0004);
static_assert(offsetof(ItemMoveInfo, guildStorage) == 0x000B);
#endif

void LoadItemNameTables();

#endif
