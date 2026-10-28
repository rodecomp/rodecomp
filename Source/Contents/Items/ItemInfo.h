#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMINFO_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMINFO_H

#include <cstdint>
#include <list>
#include <string>

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

struct alignas(8) ItemInfo {
  ItemInfo();
  explicit ItemInfo(int unused);
  ItemInfo(const ItemInfo& other);
  ItemInfo& operator=(const ItemInfo& other);
  void Reset();
  std::uint32_t Assign(const RawItemData& item);
  ItemInfo& AssignBasic(const RawItemData& item);
  bool operator<(const ItemInfo& other) const;
  bool SetName(const char* name);
  void SetItemId(int value);
  bool HasSpecialCard() const;
  bool IsApprenticeItem() const;
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

ItemInfo LookupClientItem(const std::list<ItemInfo>& values, int id, unsigned char requireIdentified);
ItemInfo LookupClientItemByIdentity(const std::list<ItemInfo>& values, std::uint32_t id);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ItemInfo) == 248);
#endif

#endif
