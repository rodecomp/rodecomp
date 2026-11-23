#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMINFO_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMINFO_H

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <list>
#include <string>
#include <vector>
#include <windows.h>

struct ModeDragState;

struct RawItemBasic {
  // OFFSET: 0x00D042A0
  // FUNCTION: fn_21557
  RawItemBasic() : id(0), price(0), realPrice(0), type(0), identified(0), location(0), sprite(0) {
    SecureZeroMemory(this->itemId, sizeof(this->itemId));
  }

  /* 0000 */ std::int32_t id;
  /* 0004 */ std::uint32_t price;
  /* 0008 */ std::uint32_t realPrice;
  /* 000C */ std::uint32_t type;
  /* 0010 */ unsigned char identified;
  /* 0011 */ char itemId[33];
  /* 0034 */ std::uint32_t location;
  /* 0038 */ std::int32_t sprite;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(RawItemBasic, itemId) == 0x11);
static_assert(offsetof(RawItemBasic, location) == 0x34);
static_assert(offsetof(RawItemBasic, sprite) == 0x38);
#endif

struct ItemOption {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t value;
  /* 0004 */ unsigned char parameter;
};

#pragma pack(push, 1)
struct ItemOptionRecord {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t value;
  /* 0004 */ unsigned char parameter;
};
#pragma pack(pop)

struct ItemSaleData {
  /* 0000 */ std::uint32_t costume;
  /* 0004 */ __time32_t startTime;
  /* 0008 */ __time32_t endTime;
  /* 000C */ std::int32_t purchaseCount;
};

struct ITEM_INFO {
  ITEM_INFO();
  explicit ITEM_INFO(int unused);
  ITEM_INFO(const ITEM_INFO& other);
  ITEM_INFO& operator=(const ITEM_INFO& other);
  void Reset();
  std::uint32_t Assign(const ModeDragState& item);
  ITEM_INFO& AssignBasic(const RawItemBasic& item);
  bool operator<(const ITEM_INFO& other) const;
  bool SetName(const char* name);
  std::string DisplayName(unsigned char detailed) const;
  bool HasCards() const;
  bool IsEquipment() const;
  const char* ResourcePath() const;
  int SlotCount() const;
  int Grade(char* output, int& capacity, unsigned char consume) const;
  int Creator(char* output, int& capacity, unsigned char consume) const;
  int Prefixes(char* output, int& capacity, unsigned char consume) const;
  int Suffixes(char* output, int& capacity, unsigned char consume) const;
  int Slots(char* output, int& capacity, unsigned char consume) const;
  int Element(char* output, int& capacity, unsigned char consume) const;
  int Star(char* output, int& capacity, unsigned char consume) const;
  int Apprentice(char* output, int& capacity, unsigned char consume) const;
  void SetItemId(int value);
  void SetOption(ItemOption option, const int& index);
  void SetCard(const int& card, const int& index);
  int GetItemId() const;
  bool HasSpecialCard() const;
  bool IsApprenticeItem() const;
  bool IsForgeMaterial() const;
  bool IsElementalStone() const;
  bool IsAboveItemId600() const;
  int Refine(char* output, int& capacity, unsigned char consume) const;
  int GetIdDisplayName(char* output, int& capacity, unsigned char consume) const;
  int UnidentifiedName(char* output, int& capacity, unsigned char consume) const;
  const std::vector<const char*>& Description() const;
  std::vector<unsigned char> Serialize() const;
  std::uint32_t Deserialize(const std::vector<unsigned char>& data);
  union {
    /* 0000 */ std::uint32_t identity[4];
    struct {
      /* 0000 */ std::uint32_t itemType;
      /* 0004 */ std::uint32_t itemIndex;
      /* 0008 */ std::uint32_t location;
      /* 000C */ std::uint32_t wearLocation;
    };
  };
  /* 0010 */ std::int32_t quantity;
  /* 0014 */ std::uint32_t prices[2];
  /* 001C */ std::uint32_t cards[4];
  /* 002C */ std::string itemId;
  /* 0044 */ std::string displayName;
  /* 005C */ unsigned char identified;
  /* 005D */ unsigned char damaged;
  /* 0060 */ std::int32_t refiningLevel;
  /* 0064 */ std::uint16_t bindOnEquip;
  /* 0066 */ unsigned char durationInHours;
  /* 0068 */ __time32_t expireTime;
  /* 006C */ std::uint16_t priceFactor;
  /* 0070 */ std::int32_t sprite;
  /* 0074 */ unsigned char placeEtcTab;
  /* 0075 */ unsigned char selected;
  /* 0076 */ unsigned char flagged;
  /* 0077 */ unsigned char uniqueItem;
  /* 0078 */ ItemSaleData saleData;
  /* 0088 */ std::int16_t grade;
  /* 0090 */ std::uint64_t productId;
  /* 0098 */ std::int32_t optionCount;
  /* 009C */ ItemOptionRecord options[5];
  /* 00B8 */ std::uint32_t recordIds[2];
  /* 00C0 */ unsigned char recordType;
  /* 00C1 */ unsigned char recordName[24];
  /* 00D9 */ unsigned char recordDate[24];
};

bool IsNumericText(const char* text);
bool LoadItemIntegerTable(std::vector<int>& values, const char* name);
bool MatchItemName(std::string name, std::string query, unsigned char partial, std::list<int>* result, int id);
std::list<int> SearchItemIds(std::string query, unsigned char cardsOnly, unsigned char partial);
int ReadItemList(std::list<ITEM_INFO>& items, int offset, int size, const std::vector<unsigned char>& data);
int ReadItemArray(ITEM_INFO* items, int offset, int size, const std::vector<unsigned char>& data);


#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ITEM_INFO) == 248);
static_assert(sizeof(ItemOption) == 6);
static_assert(sizeof(ItemOptionRecord) == 5);
static_assert(sizeof(ItemSaleData) == 0x10);
static_assert(offsetof(ItemSaleData, startTime) == 0x04);
static_assert(offsetof(ItemSaleData, endTime) == 0x08);
static_assert(offsetof(ITEM_INFO, expireTime) == 0x68);
static_assert(offsetof(ITEM_INFO, saleData) == 0x78);
#endif

#endif
