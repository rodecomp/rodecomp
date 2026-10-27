#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMNAMING_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMNAMING_H

#include "Contents/Items/ItemDescription.h"

#include <cstddef>
#include <string>
#include <windows.h>

class CPlayerNameDictionary;
struct ItemInfo;

struct ItemCardName {
  /* 0000 */ const char* name;
  /* 0004 */ const char* prefix;
  /* 0008 */ const char* suffix;
};

struct ItemNaming {
  ItemDescriptions& descriptions;
  const std::map<int, ItemCardName>& cards;
  CPlayerNameDictionary& players;
  const bool& languagePrefix;
  const int& service;
  const WORD& language;

  std::string DisplayName(const ItemInfo& item, unsigned char detailed);
  bool HasCards(const ItemInfo& item) const;
  int Grade(const ItemInfo& item, char* output, int& capacity, unsigned char consume) const;
  int Creator(const ItemInfo& item, char* output, int& capacity, unsigned char consume);
  int Prefixes(const ItemInfo& item, char* output, int& capacity, unsigned char consume) const;
  int Suffixes(const ItemInfo& item, char* output, int& capacity, unsigned char consume) const;
  int Slots(const ItemInfo& item, char* output, int& capacity, unsigned char consume);
  const char* Message(int id) const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ItemCardName) == 12);
static_assert(offsetof(ItemCardName, name) == 0x0000);
static_assert(offsetof(ItemCardName, prefix) == 0x0004);
static_assert(offsetof(ItemCardName, suffix) == 0x0008);
#endif

#endif
