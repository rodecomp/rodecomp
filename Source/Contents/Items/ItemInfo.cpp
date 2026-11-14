#include "UI/Window/UIEnums.h"
#include "Contents/Items/ItemInfo.h"
#include "Client/Mode/ModeDragState.h"
#include "Contents/Items/ItemId.h"
#include "Client/Session/Session.h"
#include "Client/Input/InputCharset.h"
#include "File/File.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdlib>
#include <cstring>
#include <list>
#include <vector>

template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

namespace {
#pragma pack(push, 1)
  struct ItemRecordHeader {
    // INLINED
    ItemRecordHeader() {
      this->Clear();
    }

    // INLINED
    ~ItemRecordHeader() {
      this->Clear();
    }

    // INLINED
    void Clear() {
      this->id = 0;
      this->bytes = 0;
    }

    // INLINED
    void Read(const unsigned char* data) {
      this->Clear();
      std::memcpy(this, data, sizeof(*this));
    }

    /* 0000 */ std::int16_t id;
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

template std::list<ITEM_INFO>::iterator std::list<ITEM_INFO>::erase(std::list<ITEM_INFO>::const_iterator) noexcept;

// OFFSET: 0x006D47E0
// FUNCTION: fn_06562
__declspec(noinline) ITEM_INFO::ITEM_INFO() {
  this->Reset();
}

// OFFSET: 0x006D4750
// FUNCTION: fn_06561
__declspec(noinline) ITEM_INFO::ITEM_INFO(int) {
  this->Reset();
}

// OFFSET: 0x005D8F90
// FUNCTION: fn_03753
ITEM_INFO::ITEM_INFO(const ITEM_INFO& other) : itemId(other.itemId), displayName(other.displayName) {
  std::memcpy(this->identity, other.identity, sizeof(this->identity));
  this->quantity = other.quantity;
  std::memcpy(this->prices, other.prices, sizeof(this->prices));
  std::memcpy(this->cards, other.cards, sizeof(this->cards));
  this->identified = other.identified;
  this->damaged = other.damaged;
  this->refiningLevel = other.refiningLevel;
  this->bindOnEquip = other.bindOnEquip;
  this->durationInHours = other.durationInHours;
  this->expireTime = other.expireTime;
  this->priceFactor = other.priceFactor;
  this->sprite = other.sprite;
  std::memcpy(this->flags, other.flags, sizeof(this->flags));
  this->saleData = other.saleData;
  this->grade = other.grade;
  this->productId = other.productId;
  this->optionCount = other.optionCount;
  std::memcpy(this->options, other.options, sizeof(this->options));
  std::memcpy(this->recordIds, other.recordIds, sizeof(this->recordIds));
  this->recordType = other.recordType;
  std::memcpy(this->recordName, other.recordName, sizeof(this->recordName));
  std::memcpy(this->recordDate, other.recordDate, sizeof(this->recordDate));
}

// OFFSET: 0x006D52B0
// FUNCTION: fn_06569
__declspec(noinline) ITEM_INFO& ITEM_INFO::operator=(const ITEM_INFO& other) {
  for (int index = 0; index < 4; ++index) {
    this->identity[index] = other.identity[index];
  }
  this->quantity = other.quantity;
  this->prices[0] = other.prices[0];
  this->prices[1] = other.prices[1];
  memcpy_s(this->cards, sizeof(this->cards), other.cards, sizeof(other.cards));
  this->itemId = other.itemId;
  this->displayName = other.displayName;
  this->identified = other.identified;
  this->damaged = other.damaged;
  this->refiningLevel = other.refiningLevel;
  this->bindOnEquip = other.bindOnEquip;
  this->durationInHours = other.durationInHours;
  this->expireTime = other.expireTime;
  this->priceFactor = other.priceFactor;
  this->sprite = other.sprite;
  this->flags[0] = other.flags[0];
  this->flags[1] = other.flags[1];
  this->flags[2] = other.flags[2];
  this->optionCount = other.optionCount;
  memcpy_s(this->options, sizeof(this->options), other.options, sizeof(other.options));
  this->flags[3] = other.flags[3];
  this->saleData.costume = other.saleData.costume;
  this->saleData.purchaseCount = other.saleData.purchaseCount;
  this->saleData.startTime = other.saleData.startTime;
  this->saleData.endTime = other.saleData.endTime;
  this->grade = other.grade;
  this->productId = other.productId;
  return *this;
}

// OFFSET: 0x006D5550
// FUNCTION: fn_06573
std::uint32_t ITEM_INFO::Assign(const ModeDragState& item) {
  this->identity[0] = item.type;
  this->identity[1] = item.id;
  this->quantity = item.quantity;
  this->prices[0] = item.itemValue;
  this->prices[1] = item.price;

  for (int index = 0; index < 4; ++index) {
    std::memcpy(&this->cards[index], &item.cards[index], sizeof(item.cards[index]));
  }

  this->itemId = item.name;

  this->identified = item.identified;
  this->refiningLevel = item.refine;

  this->optionCount = item.optionCount > 5 ? 0 : item.optionCount;

  for (int index = 0; index < this->optionCount; ++index) {
    this->options[index] = item.options[index];
  }

  this->grade = item.grade;
  this->flags[3] = item.uniqueItem;
  this->identity[2] = item.equipmentLocation;
  this->sprite = static_cast<std::uint32_t>(item.viewId);

  return this->sprite;
}

// OFFSET: 0x006D8A90
// FUNCTION: fn_06606
void ITEM_INFO::Reset() {
  for (int index = 0; index < 4; ++index) {
    this->identity[index] = 0;
  }
  this->quantity = 0;
  for (int index = 0; index < 2; ++index) {
    this->prices[index] = 0;
  }
  std::memset(this->cards, 0, sizeof(this->cards));
  this->itemId = "";
  this->displayName = "";
  this->identified = this->damaged = 0;
  this->refiningLevel = 0;
  this->bindOnEquip = 0;
  this->durationInHours = 0;
  this->expireTime = 0;
  this->priceFactor = 0;
  this->sprite = 0;
  this->optionCount = 0;
  SecureZeroMemory(this->options, sizeof(this->options));
  for (int index = 0; index < 4; ++index) {
    this->flags[index] = 0;
  }
  this->saleData.costume = 0;
  this->saleData.startTime = 0;
  this->saleData.endTime = 0;
  this->saleData.purchaseCount = 0;
  this->grade = 0;
  this->productId = 0;
}

// OFFSET: 0x00DDD4E0
// FUNCTION: fn_23386
ITEM_INFO LookupClientItem(const std::list<ITEM_INFO>& values, int id, unsigned char requireIdentified) {
  if (values.empty()) {
    return ITEM_INFO(0);
  }

  for (auto it = values.begin(); it != values.end(); ++it) {
    if (std::atoi(it->itemId.c_str()) == id && (!requireIdentified || it->identified != 0)) {
      return *it;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00DDD5A0
// FUNCTION: fn_23387
ITEM_INFO CSession::LookupClientItemByIdentity(std::uint32_t id) const {
  const std::list<ITEM_INFO>& values = this->m_inventoryItems;
  if (values.empty()) {
    return ITEM_INFO(0);
  }

  auto it = values.begin();
  for (; it != values.end(); ++it) {
    if (it->identity[1] == id) {
      break;
    }
  }

  return it != values.end() ? *it : ITEM_INFO(0);
}

// INLINED
ITEM_INFO LookupClientItemByIdentity(const std::list<ITEM_INFO>& values, std::uint32_t id) {
  if (values.empty()) {
    return ITEM_INFO(0);
  }

  auto it = values.begin();
  for (; it != values.end(); ++it) {
    if (it->identity[1] == id) {
      break;
    }
  }

  return it != values.end() ? *it : ITEM_INFO(0);
}

// OFFSET: 0x006D5450
// FUNCTION: fn_06570
ITEM_INFO& ITEM_INFO::AssignBasic(const RawItemBasic& item) {
  this->identity[1] = item.id;
  this->prices[0] = item.price;
  this->prices[1] = item.realPrice;
  this->identity[0] = item.type;
  this->identified = item.identified;
  this->itemId = item.itemId;
  this->sprite = item.sprite;
  this->identity[2] = item.location;
  this->flags[3] = item.id != -1;
  return *this;
}

// OFFSET: 0x006D54C0
// FUNCTION: fn_06571
bool ITEM_INFO::operator<(const ITEM_INFO& other) const {
  return this->displayName < other.displayName;
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
  this->displayName = name;
  return this->displayName.empty();
}

// OFFSET: 0x006D9010
// FUNCTION: fn_06610
void ITEM_INFO::SetItemId(int value) {
  char buffer[16] = {};
  this->itemId = _itoa(value, buffer, 10);
}

// OFFSET: 0x006D90C0
// FUNCTION: fn_06612
std::uint32_t ITEM_INFO::Deserialize(const std::vector<unsigned char>& data) {
  if (data.begin() == data.end()) {
    return 0xFFFFFFFF;
  }

  std::uint32_t offset = 6;
  ItemRecordHeader header;
  header.Read(data.data());
  if (281 != header.id) {
    return 0xFFFFFFFF;
  }

  while (data.size() > offset) {
    header.Read(data.data() + offset);
    offset += 6;
    switch (header.id) {
    case 282:
      return offset;
    case 283:
      std::memcpy(&this->identity[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 285:
      std::memcpy(&this->identity[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 284:
      std::memcpy(&this->identity[2], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 286:
      std::memcpy(&this->identity[3], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 287:
      std::memcpy(&this->quantity, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 288:
      std::memcpy(&this->prices[0], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 289:
      std::memcpy(&this->prices[1], data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 290:
      std::memcpy(this->cards, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 291: {
      int id = 0;
      std::memcpy(&id, data.data() + offset, header.bytes);
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
      this->displayName = buffer;
      break;
    }
    case 293:
      std::memcpy(&this->identified, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 294:
      std::memcpy(&this->damaged, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 295:
      std::memcpy(&this->refiningLevel, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 296:
      std::memcpy(&this->bindOnEquip, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 297:
      std::memcpy(&this->expireTime, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 298:
      std::memcpy(&this->sprite, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 299:
      std::memcpy(&this->grade, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 300:
      std::memcpy(&this->optionCount, data.data() + offset, header.bytes);
      offset += header.bytes;
      break;
    case 301:
      std::memcpy(this->options, data.data() + offset, header.bytes);
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
  for (auto it = this->storageItems.begin(); it != this->storageItems.end(); ++it) {
    const ITEM_INFO& required = *it;
    const ITEM_INFO owned = this->LookupClientItemByIdentity(required.identity[1]);
    if (owned.identity[1] <= 0 || owned.quantity < required.quantity) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00DFBD90
// FUNCTION: fn_23588
bool CSession::IsSlotItemIdentity(std::uint32_t id) const {
  if (id != 0 && id == this->equippedArrowIndex) {
    return true;
  }

  for (int index = 0; index < 10; ++index) {
    if (this->equippedItems[index].quantity == 1 && this->equippedItems[index].identity[1] == id) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DFBDE0
// FUNCTION: fn_23589
bool CSession::HasInventoryItem(std::uint32_t id) const {
  for (const ITEM_INFO& value : this->m_inventoryItems) {
    if (value.identity[1] == id) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DFBE40
// FUNCTION: fn_23591
bool CSession::HasItemId(int itemId) const {
  const ITEM_INFO owned = this->LookupClientItemByIdentity(this->equippedArrowIndex);
  if (owned.identity[1] > 0 && std::atoi(owned.itemId.c_str()) == itemId) {
    return true;
  }

  for (int index = 0; index < 10; ++index) {
    if (std::atoi(this->equippedItems[index].itemId.c_str()) == itemId || std::atoi(this->costumeItems[index].itemId.c_str()) == itemId) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00DFBF70
// FUNCTION: fn_23592
bool CSession::HasItemCount(int itemId, int count) const {
  const ITEM_INFO owned = this->LookupClientItemByIdentity(this->equippedArrowIndex);
  if (owned.identity[1] != 0 && std::atoi(owned.itemId.c_str()) == itemId) {
    return true;
  }

  for (int index = 0; index < 10; ++index) {
    if (std::atoi(this->equippedItems[index].itemId.c_str()) == itemId && this->equippedItems[index].refiningLevel >= count) {
      return true;
    }
    if (std::atoi(this->costumeItems[index].itemId.c_str()) == itemId && this->costumeItems[index].refiningLevel >= count) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x006D88D0
// FUNCTION: fn_06600
bool ITEM_INFO::HasSpecialCard() const {
  std::uint32_t card;
  std::memcpy(&card, this->cards, sizeof(card));
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
  case ITEMID_FLAME_HEART:
  case ITEMID_MISTIC_FROZEN:
  case ITEMID_ROUGH_WIND:
  case ITEMID_GREAT_NATURE:
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
    this->options[index].id = option.id;
    this->options[index].parameter = option.parameter;
    this->options[index].value = option.value;
  }
}

// OFFSET: 0x006D9590
// FUNCTION: fn_06613
void ITEM_INFO::SetCard(const int& card, const int& index) {
  if (index < 4) {
    std::memcpy(&this->cards[index], &card, sizeof(card));
  }
}

// OFFSET: 0x006D9620
// FUNCTION: fn_06614
bool LoadItemIntegerTable(std::vector<int>& values, const char* name) {
  values.clear();
  CFile file;
  if (g_serverLanguage != nullptr) {
    name = g_serverLanguage->ServiceName(name);
  }
  if (!file.Open(name, 0, true, false)) {
    MessageBoxA(nullptr, "Can't open file", "ErrorMessage", 0);
    return false;
  }

  const unsigned int size = file.GetSize();
  char* buffer = new char[size + 1];
  values.reserve(size);
  file.Read(buffer, size);
  buffer[size] = '\0';
  file.Close();

  char* token = buffer;
  for (int index = 0; index < static_cast<int>(size); ++index) {
    if (buffer[index] == '/' && buffer[index + 1] == '/') {
      do {
        if (buffer[index] == '\r' || index >= static_cast<int>(size)) {
          break;
        }
        ++index;
      } while (buffer[index] != '\n');
      token = buffer + index + 1;
    }
    switch (buffer[index]) {
    case '\t':
    case ' ':
      buffer[index] = '\0';
      values.push_back(std::atoi(token));
      token = buffer + index + 1;
      break;
    case '\n':
    case '\r':
      token = buffer + index + 1;
      break;
    }
  }
  if (buffer != nullptr) {
    delete[] buffer;
  }
  std::vector<int>(values).swap(values);
  return true;
}

// OFFSET: 0x00E48E90
// FUNCTION: fn_24229
int ReadItemList(std::list<ITEM_INFO>& items, int offset, int size, const std::vector<unsigned char>& data) {
  if (size <= 0) {
    return offset;
  }

  std::vector<unsigned char> buffer(size, 0);
  std::memcpy(buffer.data(), data.data() + offset, size);
  int end = offset + size;
  std::uint32_t consumed = 0;
  std::vector<unsigned char> remaining(buffer);
  while (consumed < buffer.size()) {
    ITEM_INFO item;
    consumed += item.Deserialize(remaining);
    items.push_back(item);
    int left = static_cast<int>(buffer.size()) - static_cast<int>(consumed);
    if (left > 0) {
      std::memcpy(remaining.data(), buffer.data() + consumed, left);
    }
  }

  return end;
}

// OFFSET: 0x00E49090
// FUNCTION: fn_24230
int ReadItemArray(ITEM_INFO* items, int offset, int size, const std::vector<unsigned char>& data) {
  if (size <= 0) {
    return offset;
  }

  std::vector<unsigned char> buffer(size, 0);
  std::memcpy(buffer.data(), data.data() + offset, size);
  int end = offset + size;
  std::uint32_t consumed = 0;
  ITEM_INFO item;
  std::vector<unsigned char> remaining(buffer);
  for (int i = 0; i < 10; ++i) {
    consumed += item.Deserialize(remaining);
    items[i] = item;
    if (consumed >= buffer.size()) {
      break;
    }

    std::memcpy(remaining.data(), buffer.data() + consumed, buffer.size() - consumed);
  }

  return end;
}

// OFFSET: 0x00DDA740
// FUNCTION: fn_23342
bool CSession::RemoveMerchantMyShopItem(const ITEM_INFO& item, int type, unsigned char refresh) {
  for (auto it = this->merchantMyShopItems.begin(); it != this->merchantMyShopItems.end(); ++it) {
    if (it->identity[1] == item.identity[1]) {
      if (type != 0) {
        this->merchantMyShopItems.erase(it);
      } else {
        it->quantity -= item.quantity;
        if (it->quantity <= 0) {
          this->merchantMyShopItems.erase(it);
        }
      }

      if (refresh == 1) {
        UIWindow* window;
        if (type == 1) {
          window = g_windowMgr.buyingShopMirrorItemWnd;
        } else {
          window = type == 2 ? g_windowMgr.purchaseMirrorItemWnd : g_windowMgr.merchantMirrorItemWnd;
        }

        if (window) {
          window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        }
      }
      return true;
    }
  }

  if (refresh == 1) {
    UIWindow* window;
    if (type == 1) {
      window = g_windowMgr.buyingShopMirrorItemWnd;
    } else {
      window = type == 2 ? g_windowMgr.purchaseMirrorItemWnd : g_windowMgr.merchantMirrorItemWnd;
    }

    if (window) {
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }
  }
  return false;
}

// OFFSET: 0x00DDAB30
// FUNCTION: fn_23348
void CSession::RemovePurchaseItem(const ITEM_INFO& item) {
  const bool hasFlag = item.flags[3] != 0;
  for (auto it = this->purchaseItems.begin(); it != this->purchaseItems.end(); ++it) {
    const bool same = hasFlag ? it->identity[1] == item.identity[1] : it->itemId == item.itemId;
    if (same) {
      it->quantity -= item.quantity;
      if (it->quantity <= 0) {
        this->purchaseItems.erase(it);
      }
      return;
    }
  }
}
