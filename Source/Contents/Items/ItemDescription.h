#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMDESCRIPTION_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMDESCRIPTION_H

#include <map>
#include <vector>

struct ItemCardName;
struct ItemInfo;

class ITEM_DESC {
public:
  ITEM_DESC();
  virtual ~ITEM_DESC();
  ITEM_DESC& operator=(const ITEM_DESC& other);

  /* 0004 */ const char* identifiedName;
  /* 0008 */ const char* identifiedResource;
  /* 000C */ std::vector<const char*> identifiedDescription;
  /* 0018 */ const char* unidentifiedName;
  /* 001C */ const char* unidentifiedResource;
  /* 0020 */ std::vector<const char*> unidentifiedDescription;
  /* 002C */ int unknown_002C;
  /* 0030 */ int slots;
  /* 0034 */ std::vector<int> unknown_0034;
  /* 0040 */ int unknown_0040;
};

struct ItemDescriptions {
  std::map<int, ITEM_DESC>& values;
  ITEM_DESC& Find(int id);
  ITEM_DESC& Ensure(int id);
  void IconPath(char* output, const char* id, int identified);
  const std::vector<const char*>& Description(const ItemInfo& item);
  int Name(const ItemInfo& item, char* output, int& capacity, unsigned char consume);
};

ITEM_DESC& FindItemDescription(int id);
ItemCardName& EnsureItemCardName(int id, std::map<int, ItemCardName>& values);
std::vector<const char*>& EnsureItemDescriptionLines(int id, std::map<int, std::vector<const char*>>& values);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ITEM_DESC) == 68);
#endif

#endif
