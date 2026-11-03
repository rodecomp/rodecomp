#include "Contents/MessageTable.h"
#include "Client/Session.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"

#include <new>

#define SHORTCUT_RESOURCE "유저인터페이스"

// OFFSET: 0x0092A190
// FUNCTION: fn_12654
void UIShortCutWnd::Slot15(int, int) {
  this->AddCloseButton("F12", 0)->Move(this->geometry.width - 10, 0);

  for (int index = 0; index < 36; ++index) {
    this->m_slots[index] = nullptr;
  }

  this->m_bank = new (std::nothrow) UICheckBox();
  this->m_bank->SetSize(10, 26);
  this->m_bank->SetImages(
    "유저인터페이스\\shortcut_2_bts.bmp",
    "유저인터페이스\\shortcut_1_bts.bmp", ""
  );
  this->m_bank->SetId(549);
  this->m_bank->Move(3, 4);
  this->m_bank->SetChecked(g_session.GetOption(10));
  this->m_bank->SetTooltip(LookupMessage(3596));
  this->AddChild(this->m_bank);

  const auto create = [&](
                        int id, int x, int y, unsigned char disabled, unsigned char enabled, const char* normal, const char* over, const char* pressed,
                        const char* unavailable
                      ) {
    UIBitmapButton* button = new UIBitmapButton();
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
    this->AddChild(button);

    return button;
  };
  this->m_decrease = create(
    217, 276, 0, 0, 0, SHORTCUT_RESOURCE "\\Shortcut\\btn_minus_normal.bmp", SHORTCUT_RESOURCE "\\Shortcut\\btn_minus_over.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_minus_press.bmp", nullptr
  );
  this->m_increase = create(
    216, 276, 19, 0, 1, SHORTCUT_RESOURCE "\\Shortcut\\btn_plus_normal.bmp", SHORTCUT_RESOURCE "\\Shortcut\\btn_plus_over.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_plus_press.bmp", SHORTCUT_RESOURCE "\\Shortcut\\btn_plus_disable.bmp"
  );

  for (int index = 0; index < 9; ++index) {
    this->m_itemSlots[index] = nullptr;
  }

  this->m_showItems = create(
    588, 247, 33, 1, 0, SHORTCUT_RESOURCE "\\Shortcut\\btn_open_itemslot_normal.bmp", SHORTCUT_RESOURCE "\\Shortcut\\btn_open_itemslot_normal.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_open_itemslot_press.bmp", nullptr
  );
  this->m_hideItems = create(
    589, 247, 33, 0, 1, SHORTCUT_RESOURCE "\\Shortcut\\btn_close_itemslot_normal.bmp", SHORTCUT_RESOURCE "\\Shortcut\\btn_close_itemslot_normal.bmp",
    SHORTCUT_RESOURCE "\\Shortcut\\btn_close_itemslot_press.bmp", nullptr
  );
  this->SendMsg(nullptr, 23, 0, 0, 0, 0);
}
