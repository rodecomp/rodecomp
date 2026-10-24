#include "Contents/NormalItemSlots.h"

#include <bit>
#include <json/value.h>

// OFFSET: 0x009A6C20
// FUNCTION: fn_13725
void NormalItemSlotWindow::Reset() {
  this->x = 230;
  this->y = 0;
  this->width = 290;
  this->height = 76;
  this->show = 1;
}

// OFFSET: 0x009A6000
// FUNCTION: fn_13667
bool NormalItemSlotWindow::IsValid(int screenWidth, int screenHeight, bool active) const {
  if (
    this->width != 290 || this->height < 43 || this->height > 175 || (this->height - 10) % 33 != 0 ||
    (this->height == 175 && !active)
  ) {
    return false;
  }
  const int maxX = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(screenWidth) - 290u);
  const int maxY =
    std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(screenHeight) - static_cast<std::uint32_t>(this->height));
  return this->x >= 0 && this->x <= maxX && this->y >= 0 && this->y <= maxY;
}

// OFFSET: 0x005E75A0
// FUNCTION: fn_03935
void ExportNormalItemSlots(
  Json::Value& data,
  const NormalItemSlot* slots,
  const NormalItemSlotWindow& window,
  bool active
) {
  Json::Value values;
  for (int index = 0; index < 9; ++index) {
    Json::Value value;
    value["ItemID"] = Json::UInt(slots[index].itemId);
    value["SlotNum"] = index;
    values.append(value);
  }
  Json::Value info;
  info["SlotX"] = window.x;
  info["SlotY"] = window.y;
  info["SlotWidth"] = window.width;
  info["SlotHeight"] = window.height;
  info["SlotShow"] = window.show;
  data["SlotInfo"] = info;
  data["NormalItemSlot"] = values;
  data["ItemSlotActive"] = active;
}
