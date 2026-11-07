#include "UI/Items/UISwapEquipmentWnd.h"

#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/World/World.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Dialogs/UINpcEditDialogWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Window/FuncDrawWNDChar.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>
#include <timeapi.h>

#include <cstdint>
#include <cstring>
#include <new>
#include <string>

// GLOBAL: gl_24874
unsigned long g_swapEquipmentTick;

// OFFSET: 0x0082F560
// FUNCTION: fn_10387
UISwapEquipmentWnd::UISwapEquipmentWnd()
    : UIFrameWnd(0), m_swapButton(nullptr), unknown_00BC(0), unknown_00C0(timeGetTime()), unknown_00D4(-1), unknown_00D8(-1),
      unknown_00DC(-1) {
  this->geometry.unknown_0030 = 0;
}

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

// OFFSET: 0x0082FD90
// FUNCTION: fn_10391
void UISwapEquipmentWnd::DrawEquipmentSlots() {
  this->unknown_00D0 = 0;
  this->unknown_00C4 = 0;
  this->unknown_00C8 = 0;
  this->unknown_00CC = 0;

  for (int slot = 0; slot < 10; ++slot) {
    bool fromSwap = true;
    ITEM_INFO item = this->GetSlotItem(slot);
    if (item.unknown_0000[1] == 0) {
      fromSwap = false;
      if (this->unknown_00BC == 2) {
        item = g_session.GetItem2B50(slot);
      } else {
        item = g_session.GetItem17F0(slot);
      }
    }

    if (item.unknown_0000[1] > 0 && item.quantity == 1) {
      int xCoords[4] = {34, 173, 6, 250};
      int yCoords[4] = {54, 80, 106, 132};

      if (fromSwap) {
        this->SetSlotValue(slot, item.unknown_0070);
        const int row = slot & 3;
        if (slot < 8) {
          const int iconY = row * 26 + 48;
          DrawItemIcon(*this, ResourceMgr::Instance(), xCoords[slot / 4 + 2], iconY, item, 0);
          this->DrawItemName(xCoords[slot / 4], yCoords[row], 70, &item, 0, 0, 12);
          if (item.unknown_0088 > 0) {
            CBitmapRes* gradeBitmap = static_cast<CBitmapRes*>(
              ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.unknown_0088).c_str()))
            );
            this->DrawBitmap(xCoords[slot / 4 + 2], iconY + 12, gradeBitmap, true);
          }
        } else {
          DrawItemIcon(*this, ResourceMgr::Instance(), xCoords[row + 2], 22, item, 0);
          this->DrawItemName(xCoords[slot - 8], 30, 70, &item, 0, 0, 12);
        }
      } else {
        const EquipSlots slots = g_session.GetEquipSlots(item.unknown_0000[3]);
        int locations[3] = {slots.values[2], slots.values[1], slots.values[0]};
        for (int i = 0; i < 3; ++i) {
          fromSwap = this->GetSlotItem(locations[i]).unknown_0000[1] > 0;
        }

        if (!fromSwap) {
          this->SetSlotValue(slot, item.unknown_0070);
          char path[128];
          if (slot < 8) {
            const int iconX = xCoords[slot / 4 + 2];
            std::memset(path, 0, sizeof(path));
            g_session.IconPath(path, item.itemId.c_str(), 1);
            CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
            const int row = slot & 3;
            if (bitmap) {
              this->DrawAuxiliary(1, iconX, row * 26 + 48, &bitmap->image, true, false);
            }
            this->DrawItemName(xCoords[slot / 4], yCoords[row], 70, &item, 0, 0, 12);
            if (item.unknown_0088 > 0) {
              CBitmapRes* gradeBitmap = static_cast<CBitmapRes*>(
                ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.unknown_0088).c_str()))
              );
              this->DrawBitmap(iconX, row * 26 + 60, gradeBitmap, true);
            }
          } else {
            const int iconX = xCoords[(slot & 3) + 2];
            std::memset(path, 0, sizeof(path));
            g_session.IconPath(path, item.itemId.c_str(), 1);
            CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
            if (bitmap) {
              this->DrawAuxiliary(1, iconX, 22, &bitmap->image, true, false);
            }
            this->DrawItemName(xCoords[slot - 8], 30, 70, &item, 0, 0, 12);
          }
        }
      }
    }
  }
}

// OFFSET: 0x008315E0
// FUNCTION: fn_10400
void UISwapEquipmentWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->ItemAtPoint(x, y);
  if (item.unknown_0000[1] > 0) {
    g_windowMgr.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x008316A0
// FUNCTION: fn_10401
int UISwapEquipmentWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    switch (argument1) {
    case 0xB9:
      g_session.m_swapEquipmentMode = 0;
      if (g_windowMgr.unknown_01E0) {
        g_windowMgr.unknown_01E0->SendMsg(nullptr, 6, 0x1FC, 0, 0, 0);
      }
      break;
    case 0xC9:
      g_windowMgr.CloseWindow(0x128);
      return 1;
    case 0x1FC:
      if (g_windowMgr.LookupWindow(10011)) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBB9)), 0xFF, 0, 0, 0);
        return 1;
      }
      if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_0250) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBB7)), 0xFF, 0, 0, 0);
      } else if (g_windowMgr.unknown_0304 || g_windowMgr.unknown_0308 || g_windowMgr.unknown_02FC) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBBA)), 0xFF, 0, 0, 0);
      } else if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->unknown_0264[3]) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBB8)), 0xFF, 0, 0, 0);
      } else if (g_windowMgr.unknown_01F0 || g_windowMgr.unknown_01F8) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBBB)), 0xFF, 0, 0, 0);
      } else {
        g_modeMgr.GetGameMode()->SendMsg(0x130, 0, 0, 0, 0);
      }
      return 1;
    }
    break;
  case 60:
    this->Invalidate();
    this->InvalidateChildren();
    return 1;
  }
  return 1;
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
  this->m_swapButton = new (std::nothrow) UISwapBitmapButton();
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

  UIBasicButton* close = new (std::nothrow) UIBasicButton();
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

// OFFSET: 0x00830A30
// FUNCTION: fn_10396
void UISwapEquipmentWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  std::string background;
  if (this->unknown_00BC == 1) {
    background = "유저인터페이스\\swap_equipment\\equipwin_bg2_change.bmp";
  } else if (this->unknown_00BC == 2) {
    background = "유저인터페이스\\swap_equipment\\equipwin_special_change.bmp";
  }

  const char* resolved = Resolve(background.c_str());
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(resolved));
  if (bitmap) {
    this->DrawBitmap(0, 0, bitmap, false);
  }

  this->DrawCenteredText(140, 4, LookupMessage(3110), std::strlen(LookupMessage(3110)), 0, 12, 0x14234B, 0);

  this->DrawEquipmentSlots();

  FuncDrawWNDChar drawer(
    this,
    this->geometry.width / 2 + 1,
    this->geometry.height - 53,
    g_session.SyncUnknown0E98(),
    static_cast<short>(g_session.m_job),
    g_session.m_job,
    static_cast<short>(g_session.m_bodyCostume),
    static_cast<short>(g_session.m_head),
    static_cast<short>(this->unknown_00C4),
    static_cast<short>(this->unknown_00C8),
    static_cast<short>(this->unknown_00CC),
    this->unknown_00D0,
    0,
    0,
    g_session.m_bodyStyle,
    g_session.m_headPalette,
    0,
    0,
    this->unknown_00C0
  );
  drawer.Draw(false);

  if (g_session.unknown_7EC8 != 0) {
    ITEM_INFO dragItem = LookupClientItemByIdentity(g_session.m_inventoryItems, g_session.unknown_7EC8);
    if (dragItem.unknown_0000[1] > 0) {
      DrawItemIcon(*this, ResourceMgr::Instance(), this->geometry.width / 2 - 24, 27, dragItem, 0);
      this->DrawFittedItemName(this->geometry.width / 2 - 24, 51, &dragItem, 0, 0, 12, 0);
    }
  }
}

// OFFSET: 0x00830E20
// FUNCTION: fn_10397
void UISwapEquipmentWnd::Slot26(int x, int y) {
  if (y < 17) {
    this->SendMsg(this, 6, 0xCA, 0, 0, 0);
  }

  ITEM_INFO item = this->ItemAtPoint(x, y);
  if (item.unknown_0000[1] > 0 && item.quantity > 0) {
    const EquipSlots slots = g_session.GetEquipSlots(item.unknown_0000[3]);
    CMode* mode = g_modeMgr.gameMode;
    if (GetAsyncKeyState(VK_CONTROL)) {
      mode->SendMsg(0x131, reinterpret_cast<int>(&item), 0, 0, 0);
    } else {
      mode->SendMsg(0x12F, slots.values[2], this->unknown_00BC == 2, 0, 0);
    }
  }
}

// OFFSET: 0x00830F40
// FUNCTION: fn_10398
void UISwapEquipmentWnd::Slot25(int x, int y) {
  ITEM_INFO item = this->ItemAtPoint(x, y);
  if (item.unknown_0000[1] <= 0 || item.quantity <= 0) {
    return;
  }

  const EquipSlots slots = g_session.GetEquipSlots(item.unknown_0000[3]);
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  mode->drag.BeginItem(g_windowMgr, this->unknown_00BC == 2 ? 18 : 17, item, item.quantity, slots.values[2]);

  if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
    if (g_windowMgr.GetHoverWindow() && item.identified != 0) {
      if (g_windowMgr.GetHoverWindow()->geometry.id == 492) {
        reinterpret_cast<UINpcTextEditDialogWnd*>(g_windowMgr.unknown_0318)->InsertItemTag(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 490) {
        static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemLink(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 493) {
        reinterpret_cast<UIChatEditCtrl*>(ChatWindow()->m_chatEdit)->InsertItemLink(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 491) {
        reinterpret_cast<UIRodexComposeWnd*>(g_windowMgr.unknown_0458)->InsertItemTag(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 494) {
        static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemLink(item);
      }
    }
  }
}

// OFFSET: 0x00831130
// FUNCTION: fn_10399
void UISwapEquipmentWnd::Slot28(int x, int y) {
  {
    ITEM_INFO item = this->ItemAtPoint(x, y);
    if (item.unknown_0000[1] > 0) {
      int tooltipX = 6;
      int tooltipY = y - (y - 22) % 26 - 19;
      if (x >= 170) {
        tooltipX = 175;
      } else if (x >= 106) {
        tooltipX = this->geometry.width / 2 - 20;
      }
      this->ToScreen(tooltipX, tooltipY);

      std::string text;
      if (item.quantity > 1) {
        if (item.unknown_0098 != 0) {
          text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(0x9F7), item.DisplayName(0)));
        } else {
          text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(0xB7), item.DisplayName(0).c_str(), item.quantity));
        }
      } else if (item.unknown_0098 != 0) {
        text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(0xA28), item.DisplayName(0).c_str(), item.unknown_0098));
      } else {
        text = static_cast<const std::string&>(item.DisplayName(0));
      }

      g_modeMgr.GetGameMode()->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
    }
  }

  const int previousD4 = this->unknown_00D4;
  const int previousD8 = this->unknown_00D8;
  const int previousDC = this->unknown_00DC;
  this->unknown_00D4 = -1;
  this->unknown_00D8 = -1;
  this->unknown_00DC = -1;

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode->drag.source == 1) {
    if (!((x >= 6 && x < 30) || (x >= 251 && x < 275))) {
      return;
    }

    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, mode->drag.id);
    if (item.unknown_0000[1] > 0 && item.unknown_0000[2] != 0) {
      const EquipSlots slots = g_session.GetEquipSlots(item.unknown_0000[2]);
      this->unknown_00D4 = slots.values[2];
      if (slots.values[1] != -1) {
        this->unknown_00D8 = slots.values[1];
      }
      if (slots.values[0] != -1) {
        this->unknown_00DC = slots.values[0];
      }
    }
  }

  if (previousD4 != this->unknown_00D4 || previousD8 != this->unknown_00D8 || previousDC != this->unknown_00DC) {
    this->Invalidate();
  }
}
