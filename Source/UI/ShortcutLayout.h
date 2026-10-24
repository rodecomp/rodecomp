#ifndef RAGNAROK_SOURCE_UI_SHORTCUTLAYOUT_H
#define RAGNAROK_SOURCE_UI_SHORTCUTLAYOUT_H

#include "Contents/NormalItemSlots.h"
#include "UI/ShortcutGrid.h"

class UIWindow;
class UIBitmapButton;

// Original shortcut fields+184,+188,+192,+388,+392 and client item-row byte.
// Borrowed controls are actual child windows, not layout-only substitutes.
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
