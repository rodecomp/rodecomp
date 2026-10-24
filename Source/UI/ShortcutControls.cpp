#include "UI/ShortcutControls.h"
#include "UI/UIBitmapButton.h"
#include "UI/UICheckBox.h"
#include "UI/UIFrameWnd.h"

#include <bit>
#include <new>

#define SHORTCUT_RESOURCE "\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA"

// OFFSET: 0x0092A190
// FUNCTION: fn_12654
ShortcutControls ShortcutControls::Create(
  UIFrameWnd& frame,
  UIWindowContext& windows,
  UIButtonContext& buttons,
  int bankOption,
  NormalItemSlot** slots,
  NormalItemSlot** itemSlots
) {
  ShortcutControls result;
  frame.AddCloseButton(buttons, "F12", 0)
    ->Move(std::bit_cast<int>(static_cast<unsigned>(frame.geometry.width) - 10u), 0);
  for (int index = 0; index < 36; ++index) {
    slots[index] = nullptr;
  }
  result.bank = new (std::nothrow) UICheckBox(windows, buttons);
  result.bank->SetSize(10, 26);
  result.bank->SetImages(
    "\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA\\shortcut_2_bts.bmp",
    "\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA\\shortcut_1_bts.bmp", ""
  );
  result.bank->SetId(549);
  result.bank->Move(3, 4);
  result.bank->SetChecked(bankOption);
  result.bank->SetTooltip(buttons.services.MessageText(3596));
  frame.AddChild(result.bank);

  const auto create = [&](
                        int id, int x, int y, unsigned char disabled, unsigned char enabled, const char* normal,
                        const char* over, const char* pressed, const char* unavailable
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
    217, 276, 0, 0, 0, SHORTCUT_RESOURCE "\\Shortcut\\btn_minus_normal.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_minus_over.bmp", SHORTCUT_RESOURCE "\\Shortcut\\btn_minus_press.bmp", nullptr
  );
  result.increase = create(
    216, 276, 19, 0, 1, SHORTCUT_RESOURCE "\\Shortcut\\btn_plus_normal.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_plus_over.bmp", SHORTCUT_RESOURCE "\\Shortcut\\btn_plus_press.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_plus_disable.bmp"
  );
  for (int index = 0; index < 9; ++index) {
    itemSlots[index] = nullptr;
  }
  result.showItems = create(
    588, 247, 33, 1, 0, SHORTCUT_RESOURCE "\\Shortcut\\btn_open_itemslot_normal.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_open_itemslot_normal.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_open_itemslot_press.bmp", nullptr
  );
  result.hideItems = create(
    589, 247, 33, 0, 1, SHORTCUT_RESOURCE "\\Shortcut\\btn_close_itemslot_normal.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_close_itemslot_normal.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_close_itemslot_press.bmp", nullptr
  );
  frame.SendMsg(nullptr, 23, 0, 0, 0, 0);
  return result;
}
