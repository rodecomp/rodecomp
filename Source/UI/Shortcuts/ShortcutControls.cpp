#include "UI/Window/UIEnums.h"
#include "Contents/Tables/MessageTable.h"
#include "Client/Session/Session.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstring>
#include <iterator>
#include <new>

// OFFSET: 0x0092A190
// FUNCTION: fn_12654
void UIShortCutWnd::OnCreate(int, int) {
  UIBitmapButton* closeButton = this->AddCloseButton("F12", 0);
  closeButton->Move(this->geometry.width - 10, 0);

  std::memset(this->m_slots, 0, sizeof(this->m_slots));

  this->m_bank = new (std::nothrow) UICheckBox();
  this->m_bank->SetSize(10, 26);
  this->m_bank->SetImages(
    "유저인터페이스\\shortcut_2_bts.bmp",
    "유저인터페이스\\shortcut_1_bts.bmp", ""
  );
  this->m_bank->SetId(ID_CHANGE_SKILLBAR);
  this->m_bank->Move(3, 4);
  this->m_bank->SetChecked(static_cast<int>(g_session.m_options.Get(10)));
  this->m_bank->SetTooltip(LookupMessage(MSI_CHANGE_SKILLBER));
  this->AddChild(this->m_bank);

  this->m_decrease = new UIBitmapButton();
  this->m_decrease->SetBitmap("유저인터페이스\\Shortcut\\btn_minus_normal.bmp", 0, 0);
  this->m_decrease->SetBitmap("유저인터페이스\\Shortcut\\btn_minus_over.bmp", 1, 0);
  this->m_decrease->SetBitmap("유저인터페이스\\Shortcut\\btn_minus_press.bmp", 2, 0);
  this->m_decrease->SetSize(this->m_decrease->BitmapWidth(), this->m_decrease->BitmapHeight());
  this->m_decrease->SetId(ID_MINUS);
  UIBitmapButton* decreaseButton = this->m_decrease;
  decreaseButton->SetDisabled(0);
  decreaseButton->Invalidate();
  this->m_decrease->Move(276, 0);
  this->m_decrease->Enable(0);
  this->AddChild(this->m_decrease);

  this->m_increase = new UIBitmapButton();
  this->m_increase->SetBitmap("유저인터페이스\\Shortcut\\btn_plus_normal.bmp", 0, 0);
  this->m_increase->SetBitmap("유저인터페이스\\Shortcut\\btn_plus_over.bmp", 1, 0);
  this->m_increase->SetBitmap("유저인터페이스\\Shortcut\\btn_plus_press.bmp", 2, 0);
  this->m_increase->SetBitmap("유저인터페이스\\Shortcut\\btn_plus_disable.bmp", 3, 0);
  this->m_increase->SetSize(this->m_increase->BitmapWidth(), this->m_increase->BitmapHeight());
  this->m_increase->SetId(ID_PLUS);
  UIBitmapButton* increaseButton = this->m_increase;
  increaseButton->SetDisabled(0);
  increaseButton->Invalidate();
  this->m_increase->Move(276, 19);
  this->m_increase->Enable(1);
  this->AddChild(this->m_increase);

  std::fill_n(this->m_itemSlots, 9, nullptr);

  this->m_showItems = new UIBitmapButton();
  this->m_showItems->SetBitmap("유저인터페이스\\Shortcut\\btn_open_itemslot_normal.bmp", 0, 0);
  this->m_showItems->SetBitmap("유저인터페이스\\Shortcut\\btn_open_itemslot_normal.bmp", 1, 0);
  this->m_showItems->SetBitmap("유저인터페이스\\Shortcut\\btn_open_itemslot_press.bmp", 2, 0);
  this->m_showItems->SetSize(this->m_showItems->BitmapWidth(), this->m_showItems->BitmapHeight());
  this->m_showItems->SetId(ID_SHOW_ITEM_SLOTS);
  UIBitmapButton* showItemsButton = this->m_showItems;
  showItemsButton->SetDisabled(1);
  showItemsButton->Invalidate();
  this->m_showItems->Move(247, 33);
  this->m_showItems->Enable(0);
  this->AddChild(this->m_showItems);

  this->m_hideItems = new UIBitmapButton();
  this->m_hideItems->SetBitmap("유저인터페이스\\Shortcut\\btn_close_itemslot_normal.bmp", 0, 0);
  this->m_hideItems->SetBitmap("유저인터페이스\\Shortcut\\btn_close_itemslot_normal.bmp", 1, 0);
  this->m_hideItems->SetBitmap("유저인터페이스\\Shortcut\\btn_close_itemslot_press.bmp", 2, 0);
  this->m_hideItems->SetSize(this->m_hideItems->BitmapWidth(), this->m_hideItems->BitmapHeight());
  this->m_hideItems->SetId(ID_HIDE_ITEM_SLOTS);
  UIBitmapButton* hideItemsButton = this->m_hideItems;
  hideItemsButton->SetDisabled(0);
  hideItemsButton->Invalidate();
  this->m_hideItems->Move(247, 33);
  this->m_hideItems->Enable(1);
  this->AddChild(this->m_hideItems);

  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}
