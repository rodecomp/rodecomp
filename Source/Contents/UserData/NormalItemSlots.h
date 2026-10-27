#ifndef RAGNAROK_SOURCE_CONTENTS_USERDATA_NORMALITEMSLOTS_H
#define RAGNAROK_SOURCE_CONTENTS_USERDATA_NORMALITEMSLOTS_H

#include <cstddef>
#include <cstdint>

namespace Json {
  class Value;
}

#pragma pack(push, 1)
struct NormalItemSlot {
  /* 0000 */ std::uint8_t unknown_0000;
  /* 0001 */ std::uint32_t itemId;
  /* 0005 */ std::int16_t unknown_0005;
};
#pragma pack(pop)
static_assert(sizeof(NormalItemSlot) == 7);
static_assert(offsetof(NormalItemSlot, itemId) == 1);
static_assert(offsetof(NormalItemSlot, unknown_0005) == 5);

struct NormalItemSlotWindow {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
  /* 0010 */ int show;
  void Reset();
  bool IsValid(int screenWidth, int screenHeight, bool active) const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(NormalItemSlotWindow, x) == 0x0000);
static_assert(offsetof(NormalItemSlotWindow, y) == 0x0004);
static_assert(offsetof(NormalItemSlotWindow, width) == 0x0008);
static_assert(offsetof(NormalItemSlotWindow, height) == 0x000C);
static_assert(offsetof(NormalItemSlotWindow, show) == 0x0010);
#endif

void ExportNormalItemSlots(Json::Value& data, const NormalItemSlot* slots, const NormalItemSlotWindow& window, bool active);

#endif
