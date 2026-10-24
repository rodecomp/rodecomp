#ifndef RAGNAROK_SOURCE_CONTENTS_NORMALITEMSLOTS_H
#define RAGNAROK_SOURCE_CONTENTS_NORMALITEMSLOTS_H

#include <cstddef>
#include <cstdint>
namespace Json {
  class Value;
}

// Original client record at +0x6AC: 9 records, stride7. Only itemId is
// serialized by this operation; unknown bytes are not discarded globally.
#pragma pack(push, 1)
struct NormalItemSlot {
  std::uint8_t unknown_0000;
  std::uint32_t itemId;
  std::uint16_t unknown_0005;
};
#pragma pack(pop)
static_assert(sizeof(NormalItemSlot) == 7);
static_assert(offsetof(NormalItemSlot, itemId) == 1);

// Window-state globals.
struct NormalItemSlotWindow {
  int x;
  int y;
  int width;
  int height;
  int show;
  void Reset();
  bool IsValid(int screenWidth, int screenHeight, bool active) const;
};

// Original caller supplies slots, window globals and client active flag+0x812D.
void ExportNormalItemSlots(
  Json::Value& data,
  const NormalItemSlot* slots,
  const NormalItemSlotWindow& window,
  bool active
);

#endif
