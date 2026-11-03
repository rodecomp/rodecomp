#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTCONTROLS_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTCONTROLS_H

class UIFrameWnd;
class UICheckBox;
class UIBitmapButton;
struct NormalItemSlot;

struct ShortcutControls {
  UICheckBox* bank;
  UIBitmapButton* decrease;
  UIBitmapButton* increase;
  UIBitmapButton* showItems;
  UIBitmapButton* hideItems;

  static ShortcutControls
  Create(UIFrameWnd& frame, int bankOption, NormalItemSlot** slots, NormalItemSlot** itemSlots);
};

#endif
