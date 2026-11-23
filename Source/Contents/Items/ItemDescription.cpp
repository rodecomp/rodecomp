#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Client/Session/Session.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

// GLOBAL: gl_24500
std::map<int, ITEM_DESC> g_itemDescriptions;

// OFFSET: 0x006D3540
// FUNCTION: fn_06540
ITEM_DESC::ITEM_DESC()
    : identifiedName(nullptr), identifiedResource(nullptr), unidentifiedName(nullptr), unidentifiedResource(nullptr), descriptionFlags(0), slots(0),
      viewId(0) {
}

#pragma inline_depth(0)
// OFFSET: 0x006D46A0
// FUNCTION: fn_06560
ITEM_DESC::ITEM_DESC(const ITEM_DESC&) = default;
#pragma inline_depth()

// OFFSET: 0x006D5160
// FUNCTION: fn_06568
ITEM_DESC& ITEM_DESC::operator=(const ITEM_DESC& other) = default;

// OFFSET: 0x006D3A00
// FUNCTION: fn_06545
ITEM_DESC& FindItemDescription(int id, std::map<int, ITEM_DESC>& values) {
  // GLOBAL: gl_24674
  static ITEM_DESC missing;
  const auto entry = values.find(id);
  return entry == values.end() ? missing : entry->second;
}

// OFFSET: 0x006D3C10
// FUNCTION: fn_06547
std::vector<const char*>& EnsureItemDescriptionLines(int id, std::map<int, std::vector<const char*>>& values) {
  const auto entry = values.find(id);
  if (entry != values.end()) {
    return entry->second;
  }

  const auto inserted = values.insert(std::pair<int, std::vector<const char*>>(id, std::vector<const char*>()));
  return inserted.first->second;
}

// OFFSET: 0x006D5890
// FUNCTION: fn_06579
__declspec(noinline) ITEM_DESC& FindItemDescription(int id) {
  return FindItemDescription(id, g_itemDescriptions);
}

// OFFSET: 0x006D3DC0
// FUNCTION: fn_06548
ITEM_DESC& EnsureItemDescription(int id, std::map<int, ITEM_DESC>& values) {
  const auto entry = values.find(id);
  if (entry != values.end()) {
    return entry->second;
  }

  const auto inserted = values.insert({id, ITEM_DESC()});
  return inserted.first->second;
}

// OFFSET: 0x00DDD280
// FUNCTION: fn_23382
void CSession::IconPath(const char* id, char* output, int identified) {
  const ITEM_DESC& value = FindItemDescription(std::atoi(id));
  const char* resource = identified != 0 ? value.identifiedResource : value.unidentifiedResource;
  std::sprintf(output, "유저인터페이스\\item\\%s.bmp", resource);
}

// OFFSET: 0x006D5730
// FUNCTION: fn_06576
const std::vector<const char*>& ITEM_INFO::Description() const {
  if (this->identified == 1) {
    const int id = std::atoi(this->itemId.c_str());
    return FindItemDescription(id, g_itemDescriptions).identifiedDescription;
  }

  const int id = std::atoi(this->itemId.c_str());
  return FindItemDescription(id, g_itemDescriptions).unidentifiedDescription;
}

namespace {
  // INLINED
  __forceinline int CopyText(char* output, int& capacity, unsigned char consume, const char* text) {
    const int error = strncpy_s(output, capacity, text, _TRUNCATE);
    if (error != 0 && error != 80) {
      return 0;
    }

    const int written = !output ? 0 : static_cast<int>(strnlen(output, capacity));
    if (consume) {
      capacity -= written;
    }
    return written;
  }
}

// OFFSET: 0x006D5810
// FUNCTION: fn_06578
int ITEM_INFO::GetIdDisplayName(char* output, int& capacity, unsigned char consume) const {
  const int id = std::atoi(this->itemId.c_str());
  const ITEM_DESC& desc = FindItemDescription(id, g_itemDescriptions);
  if (desc.identifiedName) {
    return CopyText(output, capacity, consume, desc.identifiedName);
  }
  return CopyText(output, capacity, consume, " ");
}
