#include "UI/NormalItemSlotLoad.h"
#include "UI/ShortcutVisibility.h"
#include "UI/UIWindow.h"
#include "UI/WindowManagerWindows.h"

#include <cstring>
#include <json/value.h>

// OFFSET: 0x005E65E0
// FUNCTION: fn_03929
void LoadNormalItemSlots(
  Json::Value& data,
  NormalItemSlot* slots,
  NormalItemSlotWindow& saved,
  bool& itemSlotActive,
  WindowManagerWindows& windows,
  ShortcutVisibility& visibility
) {
  Json::Value& values = data["NormalItemSlot"];

  if (values.isNull()) {
    return;
  }

  std::memset(slots, 0, 9 * sizeof(NormalItemSlot));

  for (auto entry = values.begin(); entry != values.end(); ++entry) {
    Json::Value& value = *entry;

    if (!value.isObject()) {
      continue;
    }

    const int item = value.get("ItemID", 0).asInt();
    const int index = value.get("SlotNum", 0).asInt();
    slots[index].itemId = static_cast<std::uint32_t>(item);
  }

  Json::Value& info = data["SlotInfo"];

  if (info.empty()) {
    saved.Reset();
  } else {
    const int x = info.get("SlotX", 0).asInt();
    const int y = info.get("SlotY", 0).asInt();
    const int width = info.get("SlotWidth", 0).asInt();
    const int height = info.get("SlotHeight", 0).asInt();
    const int show = info.get("SlotShow", 1).asInt();

    saved.x = x;
    saved.y = y;
    saved.width = width;
    saved.height = height;
    saved.show = show;
  }

  Json::Value& active = data["ItemSlotActive"];
  itemSlotActive = active.isNull() ? true : active.asBool();

  UIWindow* window = windows.LookupWindow(36);

  if (!window) {
    return;
  }

  window->geometry.x = saved.x;
  window->geometry.y = saved.y;
  window->geometry.width = saved.width;
  window->geometry.height = saved.height;

  visibility.Show(saved.show);
  window->SendMsg(nullptr, 34, reinterpret_cast<std::intptr_t>(&saved), 0, 0, 0);
  window->SendMsg(nullptr, 6, itemSlotActive ? 588 : 589, 0, 0, 0);
}
