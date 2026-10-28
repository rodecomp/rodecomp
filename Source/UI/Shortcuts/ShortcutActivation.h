#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTACTIVATION_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTACTIVATION_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Shortcuts/ShortcutGrid.h"

class AutomaticSkills;
class UIWindow;

void ActivateShortcut(
  UIWindow& window, ShortcutGrid& grid, int x, int y, bool itemSlotActive, unsigned char& currentRow, const NormalItemSlot* const* slots,
  AutomaticSkills& automatic
);

#endif
