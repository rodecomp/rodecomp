#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_NORMALITEMSLOTLOAD_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_NORMALITEMSLOTLOAD_H

#include "Contents/UserData/NormalItemSlots.h"

class WindowManagerWindows;
struct ShortcutVisibility;

void LoadNormalItemSlots(
  Json::Value& data, NormalItemSlot* slots, NormalItemSlotWindow& saved, bool& itemSlotActive, WindowManagerWindows& windows, ShortcutVisibility& visibility
);

#endif
