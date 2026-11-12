#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/Items/UISwapEquipmentWnd.h"
#include "UI/Window/UIEnums.h"

#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Contents/World/World.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Dialogs/UINpcEditDialogWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Login/FuncDrawWNDChar.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
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
    : UIFrameWnd(0), m_swapButton(nullptr), equipmentPage(0), previewTick(timeGetTime()), hoverSlot(-1), hoverSecondarySlot(-1),
      hoverTertiarySlot(-1) {
  this->geometry.state = 0;
}

// OFFSET: 0x0082F990
// FUNCTION: fn_10390
void UISwapEquipmentWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
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
    item = g_session.GetEquippedItemAt(drag->slot);
    break;
  case 24:
    equipped = 1;
    item = g_session.GetCostumeItemAt(drag->slot);
    break;
  }

  if (item.identity[1] > 0) {
    switch (item.identity[0]) {
    case 10:
    case 16:
    case 17:
    case 19:
      mode->SendMsg(MM_WEAR_SWAP_EQUIP, drag->id, item.identity[2], 0, 0);
      break;
    default:
      if (equipped == 1) {
        mode->SendMsg(MM_SWAP_EQUIPMENT_BY_INDEX, reinterpret_cast<int>(&item), 0, 0, 0);
      } else if (item.identity[2] != 0 && item.identified != 0 && item.identity[3] == 0) {
        mode->SendMsg(MM_WEAR_SWAP_EQUIP, drag->id, item.identity[2], 0, 0);
      }
      break;
    }
  }

  g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x0082FD90
// FUNCTION: fn_10391
void UISwapEquipmentWnd::DrawEquipmentSlots() {
  this->weaponView = 0;
  this->topAccessory = 0;
  this->midAccessory = 0;
  this->bottomAccessory = 0;

  for (int slot = 0; slot < 10; ++slot) {
    bool fromSwap = true;
    ITEM_INFO item = this->GetSlotItem(slot);
    if (item.identity[1] <= 0) {
      fromSwap = false;
      if (this->equipmentPage == 2) {
        item = g_session.GetCostumeItemAt(slot);
      } else {
        item = g_session.GetEquippedItemAt(slot);
      }
    }

    if (item.identity[1] > 0 && item.quantity == 1) {
      int xCoords[4] = {34, 173, 6, 250};
      int yCoords[4] = {54, 80, 106, 132};

      if (fromSwap == true) {
        this->SetSlotValue(slot, item.sprite);
        const int row = slot & 3;
        if (slot < 8) {
          const int iconY = row * 26 + 48;
          DrawItemIcon(*this, ResourceMgr::Instance(), xCoords[slot / 4 + 2], iconY, item, 0);
          this->DrawItemName(xCoords[slot / 4], yCoords[row], 70, &item, 0, 0, 12);
          if (0 < item.grade) {
            this->DrawBitmap(
              xCoords[slot / 4 + 2], iconY + 12,
              static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.grade).c_str()))),
              true
            );
          }
        } else {
          DrawItemIcon(*this, ResourceMgr::Instance(), xCoords[row + 2], 22, item, 0);
          this->DrawItemName(xCoords[slot - 8], 30, 70, &item, 0, 0, 12);
        }
      } else {
        const EquipSlots slots = g_session.GetEquipSlots(item.identity[3]);
        int locations[3] = {slots.values[2], slots.values[1], slots.values[0]};
        for (int i = 0; i < 3; ++i) {
          fromSwap = this->GetSlotItem(locations[i]).identity[1] > 0;
        }

        if (fromSwap != true) {
          this->SetSlotValue(slot, item.sprite);
          char path[128];
          if (slot < 8) {
            const char* id = item.itemId.c_str();
            const int iconX = xCoords[slot / 4 + 2];
            std::memset(path, 0, sizeof(path));
            g_session.IconPath(path, id, 1);
            CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
            const int row = slot & 3;
            if (bitmap) {
              this->DrawAuxiliary(1, iconX, row * 26 + 48, &bitmap->image, true, false);
            }
            this->DrawItemName(xCoords[slot / 4], yCoords[row], 70, &item, 0, 0, 12);
            if (0 < item.grade) {
              this->DrawBitmap(
                iconX, row * 26 + 60,
                static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.grade).c_str()))),
                true
              );
            }
          } else {
            const char* id = item.itemId.c_str();
            const int iconX = xCoords[(slot & 3) + 2];
            std::memset(path, 0, sizeof(path));
            g_session.IconPath(path, id, 1);
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
void UISwapEquipmentWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->ItemAtPoint(x, y);
  if (item.identity[1] > 0) {
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x008316A0
// FUNCTION: fn_10401
int UISwapEquipmentWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_COMMAND:
    switch (argument1) {
    case ID_CANCEL:
      g_session.m_swapEquipmentMode = 0;
      if (g_windowMgr.equipWnd) {
        g_windowMgr.equipWnd->SendMsg(nullptr, UM_COMMAND, 0x1FC, 0, 0, 0);
      }
      break;
    case ID_CLOSE:
      g_windowMgr.CloseWindow(0x128);
      return 1;
    case 0x1FC:
      if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SWAP_EQUIPITEM_FAIL_CHAT)), 0xFF, 0, 0, 0);
        return 1;
      }
      if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->isPlayerDead) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SWAP_EQUIPITEM_FAIL_MERCHANTSHOP)), 0xFF, 0, 0, 0);
        return 1;
      }
      if (g_windowMgr.merchantItemMyShopWnd) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MDUNGEON_CREATE_FAIL_PARTY_ONLINE)), 0xFF, 0, 0, 0);
        return 1;
      }
      if (g_windowMgr.merchantItemPurchaseWnd) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MDUNGEON_CREATE_FAIL_PARTY_ONLINE)), 0xFF, 0, 0, 0);
        return 1;
      }
      if (g_windowMgr.merchantShopMakeWnd) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MDUNGEON_CREATE_FAIL_PARTY_ONLINE)), 0xFF, 0, 0, 0);
        return 1;
      }
      if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->actorWindows[3]) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SWAP_EQUIPITEM_FAIL_USE_NPC)), 0xFF, 0, 0, 0);
        return 1;
      }
      if (g_windowMgr.sayDialogWnd || g_windowMgr.choose3Wnd) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_USESKILL_FAIL_NEED_MORE_ARROW_CNT)), 0xFF, 0, 0, 0);
        return 1;
      }
      g_modeMgr.GetGameMode()->SendMsg(MM_SWAP_EQUIPMENT, 0, 0, 0, 0);
      return 1;
    }
    break;
  case UM_REFRESH:
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
    this->midAccessory = value;
    break;
  case 9:
    this->bottomAccessory = value;
    break;
  case 0:
    this->topAccessory = value;
    break;
  case 2:
    this->weaponView = value;
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

  if (this->equipmentPage == 2) {
    ITEM_INFO item = g_session.GetSwapEquippedItem(slot);
    return item.identity[1] > 0 ? item : ITEM_INFO(0);
  }

  ITEM_INFO item = g_session.GetSwapCostumeItem(slot);
  return item.identity[1] > 0 ? item : ITEM_INFO(0);
}

// OFFSET: 0x00830760
// FUNCTION: fn_10394
void UISwapEquipmentWnd::OnCreate(int width, int height) {
  this->m_swapButton = new (std::nothrow) UISwapBitmapButton;
  this->m_swapButton->SetBitmap("유저인터페이스\\swap_equipment\\btn_change2_normal.bmp", 0, 0);
  this->m_swapButton->SetBitmap("유저인터페이스\\swap_equipment\\btn_change2_over.bmp", 1, 0);
  this->m_swapButton->SetBitmap("유저인터페이스\\swap_equipment\\btn_change2_press.bmp", 2, 0);
  this->m_swapButton->SetSize(this->m_swapButton->BitmapWidth(), this->m_swapButton->BitmapHeight());
  this->m_swapButton->SetLabel(LookupMessage(MSI_DESC_VIT2));
  this->m_swapButton->m_textColor = 0x172842;
  this->m_swapButton->Move(0x51, 0x9B);
  this->m_swapButton->SetId(0x1FC);
  this->AddChild(this->m_swapButton);

  UIBasicButton* close = new (std::nothrow) UIBasicButton();
  close->Configure(0x32, LookupMessage(MSI_CANCEL));
  close->Move(0xE0, 0x9B);
  close->SetId(0xB9);
  this->AddChild(close);
}

// OFFSET: 0x00830970
// FUNCTION: fn_10395
void UISwapBitmapButton::Paint() {
  UIBitmapButton::Paint();
  const unsigned long elapsed = timeGetTime() - g_swapEquipmentTick;
  this->SetBitmap(elapsed >= 10000 ? "유저인터페이스\\swap_equipment\\btn_change2_normal.bmp" : "유저인터페이스\\swap_equipment\\btn_change2_disable.bmp", 0, 0);
  this->SetBitmap(elapsed >= 10000 ? "유저인터페이스\\swap_equipment\\btn_change2_over.bmp" : "유저인터페이스\\swap_equipment\\btn_change2_disable.bmp", 1, 0);
  this->SetBitmap(elapsed >= 10000 ? "유저인터페이스\\swap_equipment\\btn_change2_press.bmp" : "유저인터페이스\\swap_equipment\\btn_change2_disable.bmp", 2, 0);
  this->m_textColor = elapsed < 10000 ? 0x969696 : 0x172842;
  this->DrawCenteredText(this->geometry.width / 2, this->geometry.height / 2 - 5, this->label.c_str(), this->label.size(), 0, 12, this->m_textColor, 0);
}

// OFFSET: 0x00830A30
// FUNCTION: fn_10396
void UISwapEquipmentWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  std::string background;
  if (this->equipmentPage == 1) {
    background = "유저인터페이스\\swap_equipment\\equipwin_bg2_change.bmp";
  } else if (this->equipmentPage == 2) {
    background = "유저인터페이스\\swap_equipment\\equipwin_special_change.bmp";
  }

  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background.c_str())));
  if (bitmap) {
    this->DrawBitmap(0, 0, bitmap, false);
  }

  this->DrawCenteredText(140, 4, LookupMessage(MSI_DESC_AGI2), std::strlen(LookupMessage(MSI_DESC_AGI2)), 0, 12, 0x14234B, 0);

  this->DrawEquipmentSlots();

  FuncDrawWNDChar drawer(
    this,
    this->geometry.width / 2 + 1,
    this->geometry.height - 53,
    g_session.GetCurrentSex(),
    static_cast<short>(g_session.m_job),
    g_session.m_job,
    static_cast<short>(g_session.m_bodyCostume),
    static_cast<short>(g_session.m_head),
    static_cast<short>(this->topAccessory),
    static_cast<short>(this->midAccessory),
    static_cast<short>(this->bottomAccessory),
    this->weaponView,
    0,
    0,
    g_session.m_bodyStyle,
    g_session.m_headPalette,
    0,
    0,
    this->previewTick
  );
  drawer.Draw(false);

  if (g_session.swapArrowIndex != 0) {
    ITEM_INFO dragItem = LookupClientItemByIdentity(g_session.m_inventoryItems, g_session.swapArrowIndex);
    if (dragItem.identity[1] > 0) {
      DrawItemIcon(*this, ResourceMgr::Instance(), this->geometry.width / 2 - 24, 27, dragItem, 0);
      this->DrawFittedItemName(this->geometry.width / 2 - 24, 51, &dragItem, 0, 0, 12, 0);
    }
  }
}

// OFFSET: 0x00830E20
// FUNCTION: fn_10397
void UISwapEquipmentWnd::OnLBtnDblClk(int x, int y) {
  if (y < 17) {
    this->SendMsg(this, UM_COMMAND, ID_MINI, 0, 0, 0);
  }

  ITEM_INFO item = this->ItemAtPoint(x, y);
  if (item.identity[1] > 0 && item.quantity > 0) {
    const EquipSlots slots = g_session.GetEquipSlots(item.identity[3]);
    CMode* mode = g_modeMgr.gameMode;
    if (GetAsyncKeyState(VK_CONTROL)) {
      mode->SendMsg(MM_SWAP_EQUIPMENT_BY_INDEX, reinterpret_cast<int>(&item), 0, 0, 0);
    } else {
      mode->SendMsg(MM_TAKE_OFF_SWAP_EQUIP, slots.values[2], this->equipmentPage == 2, 0, 0);
    }
  }
}

// OFFSET: 0x00830F40
// FUNCTION: fn_10398
void UISwapEquipmentWnd::OnLBtnDown(int x, int y) {
  ITEM_INFO item = this->ItemAtPoint(x, y);
  if (item.identity[1] <= 0 || item.quantity <= 0) {
    return;
  }

  const EquipSlots slots = g_session.GetEquipSlots(item.identity[3]);
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  mode->drag.BeginItem(g_windowMgr, this->equipmentPage == 2 ? 18 : 17, item, item.quantity, slots.values[2]);

  if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
    if (g_windowMgr.GetHoverWindow() && item.identified != 0) {
      if (g_windowMgr.GetHoverWindow()->geometry.id == 492) {
        reinterpret_cast<UINpcTextEditDialogWnd*>(g_windowMgr.npcTextEditDialogWnd)->InsertItemTag(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 490) {
        static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemLink(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 493) {
        reinterpret_cast<UIChatEditCtrl*>(ChatWindow()->m_chatEdit)->InsertItemLink(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 491) {
        reinterpret_cast<UIRodexComposeWnd*>(g_windowMgr.rodexComposeWnd)->InsertItemTag(item);
      } else if (g_windowMgr.GetHoverWindow()->geometry.id == 494) {
        static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemLink(item);
      }
    }
  }
}

// OFFSET: 0x00831130
// FUNCTION: fn_10399
void UISwapEquipmentWnd::OnMouseHover(int x, int y) {
  {
    ITEM_INFO item = this->ItemAtPoint(x, y);
    if (item.identity[1] > 0) {
      int tooltipX = 6;
      int tooltipY = y - (y - 22) % 26 - 19;
      if (x >= 170) {
        tooltipX = 175;
      } else if (x >= 106) {
        tooltipX = this->geometry.width / 2 - 20;
      }
      this->ToScreen(tooltipX, tooltipY);

      std::string text;
      if (item.quantity <= 1) {
        if (item.optionCount != 0) {
          text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_FAILED_MAIL_LIMIT_CNT), item.DisplayName(0).c_str(), item.optionCount));
        } else {
          text = item.DisplayName(0);
        }
      } else if (item.optionCount != 0) {
        text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_FAIL_NEED_EQUIPPED_GUN_GATLING), item.DisplayName(0)));
      } else {
        text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_EA2), item.DisplayName(0).c_str(), item.quantity));
      }

      g_modeMgr.GetGameMode()->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
    }
  }

  const int previousD4 = this->hoverSlot;
  const int previousD8 = this->hoverSecondarySlot;
  const int previousDC = this->hoverTertiarySlot;
  this->hoverSlot = -1;
  this->hoverSecondarySlot = -1;
  this->hoverTertiarySlot = -1;

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (mode->drag.source == 1) {
    if (!((x >= 6 && x < 30) || (x >= 251 && x < 275))) {
      return;
    }

    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, mode->drag.id);
    if (item.identity[1] > 0 && item.identity[2] != 0) {
      const EquipSlots slots = g_session.GetEquipSlots(item.identity[2]);
      this->hoverSlot = slots.values[2];
      if (slots.values[1] != -1) {
        this->hoverSecondarySlot = slots.values[1];
      }
      if (slots.values[0] != -1) {
        this->hoverTertiarySlot = slots.values[0];
      }
    }
  }

  if (previousD4 != this->hoverSlot || previousD8 != this->hoverSecondarySlot || previousDC != this->hoverTertiarySlot) {
    this->Invalidate();
  }
}
