#include "Contents/ItemInfo.h"

#include <cstdlib>
#include <cstring>

// OFFSET: 0x006D47E0
// FUNCTION: fn_06562
ItemInfo::ItemInfo() {
  this->Reset();
}
// OFFSET: 0x006D4750
// FUNCTION: fn_06561
ItemInfo::ItemInfo(int) {
  this->Reset();
}

// OFFSET: 0x005D8F90
// FUNCTION: fn_03753
ItemInfo::ItemInfo(const ItemInfo& other) : itemId(other.itemId), unknown_0044(other.unknown_0044) {
  std::memcpy(this->unknown_0000, other.unknown_0000, sizeof(this->unknown_0000));
  this->quantity = other.quantity;
  std::memcpy(this->unknown_0014, other.unknown_0014, sizeof(this->unknown_0014));
  std::memcpy(this->unknown_001C, other.unknown_001C, sizeof(this->unknown_001C));
  this->identified = other.identified;
  this->unknown_005D = other.unknown_005D;
  this->unknown_0060 = other.unknown_0060;
  this->unknown_0064 = other.unknown_0064;
  this->unknown_0066 = other.unknown_0066;
  this->unknown_0068 = other.unknown_0068;
  this->unknown_006C = other.unknown_006C;
  this->unknown_0070 = other.unknown_0070;
  std::memcpy(this->unknown_0074, other.unknown_0074, sizeof(this->unknown_0074));
  std::memcpy(this->unknown_0078, other.unknown_0078, sizeof(this->unknown_0078));
  this->unknown_0088 = other.unknown_0088;
  this->unknown_0090 = other.unknown_0090;
  this->unknown_0098 = other.unknown_0098;
  std::memcpy(this->unknown_009C, other.unknown_009C, sizeof(this->unknown_009C));
  std::memcpy(this->unknown_00B8, other.unknown_00B8, sizeof(this->unknown_00B8));
  this->unknown_00C0 = other.unknown_00C0;
  std::memcpy(this->unknown_00C1, other.unknown_00C1, sizeof(this->unknown_00C1));
  std::memcpy(this->unknown_00D9, other.unknown_00D9, sizeof(this->unknown_00D9));
}

// OFFSET: 0x006D52B0
// FUNCTION: fn_06569
ItemInfo& ItemInfo::operator=(const ItemInfo& other) {
  if (this == &other) {
    return *this;
  }

  std::memcpy(this->unknown_0000, other.unknown_0000, sizeof(this->unknown_0000));
  this->quantity = other.quantity;
  std::memcpy(this->unknown_0014, other.unknown_0014, sizeof(this->unknown_0014));
  std::memcpy(this->unknown_001C, other.unknown_001C, sizeof(this->unknown_001C));

  this->itemId = other.itemId;
  this->unknown_0044 = other.unknown_0044;

  this->identified = other.identified;
  this->unknown_005D = other.unknown_005D;
  this->unknown_0060 = other.unknown_0060;
  this->unknown_0064 = other.unknown_0064;
  this->unknown_0066 = other.unknown_0066;
  this->unknown_0068 = other.unknown_0068;
  this->unknown_006C = other.unknown_006C;
  this->unknown_0070 = other.unknown_0070;
  std::memcpy(this->unknown_0074, other.unknown_0074, sizeof(this->unknown_0074));
  std::memcpy(this->unknown_0078, other.unknown_0078, sizeof(this->unknown_0078));
  this->unknown_0088 = other.unknown_0088;
  this->unknown_0090 = other.unknown_0090;
  this->unknown_0098 = other.unknown_0098;
  std::memcpy(this->unknown_009C, other.unknown_009C, sizeof(this->unknown_009C));
  // Unlike copy construction, assignment leaves +0xB8 onward untouched.
  return *this;
}

// OFFSET: 0x006D5550
// FUNCTION: fn_06573
std::uint32_t ItemInfo::Assign(const RawItemData& item) {
  this->unknown_0000[0] = item.unknown_0018;
  this->unknown_0000[1] = item.unknown_0004;
  this->quantity = item.quantity;
  this->unknown_0014[0] = item.unknown_0008;
  this->unknown_0014[1] = item.unknown_000C;

  std::memcpy(this->unknown_001C, item.cards, sizeof(item.cards));

  if (&this->itemId != &item.itemId) {
    this->itemId.assign(item.itemId.data(), item.itemId.size());
  }

  this->identified = item.identified;
  this->unknown_0060 = item.unknown_001C;

  int count = 0;
  if (item.optionCount <= 5) {
    count = item.optionCount;
  }
  this->unknown_0098 = static_cast<std::uint32_t>(count);

  if (count > 0) {
    std::memcpy(this->unknown_009C, item.options, static_cast<std::size_t>(count) * 5);
  }

  this->unknown_0088 = item.unknown_00AC;
  this->unknown_0074[3] = item.unknown_00AE;
  this->unknown_0000[2] = item.unknown_00B0;
  this->unknown_0070 = item.unknown_00B8;

  return item.unknown_00B8;
}

// OFFSET: 0x006D8A90
// FUNCTION: fn_06606
void ItemInfo::Reset() {
  std::memset(this->unknown_0000, 0, sizeof(this->unknown_0000));
  this->quantity = 0;
  std::memset(this->unknown_0014, 0, sizeof(this->unknown_0014));
  std::memset(this->unknown_001C, 0, sizeof(this->unknown_001C));
  this->itemId.clear();
  this->unknown_0044.clear();
  this->identified = this->unknown_005D = 0;
  this->unknown_0060 = 0;
  this->unknown_0064 = 0;
  this->unknown_0066 = 0;
  this->unknown_0068 = 0;
  this->unknown_006C = 0;
  this->unknown_0070 = 0;
  std::memset(this->unknown_0074, 0, sizeof(this->unknown_0074));
  std::memset(this->unknown_0078, 0, sizeof(this->unknown_0078));
  this->unknown_0088 = 0;
  this->unknown_0090 = 0;
  this->unknown_0098 = 0;
  std::memset(this->unknown_009C, 0, sizeof(this->unknown_009C));
  // Original leaves+0xB8 onward untouched, as well as ABI padding gaps.
}

// OFFSET: 0x00DDD4E0
// FUNCTION: fn_23386
ItemInfo LookupClientItem(const std::list<ItemInfo>& values, int id, unsigned char requireIdentified) {
  for (const ItemInfo& value : values) {
    if (std::atoi(value.itemId.c_str()) == id && (!requireIdentified || value.identified != 0)) {
      return value;
    }
  }
  return ItemInfo(0);
}

// OFFSET: 0x00DDD5A0
// FUNCTION: fn_23387
ItemInfo LookupClientItemByIdentity(const std::list<ItemInfo>& values, std::uint32_t id) {
  for (const ItemInfo& value : values) {
    if (value.unknown_0000[1] == id) {
      return value;
    }
  }
  return ItemInfo(0);
}
