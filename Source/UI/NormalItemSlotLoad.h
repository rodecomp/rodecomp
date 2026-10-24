#ifndef RAGNAROK_SOURCE_UI_NORMALITEMSLOTLOAD_H
#define RAGNAROK_SOURCE_UI_NORMALITEMSLOTLOAD_H

#include "Contents/NormalItemSlots.h"

class WindowManagerWindows;
struct ShortcutVisibility;

void LoadNormalItemSlots(
  Json::Value& data,
  NormalItemSlot* slots,
  NormalItemSlotWindow& saved,
  bool& itemSlotActive,
  WindowManagerWindows& windows,
  ShortcutVisibility& visibility
);

#endif
