#ifndef RAGNAROK_SOURCE_UI_SHORTCUTCONTROLS_H
#define RAGNAROK_SOURCE_UI_SHORTCUTCONTROLS_H

struct UIWindowContext;
struct UIButtonContext;
class UIFrameWnd;
class UICheckBox;
class UIBitmapButton;
struct NormalItemSlot;

// Borrowed original child pointers; the frame's child list owns the windows.
struct ShortcutControls {
  UICheckBox* bank;
  UIBitmapButton* decrease;
  UIBitmapButton* increase;
  UIBitmapButton* showItems;
  UIBitmapButton* hideItems;

  static ShortcutControls Create(
    UIFrameWnd& frame,
    UIWindowContext& windows,
    UIButtonContext& buttons,
    int bankOption,
    NormalItemSlot** slots,
    NormalItemSlot** itemSlots
  );
};

#endif
