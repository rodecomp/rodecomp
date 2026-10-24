#ifndef RAGNAROK_SOURCE_UI_SHORTCUTACTIVATION_H
#define RAGNAROK_SOURCE_UI_SHORTCUTACTIVATION_H

#include "Contents/NormalItemSlots.h"
#include "UI/ShortcutGrid.h"

class AutomaticSkills;
struct ClientOptions;
class UIWindow;

void ActivateShortcut(
  UIWindow& window,
  ShortcutGrid& grid,
  int x,
  int y,
  bool itemSlotActive,
  unsigned char& currentRow,
  const NormalItemSlot* const* slots,
  AutomaticSkills& automatic,
  ClientOptions& options
);

#endif
