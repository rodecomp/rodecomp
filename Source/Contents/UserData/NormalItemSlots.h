#ifndef RAGNAROK_SOURCE_CONTENTS_USERDATA_NORMALITEMSLOTS_H
#define RAGNAROK_SOURCE_CONTENTS_USERDATA_NORMALITEMSLOTS_H

#include <cstddef>
#include <cstdint>

namespace Json {
  class Value;
}

#pragma pack(push, 1)
struct NormalItemSlot {
  std::uint8_t unknown_0000;
  std::uint32_t itemId;
  std::int16_t unknown_0005;
};
#pragma pack(pop)
static_assert(sizeof(NormalItemSlot) == 7);
static_assert(offsetof(NormalItemSlot, itemId) == 1);

struct NormalItemSlotWindow {
  int x;
  int y;
  int width;
  int height;
  int show;
  void Reset();
  bool IsValid(int screenWidth, int screenHeight, bool active) const;
};

void ExportNormalItemSlots(Json::Value& data, const NormalItemSlot* slots, const NormalItemSlotWindow& window, bool active);

#endif
