#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMNAMING_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMNAMING_H

#include "Contents/ItemDescription.h"

#include <string>
#include <windows.h>

class CPlayerNameDictionary;
struct ItemInfo;

struct ItemCardName {
  const char* name;
  const char* prefix;
  const char* suffix;
};

// Borrowed original descriptor/card tables, creator dictionary and locale.
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

#endif
