#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTLAYOUT_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTLAYOUT_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Shortcuts/ShortcutGrid.h"

class UIWindow;
class UIBitmapButton;

struct ShortcutLayout {
  UIWindow& window;
  ShortcutGrid& grid;
  NormalItemSlotWindow*& saved;
  UIBitmapButton& decrease;
  UIBitmapButton& increase;
  UIBitmapButton& showItems;
  UIBitmapButton& hideItems;
  const bool& itemSlotActive;
  const int& screenWidth;
  const int& screenHeight;

  void ResizeRows();
  void Restore(NormalItemSlotWindow* value);
  void Persist();

private:
  void PlaceRowButtons();
};

#endif
