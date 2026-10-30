#include "UI/Items/UISwapEquipmentWnd.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>
#include <timeapi.h>

#include <cstdint>
#include <new>
#include <string>

// GLOBAL: gl_24874
unsigned long g_swapEquipmentTick;

// OFFSET: 0x0082F990
// FUNCTION: fn_10390
void UISwapEquipmentWnd::Slot10(int x, int y, std::intptr_t value) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  ITEM_INFO item(0);
  unsigned char equipped = 0;
  switch (drag->source) {
  case 1:
    item = LookupClientItemByIdentity(g_session.m_inventoryItems, drag->id);
    break;
  case 2:
    equipped = 1;
    item = g_session.GetItem17F0(drag->slot);
    break;
  case 24:
    equipped = 1;
    item = g_session.GetItem2B50(drag->slot);
    break;
  }

  if (item.unknown_0000[1] > 0) {
    switch (item.unknown_0000[0]) {
    case 10:
    case 16:
    case 17:
    case 19:
      mode->SendMsg(302, drag->id, item.unknown_0000[2], 0, 0);
      break;
    default:
      if (equipped == 1) {
        mode->SendMsg(305, reinterpret_cast<int>(&item), 0, 0, 0);
      } else if (item.unknown_0000[2] != 0 && item.identified != 0 && item.unknown_0000[3] == 0) {
        mode->SendMsg(302, drag->id, item.unknown_0000[2], 0, 0);
      }
      break;
    }
  }

  g_modeMgr.gameMode->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x00831860
// FUNCTION: fn_10402
void UISwapEquipmentWnd::SetSlotValue(int index, int value) {
  switch (index) {
  case 8:
    this->unknown_00C8 = value;
    break;
  case 9:
    this->unknown_00CC = value;
    break;
  case 0:
    this->unknown_00C4 = value;
    break;
  case 2:
    this->unknown_00D0 = value;
    break;
  }
}

// OFFSET: 0x00830400
// FUNCTION: fn_10392
ITEM_INFO UISwapEquipmentWnd::ItemAtPoint(int x, int y) {
  int slot = -1;
  if (y <= this->geometry.height && y >= 22 && y <= 151) {
    if ((x >= 6 && x < 106) || (x >= 170 && x < this->geometry.width - 6)) {
      const int row = (y - 22) / 26;
      if (x >= 170) {
        slot = row == 0 ? 9 : row + 3;
      } else {
        slot = row == 0 ? 8 : row - 1;
      }
    }

    if (x > 112 && x < 170 && y >= 28 && y <= 52) {
      slot = 15;
    }
  }

  return this->GetSlotItem(slot);
}

// OFFSET: 0x008304F0
// FUNCTION: fn_10393
ITEM_INFO UISwapEquipmentWnd::GetSlotItem(int slot) {
  if (slot == -1) {
    return ITEM_INFO(0);
  }

  if (this->unknown_00BC == 2) {
    ITEM_INFO item = g_session.GetItem6B68(slot);
    return item.unknown_0000[1] > 0 ? item : ITEM_INFO(0);
  }

  ITEM_INFO item = g_session.GetItem61B8(slot);
  return item.unknown_0000[1] > 0 ? item : ITEM_INFO(0);
}

// OFFSET: 0x00830760
// FUNCTION: fn_10394
void UISwapEquipmentWnd::Slot15(int width, int height) {
  this->m_swapButton = new (std::nothrow) UISwapBitmapButton(*this->m_context, *this->m_controls);
  this->m_swapButton->SetBitmap("유저인터페이스\\swap_equipment\\btn_change2_normal.bmp", 0, 0);
  this->m_swapButton->SetBitmap("유저인터페이스\\swap_equipment\\btn_change2_over.bmp", 1, 0);
  this->m_swapButton->SetBitmap("유저인터페이스\\swap_equipment\\btn_change2_press.bmp", 2, 0);
  this->m_swapButton->SetSize(this->m_swapButton->BitmapWidth(), this->m_swapButton->BitmapHeight());
  UISwapBitmapButton* button = this->m_swapButton;
  const std::string label = LookupMessage(0xC27);
  button->label = label;
  this->m_swapButton->m_textColor = 0x172842;
  this->m_swapButton->Move(0x51, 0x9B);
  this->m_swapButton->SetId(0x1FC);
  this->AddChild(this->m_swapButton);

  UIBasicButton* close = new (std::nothrow) UIBasicButton(*this->m_context, *this->m_controls);
  close->Configure(0x32, LookupMessage(0xDA));
  close->Move(0xE0, 0x9B);
  close->SetId(0xB9);
  this->AddChild(close);
}

// OFFSET: 0x00830970
// FUNCTION: fn_10395
void UISwapBitmapButton::Paint() {
  UIBitmapButton::Paint();
  const unsigned long elapsed = timeGetTime() - g_swapEquipmentTick;
  this->SetBitmap(elapsed < 10000 ? "유저인터페이스\\swap_equipment\\btn_change2_disable.bmp" : "유저인터페이스\\swap_equipment\\btn_change2_normal.bmp", 0, 0);
  this->SetBitmap(elapsed < 10000 ? "유저인터페이스\\swap_equipment\\btn_change2_disable.bmp" : "유저인터페이스\\swap_equipment\\btn_change2_over.bmp", 1, 0);
  this->SetBitmap(elapsed < 10000 ? "유저인터페이스\\swap_equipment\\btn_change2_disable.bmp" : "유저인터페이스\\swap_equipment\\btn_change2_press.bmp", 2, 0);
  this->m_textColor = elapsed < 10000 ? 0x969696 : 0x172842;
  this->DrawCenteredText(this->geometry.width / 2, this->geometry.height / 2 - 5, this->label.c_str(), this->label.size(), 0, 12, this->m_textColor, 0);
}
