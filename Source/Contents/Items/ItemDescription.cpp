#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

// GLOBAL: gl_24500
std::map<int, ITEM_DESC> g_itemDescriptions;

// OFFSET: 0x006D3540
// FUNCTION: fn_06540
ITEM_DESC::ITEM_DESC()
    : identifiedName(nullptr), identifiedResource(nullptr), unidentifiedName(nullptr), unidentifiedResource(nullptr), unknown_002C(0), slots(0),
      unknown_0040(0) {
}

// OFFSET: 0x006D3610
// FUNCTION: fn_06541
ITEM_DESC::~ITEM_DESC() = default;

// OFFSET: 0x006D5160
// FUNCTION: fn_06568
ITEM_DESC& ITEM_DESC::operator=(const ITEM_DESC& other) {
  if (this == &other) {
    return *this;
  }

  this->identifiedName = other.identifiedName;
  this->identifiedResource = other.identifiedResource;
  this->identifiedDescription = other.identifiedDescription;
  this->unidentifiedName = other.unidentifiedName;
  this->unidentifiedResource = other.unidentifiedResource;
  this->unidentifiedDescription = other.unidentifiedDescription;
  this->unknown_002C = other.unknown_002C;
  this->slots = other.slots;
  this->unknown_0034 = other.unknown_0034;
  this->unknown_0040 = other.unknown_0040;

  return *this;
}

// OFFSET: 0x006D3A00
// FUNCTION: fn_06545
ITEM_DESC& ItemDescriptions::Find(int id) {
  static ITEM_DESC missing;
  const auto entry = this->values.find(id);
  return entry == this->values.end() ? missing : entry->second;
}

// FUNCTION: fn_06546
ItemCardName& EnsureItemCardName(int id, std::map<int, ItemCardName>& values) {
  const auto entry = values.find(id);
  if (entry != values.end()) {
    return entry->second;
  }

  return values[id];
}

// FUNCTION: fn_06547
std::vector<const char*>& EnsureItemDescriptionLines(int id, std::map<int, std::vector<const char*>>& values) {
  const auto entry = values.find(id);
  if (entry != values.end()) {
    return entry->second;
  }

  return values[id];
}

// OFFSET: 0x006D5890
// FUNCTION: fn_06579
ITEM_DESC& FindItemDescription(int id) {
  ItemDescriptions descriptions{g_itemDescriptions};
  return descriptions.Find(id);
}

// OFFSET: 0x006D3DC0
// FUNCTION: fn_06548
ITEM_DESC& ItemDescriptions::Ensure(int id) {
  return this->values[id];
}

// OFFSET: 0x00DDD280
// FUNCTION: fn_23382
void ItemDescriptions::IconPath(char* output, const char* id, int identified) {
  const ITEM_DESC& value = this->Find(std::atoi(id));
  const char* resource = identified != 0 ? value.identifiedResource : value.unidentifiedResource;
  std::sprintf(output, "\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA\\item\\%s.bmp", resource);
}

// OFFSET: 0x006D5730
// FUNCTION: fn_06576
const std::vector<const char*>& ItemDescriptions::Description(const ItemInfo& item) {
  const ITEM_DESC& value = this->Find(std::atoi(item.itemId.c_str()));
  return item.identified == 1 ? value.identifiedDescription : value.unidentifiedDescription;
}

// OFFSET: 0x006D5810
// FUNCTION: fn_06578
int ItemDescriptions::Name(const ItemInfo& item, char* output, int& capacity, unsigned char consume) {
  const char* name = this->Find(std::atoi(item.itemId.c_str())).identifiedName;
  const int error = strncpy_s(output, capacity, name ? name : " ", _TRUNCATE);

  if (error != 0 && error != 80) {
    return 0;
  }

  const int length = output ? static_cast<int>(strnlen(output, capacity)) : 0;
  if (consume != 0) {
    capacity -= length;
  }

  return length;
}
