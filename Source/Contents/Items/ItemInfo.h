#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMINFO_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMINFO_H

#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

struct RawItemData {
  /* 0000 */ std::uint32_t unknown_0000;
  /* 0004 */ std::uint32_t unknown_0004;
  /* 0008 */ std::uint32_t unknown_0008;
  /* 000C */ std::uint32_t unknown_000C;
  /* 0010 */ std::int32_t quantity;
  /* 0014 */ std::uint32_t unknown_0014;
  /* 0018 */ std::uint32_t unknown_0018;
  /* 001C */ std::uint32_t unknown_001C;
  /* 0020 */ unsigned char identified;
  /* 0021 */ unsigned char unknown_0021[27];
  /* 003C */ std::string itemId;
  /* 0054 */ unsigned char unknown_0054[28];
  /* 0070 */ std::uint32_t cards[4];
  /* 0080 */ unsigned char unknown_0080[8];
  /* 0088 */ std::int32_t optionCount;
  /* 008C */ unsigned char options[25];
  /* 00A5 */ unsigned char unknown_00A5[7];
  /* 00AC */ std::int16_t unknown_00AC;
  /* 00AE */ unsigned char unknown_00AE;
  /* 00AF */ unsigned char unknown_00AF;
  /* 00B0 */ std::uint32_t unknown_00B0;
  /* 00B4 */ std::uint32_t unknown_00B4;
  /* 00B8 */ std::uint32_t unknown_00B8;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RawItemData) == 188);
#endif

struct RawItemBasic {
  /* 0000 */ std::int32_t id;
  /* 0004 */ std::uint32_t unknown_0004;
  /* 0008 */ std::uint32_t unknown_0008;
  /* 000C */ std::uint32_t unknown_000C;
  /* 0010 */ unsigned char identified;
  /* 0011 */ char itemId[33];
  /* 0034 */ std::uint32_t unknown_0034;
  /* 0038 */ std::int32_t unknown_0038;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(RawItemBasic, itemId) == 0x11);
static_assert(offsetof(RawItemBasic, unknown_0034) == 0x34);
static_assert(offsetof(RawItemBasic, unknown_0038) == 0x38);
#endif

struct ItemOption {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t value;
  /* 0004 */ unsigned char parameter;
};

struct alignas(8) ITEM_INFO {
  ITEM_INFO();
  explicit ITEM_INFO(int unused);
  ITEM_INFO(const ITEM_INFO& other);
  ITEM_INFO& operator=(const ITEM_INFO& other);
  void Reset();
  std::uint32_t Assign(const RawItemData& item);
  ITEM_INFO& AssignBasic(const RawItemBasic& item);
  bool operator<(const ITEM_INFO& other) const;
  bool SetName(const char* name);
  std::string DisplayName(unsigned char detailed);
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
  int sub_6D5810(char* output, int& capacity, unsigned char consume);
  std::vector<unsigned char> Serialize() const;
  std::uint32_t Deserialize(const std::vector<unsigned char>& data);
  /* 0000 */ std::uint32_t unknown_0000[4];
  /* 0010 */ std::int32_t quantity;
  /* 0014 */ std::uint32_t unknown_0014[2];
  /* 001C */ unsigned char unknown_001C[16];
  /* 002C */ std::string itemId;
  /* 0044 */ std::string unknown_0044;
  /* 005C */ unsigned char identified;
  /* 005D */ unsigned char unknown_005D;
  /* 0060 */ std::int32_t unknown_0060;
  /* 0064 */ std::uint16_t unknown_0064;
  /* 0066 */ unsigned char unknown_0066;
  /* 0068 */ std::uint32_t unknown_0068;
  /* 006C */ std::uint16_t unknown_006C;
  /* 0070 */ std::int32_t unknown_0070;
  /* 0074 */ unsigned char unknown_0074[4];
  /* 0078 */ std::uint32_t unknown_0078[4];
  /* 0088 */ std::int16_t unknown_0088;
  /* 0090 */ std::uint64_t unknown_0090;
  /* 0098 */ std::int32_t unknown_0098;
  /* 009C */ unsigned char unknown_009C[25];
  /* 00B8 */ std::uint32_t unknown_00B8[2];
  /* 00C0 */ unsigned char unknown_00C0;
  /* 00C1 */ unsigned char unknown_00C1[24];
  /* 00D9 */ unsigned char unknown_00D9[24];
};

ITEM_INFO LookupClientItem(const std::list<ITEM_INFO>& values, int id, unsigned char requireIdentified);
ITEM_INFO LookupClientItemByIdentity(const std::list<ITEM_INFO>& values, std::uint32_t id);
bool IsNumericText(const char* text);
bool MatchItemName(std::string name, std::string query, unsigned char partial, std::list<int>* result, int id);
std::list<int> SearchItemIds(std::string query, unsigned char cardsOnly, unsigned char partial);
int ReadItemList(std::list<ITEM_INFO>& items, int offset, int size, const std::vector<unsigned char>& data);
int ReadItemArray(ITEM_INFO* items, int offset, int size, const std::vector<unsigned char>& data);


#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ITEM_INFO) == 248);
static_assert(sizeof(ItemOption) == 6);
#endif

#endif
