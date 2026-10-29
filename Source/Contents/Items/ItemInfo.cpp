#include "Contents/Items/ItemInfo.h"
#include "Client/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"

#include <cstdlib>
#include <cstring>

namespace {
#pragma pack(push, 1)
  struct ItemRecordHeader {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::uint32_t bytes;
  };
#pragma pack(pop)
}

// GLOBAL: gl_24720
unsigned char g_digitCharacters[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// OFFSET: 0x006D47E0
// FUNCTION: fn_06562
ITEM_INFO::ITEM_INFO() {
  this->Reset();
}

// OFFSET: 0x006D4750
// FUNCTION: fn_06561
ITEM_INFO::ITEM_INFO(int) {
  this->Reset();
}

// OFFSET: 0x005D8F90
// FUNCTION: fn_03753
ITEM_INFO::ITEM_INFO(const ITEM_INFO& other) : itemId(other.itemId), unknown_0044(other.unknown_0044) {
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
ITEM_INFO& ITEM_INFO::operator=(const ITEM_INFO& other) {
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

  return *this;
}

// OFFSET: 0x006D5550
// FUNCTION: fn_06573
std::uint32_t ITEM_INFO::Assign(const RawItemData& item) {
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

  this->unknown_0098 = count;

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
void ITEM_INFO::Reset() {
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
}

// OFFSET: 0x00DDD4E0
// FUNCTION: fn_23386
ITEM_INFO LookupClientItem(const std::list<ITEM_INFO>& values, int id, unsigned char requireIdentified) {
  for (const ITEM_INFO& value : values) {
    if (std::atoi(value.itemId.c_str()) == id && (!requireIdentified || value.identified != 0)) {
      return value;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDD5A0
// FUNCTION: fn_23387
ITEM_INFO LookupClientItemByIdentity(const std::list<ITEM_INFO>& values, std::uint32_t id) {
  for (const ITEM_INFO& value : values) {
    if (value.unknown_0000[1] == id) {
      return value;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x006D5450
// FUNCTION: fn_06570
ITEM_INFO& ITEM_INFO::AssignBasic(const RawItemBasic& item) {
  this->unknown_0000[1] = item.id;
  this->unknown_0014[0] = item.unknown_0004;
  this->unknown_0014[1] = item.unknown_0008;
  this->unknown_0000[0] = item.unknown_000C;
  this->identified = item.identified;
  this->itemId = item.itemId;
  this->unknown_0070 = item.unknown_0038;
  this->unknown_0000[2] = item.unknown_0034;
  this->unknown_0074[3] = item.id != -1;
  return *this;
}

// OFFSET: 0x006D54C0
// FUNCTION: fn_06571
bool ITEM_INFO::operator<(const ITEM_INFO& other) const {
  return this->unknown_0044 < other.unknown_0044;
}

// OFFSET: 0x006D8B60
// FUNCTION: fn_06607
std::list<int> SearchItemIds(std::string query, unsigned char cardsOnly, unsigned char partial) {
  std::list<int> result;
  for (const auto& entry : g_itemDescriptions) {
    if (cardsOnly == 1 && g_itemCardNames.find(entry.first) == g_itemCardNames.end()) {
      continue;
    }

    const char* name = entry.second.identifiedName;
    if (name == nullptr || *name == '\0') {
      continue;
    }

    if (MatchItemName(name, query, partial, &result, entry.first)) {
      continue;
    }

    if (g_itemCardNames.find(entry.first) == g_itemCardNames.end()) {
      continue;
    }

    if (FindItemCardName(entry.first, g_itemCardNames).prefix != nullptr &&
        MatchItemName(FindItemCardName(entry.first, g_itemCardNames).prefix, query, partial, &result, entry.first)) {
      continue;
    }

    if (FindItemCardName(entry.first, g_itemCardNames).suffix != nullptr) {
      MatchItemName(FindItemCardName(entry.first, g_itemCardNames).suffix, query, partial, &result, entry.first);
    }
  }

  return result;
}

// OFFSET: 0x006D8E60
// FUNCTION: fn_06608
bool MatchItemName(std::string name, std::string query, unsigned char partial, std::list<int>* result, int id) {
  if ((partial == 1 && name.find(query) != std::string::npos) || name == query) {
    result->push_back(id);
    return true;
  }

  return false;
}

// OFFSET: 0x006D8FE0
// FUNCTION: fn_06609
bool ITEM_INFO::SetName(const char* name) {
  this->unknown_0044 = name;
  return this->unknown_0044.empty();
}

// OFFSET: 0x006D9010
// FUNCTION: fn_06610
void ITEM_INFO::SetItemId(int value) {
  char buffer[16];
  _itoa(value, buffer, 10);
  this->itemId = buffer;
}

// OFFSET: 0x006D90C0
// FUNCTION: fn_06612
std::uint32_t ITEM_INFO::Deserialize(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  std::uint32_t offset = 6;
  ItemRecordHeader header{};
  std::memcpy(&header, data.data(), sizeof(header));
  if (header.id != 281) {
    return 0xFFFFFFFF;
  }

  while (offset < data.size()) {
    std::memcpy(&header, data.data() + offset, sizeof(header));
    offset += 6;
    const unsigned char* source = data.data() + offset;
    switch (header.id) {
    case 282:
      return offset;
    case 283:
      std::memcpy(&this->unknown_0000[0], source, header.bytes);
      offset += header.bytes;
      break;
    case 284:
      std::memcpy(&this->unknown_0000[2], source, header.bytes);
      offset += header.bytes;
      break;
    case 285:
      std::memcpy(&this->unknown_0000[1], source, header.bytes);
      offset += header.bytes;
      break;
    case 286:
      std::memcpy(&this->unknown_0000[3], source, header.bytes);
      offset += header.bytes;
      break;
    case 287:
      std::memcpy(&this->quantity, source, header.bytes);
      offset += header.bytes;
      break;
    case 288:
      std::memcpy(&this->unknown_0014[0], source, header.bytes);
      offset += header.bytes;
      break;
    case 289:
      std::memcpy(&this->unknown_0014[1], source, header.bytes);
      offset += header.bytes;
      break;
    case 290:
      std::memcpy(this->unknown_001C, source, header.bytes);
      offset += header.bytes;
      break;
    case 291: {
      int id = 0;
      std::memcpy(&id, source, header.bytes);
      offset += header.bytes;
      char buffer[16] = {};
      this->itemId = _itoa(id, buffer, 10);
      break;
    }
    case 292: {
      char buffer[260];
      std::memset(buffer, 0, sizeof(buffer));
      std::memcpy(buffer, data.data() + offset, header.bytes);
      offset += header.bytes;
      this->unknown_0044 = buffer;
      break;
    }
    case 293:
      std::memcpy(&this->identified, source, header.bytes);
      offset += header.bytes;
      break;
    case 294:
      std::memcpy(&this->unknown_005D, source, header.bytes);
      offset += header.bytes;
      break;
    case 295:
      std::memcpy(&this->unknown_0060, source, header.bytes);
      offset += header.bytes;
      break;
    case 296:
      std::memcpy(&this->unknown_0064, source, header.bytes);
      offset += header.bytes;
      break;
    case 297:
      std::memcpy(&this->unknown_0068, source, header.bytes);
      offset += header.bytes;
      break;
    case 298:
      std::memcpy(&this->unknown_0070, source, header.bytes);
      offset += header.bytes;
      break;
    case 299:
      std::memcpy(&this->unknown_0088, source, header.bytes);
      offset += header.bytes;
      break;
    case 300:
      std::memcpy(&this->unknown_0098, source, header.bytes);
      offset += header.bytes;
      break;
    case 301:
      std::memcpy(this->unknown_009C, source, header.bytes);
      offset += header.bytes;
      break;
    default:
      offset += header.bytes;
      break;
    }
  }

  return offset;
}

// OFFSET: 0x00DFB9D0
// FUNCTION: fn_23584
bool CSession::HasRequiredItems() const {
  for (const ITEM_INFO& required : this->unknown_1730) {
    const ITEM_INFO owned = LookupClientItemByIdentity(this->m_inventoryItems, required.unknown_0000[1]);
    if (owned.unknown_0000[1] == 0 || owned.quantity < required.quantity) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00DFBD90
// FUNCTION: fn_23588
bool CSession::IsSlotItemIdentity(std::uint32_t id) const {
  if (id != 0 && id == this->unknown_16EC) {
    return true;
  }

  for (int index = 0; index < 10; ++index) {
    if (this->unknown_17F0[index].quantity == 1 && this->unknown_17F0[index].unknown_0000[1] == id) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DFBDE0
// FUNCTION: fn_23589
bool CSession::HasInventoryItem(std::uint32_t id) const {
  for (const ITEM_INFO& value : this->m_inventoryItems) {
    if (value.unknown_0000[1] == id) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DFBE40
// FUNCTION: fn_23591
bool CSession::HasItemId(int itemId) const {
  const ITEM_INFO owned = LookupClientItemByIdentity(this->m_inventoryItems, this->unknown_16EC);
  if (owned.unknown_0000[1] != 0 && std::atoi(owned.itemId.c_str()) == itemId) {
    return true;
  }

  for (int index = 0; index < 10; ++index) {
    if (std::atoi(this->unknown_17F0[index].itemId.c_str()) == itemId || std::atoi(this->unknown_2B50[index].itemId.c_str()) == itemId) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DFBF70
// FUNCTION: fn_23592
bool CSession::HasItemCount(int itemId, int count) const {
  const ITEM_INFO owned = LookupClientItemByIdentity(this->m_inventoryItems, this->unknown_16EC);
  if (owned.unknown_0000[1] != 0 && std::atoi(owned.itemId.c_str()) == itemId) {
    return true;
  }

  for (int index = 0; index < 10; ++index) {
    if (std::atoi(this->unknown_17F0[index].itemId.c_str()) == itemId && this->unknown_17F0[index].unknown_0060 >= count) {
      return true;
    }
    if (std::atoi(this->unknown_2B50[index].itemId.c_str()) == itemId && this->unknown_2B50[index].unknown_0060 >= count) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x006D88D0
// FUNCTION: fn_06600
bool ITEM_INFO::HasSpecialCard() const {
  std::uint32_t card;
  std::memcpy(&card, this->unknown_001C, sizeof(card));
  return card == 254 || card == 255;
}

// OFFSET: 0x006D89D0
// FUNCTION: fn_06603
bool ITEM_INFO::IsApprenticeItem() const {
  const unsigned id = static_cast<unsigned>(std::atoi(this->itemId.c_str()));
  return id - 9001u <= 498u || id - 10000u <= 999u;
}

// OFFSET: 0x006D8920
// FUNCTION: fn_06601
bool ITEM_INFO::IsForgeMaterial() const {
  return std::atoi(this->itemId.c_str()) == 1000 || this->IsElementalStone();
}

// OFFSET: 0x006D8980
// FUNCTION: fn_06602
bool IsNumericText(const char* text) {
  for (unsigned index = 0; index < std::strlen(text); ++index) {
    if (!g_digitCharacters[static_cast<unsigned char>(text[index])]) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x006D8A10
// FUNCTION: fn_06604
bool ITEM_INFO::IsElementalStone() const {
  switch (std::atoi(this->itemId.c_str())) {
  case 994:
  case 995:
  case 996:
  case 997:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x006D8A50
// FUNCTION: fn_06605
bool ITEM_INFO::IsAboveItemId600() const {
  return std::atoi(this->itemId.c_str()) > 500 && std::atoi(this->itemId.c_str()) > 600;
}

// OFFSET: 0x005FCCF0
// FUNCTION: fn_04106
int ITEM_INFO::GetItemId() const {
  return std::atoi(this->itemId.c_str());
}

// OFFSET: 0x006D9070
// FUNCTION: fn_06611
void ITEM_INFO::SetOption(ItemOption option, const int& index) {
  if (index < 5) {
    std::memcpy(this->unknown_009C + index * 5, &option.id, sizeof(option.id));
    this->unknown_009C[index * 5 + 4] = option.parameter;
    std::memcpy(this->unknown_009C + index * 5 + 2, &option.value, sizeof(option.value));
  }
}

// OFFSET: 0x006D9590
// FUNCTION: fn_06613
void ITEM_INFO::SetCard(const int& card, const int& index) {
  if (index < 4) {
    std::memcpy(this->unknown_001C + index * 4, &card, sizeof(card));
  }
}
