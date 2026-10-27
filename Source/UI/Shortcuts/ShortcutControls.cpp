#include "UI/Shortcuts/ShortcutControls.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Window/UIFrameWnd.h"

#include <new>

#define SHORTCUT_RESOURCE "유저인터페이스"

// OFFSET: 0x0092A190
// FUNCTION: fn_12654
ShortcutControls ShortcutControls::Create(
  UIFrameWnd& frame, UIWindowContext& windows, UIButtonContext& buttons, int bankOption, NormalItemSlot** slots, NormalItemSlot** itemSlots
) {
  ShortcutControls result;
  frame.AddCloseButton(buttons, "F12", 0)->Move(frame.geometry.width - 10, 0);

  for (int index = 0; index < 36; ++index) {
    slots[index] = nullptr;
  }

  result.bank = new (std::nothrow) UICheckBox(windows, buttons);
  result.bank->SetSize(10, 26);
  result.bank->SetImages(
    "유저인터페이스\\shortcut_2_bts.bmp",
    "유저인터페이스\\shortcut_1_bts.bmp", ""
  );
  result.bank->SetId(549);
  result.bank->Move(3, 4);
  result.bank->SetChecked(bankOption);
  result.bank->SetTooltip(buttons.services.MessageText(3596));
  frame.AddChild(result.bank);

  const auto create = [&](
                        int id, int x, int y, unsigned char disabled, unsigned char enabled, const char* normal, const char* over, const char* pressed,
                        const char* unavailable
                      ) {
    UIBitmapButton* button = new UIBitmapButton(windows, buttons);
    button->SetBitmap(normal, 0, 0);
    button->SetBitmap(over, 1, 0);
    button->SetBitmap(pressed, 2, 0);

    if (unavailable) {
      button->SetBitmap(unavailable, 3, 0);
    }

    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->SetId(id);
    button->SetDisabled(disabled);
    button->Invalidate();
    button->Move(x, y);
    button->Enable(enabled);
    frame.AddChild(button);

    return button;
  };
  result.decrease = create(
    217, 276, 0, 0, 0, SHORTCUT_RESOURCE "\\Shortcut\\btn_minus_normal.bmp", SHORTCUT_RESOURCE "\\Shortcut\\btn_minus_over.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_minus_press.bmp", nullptr
  );
  result.increase = create(
    216, 276, 19, 0, 1, SHORTCUT_RESOURCE "\\Shortcut\\btn_plus_normal.bmp", SHORTCUT_RESOURCE "\\Shortcut\\btn_plus_over.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_plus_press.bmp", SHORTCUT_RESOURCE "\\Shortcut\\btn_plus_disable.bmp"
  );

  for (int index = 0; index < 9; ++index) {
    itemSlots[index] = nullptr;
  }

  result.showItems = create(
    588, 247, 33, 1, 0, SHORTCUT_RESOURCE "\\Shortcut\\btn_open_itemslot_normal.bmp", SHORTCUT_RESOURCE "\\Shortcut\\btn_open_itemslot_normal.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_open_itemslot_press.bmp", nullptr
  );
  result.hideItems = create(
    589, 247, 33, 0, 1, SHORTCUT_RESOURCE "\\Shortcut\\btn_close_itemslot_normal.bmp", SHORTCUT_RESOURCE "\\Shortcut\\btn_close_itemslot_normal.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_close_itemslot_press.bmp", nullptr
  );
  frame.SendMsg(nullptr, 23, 0, 0, 0, 0);

  return result;
}
