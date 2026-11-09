#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/UIGradeEnchantWnd.h"
#include "Client/Mode/CMode.h"

#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/ModeResultEnums.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <cstring>

#include <windows.h>
#include <shellapi.h>
#include <timeapi.h>

#pragma pack(push, 1)
struct GradeEnchantSelectPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint16_t index;
};

struct GradeEnchantRequestPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint16_t index;
  /* 0004 */ int material;
  /* 0008 */ unsigned char useBlessing;
  /* 0009 */ int blessingCount;
  /* 000D */ unsigned char protect;
};

struct GradeEnchantClosePacket {
  /* 0000 */ std::uint16_t id;
};
#pragma pack(pop)

// INLINED
static inline int MaterialRowAt(int x, int y) {
  if (x < 158 || x > 393) {
    return -1;
  }

  if (y < 102 || y > 219) {
    return -1;
  }

  const int row = (y - 102) / 40;
  if (y > row * 40 + 139) {
    return -1;
  }

  return row >= 3 ? -1 : row;
}

// OFFSET: 0x007E68F0
// FUNCTION: fn_09738
UIGradeEnchantWnd::UIGradeEnchantWnd()
    : UIFrameWnd(0), enhancedUpButton(nullptr), state(0),
      result(-1), itemFrameInterval(200), wheelFrameInterval(100), stateTick(0), wheelTick(0), itemFrame(1), wheelFrame(1), resultTick(0),
      selectedMaterial(-1), hoverMaterial(-1), materials(), info{}, enchantOption(0), enchantOptionCount(0), protect(0), blessingAmount(0), baseSuccessRate(0),
      messageIds(), item(0), requestPending(0), startButton(nullptr), enhancedDownButton(nullptr) {
}

// OFFSET: 0x007E6A60
// FUNCTION: fn_09739
UIGradeEnchant_BtnEnhancedDown::UIGradeEnchant_BtnEnhancedDown() : UIBitmapButton() {
}

// OFFSET: 0x007E6AC0
// FUNCTION: fn_09740
UIGradeEnchant_BtnEnhancedUp::UIGradeEnchant_BtnEnhancedUp()
    : UIBitmapButton(), tooltipValue(0), tooltipEnabled(0) {
}

// OFFSET: 0x007E6B30
// FUNCTION: fn_09741
UIGradeEnchant_BtnStart::UIGradeEnchant_BtnStart() : UIBitmapButton() {
}

// OFFSET: 0x007E7500
// FUNCTION: fn_09749
void UIGradeEnchantWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (x >= 53 && x <= 99 && y >= 114 && y <= 160 && drag->source == 1) {
    this->SendMsg(this, UM_REFRESHITEM, drag->id, 0, 0, 0);
  }

  g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x007E7560
// FUNCTION: fn_09750
void UIGradeEnchantWnd::ResetItem() {
  this->requestPending = 0;
  this->itemFrame = 1;
  this->wheelFrame = 1;
  this->item = ITEM_INFO(0);

  this->materials.clear();
  this->selectedMaterial = -1;
  this->info = GradeEnchantInfo{};
  this->enchantOption = 0;
  this->enchantOptionCount = 0;
  this->blessingAmount = 0;
  this->protect = 0;
  this->baseSuccessRate = 0;
  this->resultTick = 0;

  this->SetResult(0, -1);
  this->Invalidate();
}

// OFFSET: 0x007E76A0
// FUNCTION: fn_09751
int UIGradeEnchantWnd::SelectedMaterialZeny() {
  if (this->item.identity[1] > 0 && this->materials.size() > 0 && this->selectedMaterial >= 0 &&
      this->materials.size() > static_cast<unsigned>(this->selectedMaterial)) {
    return this->materials[this->selectedMaterial].zeny;
  }

  return 0;
}

// OFFSET: 0x007E7740
// FUNCTION: fn_09752
int UIGradeEnchantWnd::SuccessRate() {
  if (this->item.identity[1] <= 0) {
    return 0;
  }

  int rate = this->baseSuccessRate;
  if (this->blessingAmount > 0) {
    rate += this->blessingAmount / this->info.blessingStepAmount * this->info.bonusPerStep;
  }

  return rate > 100 ? 100 : rate;
}

// OFFSET: 0x007E7780
// FUNCTION: fn_09753
void UIGradeEnchantWnd::OnCreate(int, int) {
  if (UIBitmapButton* close = this->AddCloseButton("", 0)) {
    close->Move(0x180, 0x14);
  }

  auto* title = new UIBasicButton();
  title->Configure(0x2A, LookupMessage(MSI_BOOK_CLOSE));
  title->Move(0x159, 299);
  title->SetId(0xB9);
  this->AddChild(title);

  UIGradeEnchant_BtnStart* start = new UIGradeEnchant_BtnStart();
  this->startButton = start;
  start->SetBitmap("유저인터페이스\\grade_enchant\\btn_start.bmp", 0, 0);
  start->SetBitmap("유저인터페이스\\grade_enchant\\btn_start_over.bmp", 1, 0);
  start->SetBitmap("유저인터페이스\\grade_enchant\\btn_start_press.bmp", 2, 0);
  start->SetBitmap("유저인터페이스\\grade_enchant\\btn_start_disable.bmp", 3, 0);
  start->Resize(start->BitmapWidth(), start->BitmapHeight());
  this->startButton->Move(0x23, 0x123);
  this->startButton->SetId(0xB8);
  this->startButton->SetDisabled(0);
  this->startButton->Invalidate();
  this->AddChild(this->startButton);

  UIGradeEnchant_BtnEnhancedUp* enhancedUp = new UIGradeEnchant_BtnEnhancedUp();
  this->enhancedUpButton = enhancedUp;
  enhancedUp->SetBitmap("유저인터페이스\\grade_enchant\\btn_chance_up.bmp", 0, 0);
  enhancedUp->SetBitmap("유저인터페이스\\grade_enchant\\btn_chance_up_over.bmp", 1, 0);
  enhancedUp->SetBitmap("유저인터페이스\\grade_enchant\\btn_chance_up_pick.bmp", 2, 0);
  enhancedUp->Resize(enhancedUp->BitmapWidth(), enhancedUp->BitmapHeight());
  this->enhancedUpButton->Move(0x9E, 0xE1);
  this->enhancedUpButton->SetId(0x117);
  this->enhancedUpButton->SetDisabled(0);
  this->enhancedUpButton->Invalidate();
  this->AddChild(this->enhancedUpButton);

  UIGradeEnchant_BtnEnhancedDown* enhancedDown = new UIGradeEnchant_BtnEnhancedDown();
  this->enhancedDownButton = enhancedDown;
  enhancedDown->SetBitmap("유저인터페이스\\grade_enchant\\btn_chance_down.bmp", 0, 0);
  enhancedDown->SetBitmap("유저인터페이스\\grade_enchant\\btn_chance_down_over.bmp", 1, 0);
  enhancedDown->SetBitmap("유저인터페이스\\grade_enchant\\btn_chance_down_pick.bmp", 2, 0);
  enhancedDown->Resize(enhancedDown->BitmapWidth(), enhancedDown->BitmapHeight());
  this->enhancedDownButton->Move(0x9E, 0xF0);
  this->enhancedDownButton->SetId(0x118);
  this->enhancedDownButton->SetDisabled(0);
  this->enhancedDownButton->Invalidate();
  this->AddChild(this->enhancedDownButton);

  auto* help = new UIBitmapButton();
  help->SetBitmap("유저인터페이스\\basic_interface\\btn_help_normal.bmp", 0, 0);
  help->SetBitmap("유저인터페이스\\basic_interface\\btn_help_over.bmp", 1, 0);
  help->SetBitmap("유저인터페이스\\basic_interface\\btn_help_pressed.bmp", 2, 0);
  help->SetBitmap("유저인터페이스\\basic_interface\\btn_help_disabled.bmp", 3, 0);
  help->SetSize(help->BitmapWidth(), help->BitmapHeight());
  help->Move(0x8C, 100);
  help->SetTooltip(LookupMessage(MSI_PROBABILITY_TITLE));
  help->SetId(0x1FE);
  this->AddChild(help);
}

// OFFSET: 0x007E7BA0
// FUNCTION: fn_09754
void UIGradeEnchantWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\grade_enchant\\bg_grade_enchant.bmp"))), false);

  this->DrawBitmap(158, 142, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\btn_material.bmp").c_str()))), false);
  this->DrawBitmap(158, 182, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\btn_material.bmp").c_str()))), false);
  this->DrawBitmap(158, 102, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\btn_material.bmp").c_str()))), false);

  if (this->state == 0) {
    this->DrawBitmap(152, 46, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\caution.bmp").c_str()))), true);
    this->DrawBitmap(152, 62, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\caution.bmp").c_str()))), true);
    this->DrawBitmap(152, 78, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\caution.bmp").c_str()))), true);

    this->DrawAlignedText(166, 46, LookupMessage(MSI_HOTKEY_RESET), 228, 0, 0x00A0ADC, 0, 0, 11, 1, 0);
    this->DrawAlignedText(166, 62, LookupMessage(MSI_HOTKEY_OLD_VER), 228, 0, 0x5AEB00, 0, 0, 11, 1, 0);
    this->DrawAlignedText(166, 78, LookupMessage(MSI_EXP_BAR_BASE_LV), 228, 0, 0x5AEB00, 0, 0, 11, 1, 0);
  }

  if (this->state == 1 || this->state == 2) {
    this->DrawBitmap(192, 225, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\btn_unbroken.bmp").c_str()))), false);
    this->DrawBitmap(24, 54, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\bg_gradeitem%d.bmp", this->itemFrame).c_str()))), false);

    if (this->hoverMaterial >= 0 && this->hoverMaterial != this->selectedMaterial) {
      this->DrawBitmap(158, this->hoverMaterial * 40 + 102, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\btn_material_over.bmp").c_str()))), false);
    }

    if (this->selectedMaterial >= 0) {
      this->DrawBitmap(158, this->selectedMaterial * 40 + 102, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\btn_material_pick.bmp").c_str()))), false);
    }

    char description[260];
    std::memset(description, 0, sizeof(description));
    int capacity = sizeof(description);

    int y = 118;
    for (const GradeEnchantMaterial& material : this->materials) {
      ITEM_INFO itemInfo;
      itemInfo.SetItemId(material.itemId);
      itemInfo.GetIdDisplayName(description, capacity, 0);
      DrawItemIcon(*this, ResourceMgr::Instance(), 170, y - 9, itemInfo, 0);
      std::string text = FormatOwnedString(LookupMessage(MSI_NO_ITEM_FOR_EXCHANGE), description, material.amount);
      this->DrawAlignedText(203, y, text, 180, 0, 0, 0, 0, 11, 0, 0);
      y += 40;
    }

    if (this->info.blessingItemId > 0) {
      ITEM_INFO itemInfo;
      itemInfo.SetItemId(this->info.blessingItemId);
      itemInfo.GetIdDisplayName(description, capacity, 0);
      DrawItemIcon(*this, ResourceMgr::Instance(), 195, 228, itemInfo, 0);
      std::string text = FormatOwnedString(LookupMessage(MSI_NO_ITEM_FOR_EXCHANGE), description, this->blessingAmount);
      this->DrawAlignedText(228, 237, text, 160, 0, 0xFFFFFF, 0, 0, 11, 1, 1);
    }

    this->UpdateEnchantMessages();

    const int count = static_cast<int>(this->messageIds.size());
    for (int i = 0; i < 3; ++i) {
      if (i < count) {
        const int itemY = 46 + i * 16;
        this->DrawBitmap(152, itemY, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\caution.bmp").c_str()))), true);
        const int msgId = this->messageIds[i];
        if (msgId == 3711 || msgId == 3822) {
          this->DrawAlignedText(166, itemY, LookupMessage(msgId), 228, 0, 0x5AEB00, 0, 0, 11, 1, 0);
        } else {
          this->DrawAlignedText(166, itemY, LookupMessage(msgId), 228, 0, 0x00A0ADC, 0, 0, 11, 1, 0);
        }
      }
    }

    std::string zeny = FormatOwnedString("%d", this->SelectedMaterialZeny());
    this->DrawAlignedText(307, 303, zeny, 120, 2, 0, 0, 0, 16, 1, 0);
  }

  if (this->state == 2) {
    this->DrawBitmap(56, 268, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\gradewheel%d.bmp", this->wheelFrame).c_str()))), true);
  } else {
    this->DrawBitmap(56, 268, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\gradewheel%d.bmp", 1).c_str()))), true);
  }

  if (this->item.identity[1] > 0) {
    DrawItemIcon(*this, ResourceMgr::Instance(), 64, 125, this->item, 0);
    if (this->item.grade > 0) {
      this->DrawBitmap(64, 137, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", this->item.grade).c_str()))), true);
    }
  }

  if (this->state == 1 || this->state == 2) {
    std::string rate = FormatOwnedString("%d%%", this->SuccessRate());
    this->DrawAlignedText(39, 88, rate, 74, 6, 0xFFFFFF, 0, 29, 28, 1, 1);
  }
}

// OFFSET: 0x007E8960
// FUNCTION: fn_09755
void UIGradeEnchant_BtnEnhancedDown::Paint() {
  UIBitmapButton::Paint();
}

// OFFSET: 0x007E8970
// FUNCTION: fn_09756
void UIGradeEnchant_BtnEnhancedUp::Paint() {
  UIBitmapButton::Paint();
}

// OFFSET: 0x007E8980
// FUNCTION: fn_09757
void UIGradeEnchant_BtnStart::Paint() {
  UIBitmapButton::Paint();
}

// OFFSET: 0x007E8990
// FUNCTION: fn_09758
void UIGradeEnchantWnd::OnLBtnDblClk(int x, int y) {
  if (this->item.identity[1] > 0 && x >= 53 && x <= 99 && y >= 114 && y <= 160) {
    this->ResetItem();
  }
}

// OFFSET: 0x007E89C0
// FUNCTION: fn_09759
void UIGradeEnchantWnd::OnLBtnDown(int x, int y) {
  if (y < 38 && x > 141) {
    UIFrameWnd::OnLBtnDown(x, y);
    return;
  }

  if (this->state == 1 && this->item.identity[1] > 0 && x >= 64 && x <= 110 && y >= 125 && y <= 171) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(g_windowMgr, 33, this->item, 1, -1);
  }
}

// OFFSET: 0x007E8A30
// FUNCTION: fn_09760
void UIGradeEnchantWnd::OnLBtnUp(int x, int y) {
  if (y < 80 && x > 141) {
    UIFrameWnd::OnLBtnUp(x, y);
    return;
  }

  const int row = MaterialRowAt(x, y);
  if (row < 0 || static_cast<unsigned>(row) >= this->materials.size()) {
    return;
  }

  if (this->selectedMaterial != row) {
    if (this->blessingAmount > 0) {
      g_windowMgr.Confirm(LookupMessage(MSI_ENCHANT_TAB_PERFECT), 0, true, false, 0, "Message", 280, 120, 0);
    }

    this->selectedMaterial = row;
    this->blessingAmount = 0;
    this->protect = 0;
  }

  this->Invalidate();
}

// OFFSET: 0x007E8B30
// FUNCTION: fn_09761
void UIGradeEnchantWnd::OnMouseHover(int x, int y) {
  this->hoverMaterial = MaterialRowAt(x, y);
}

// OFFSET: 0x007E8BB0
// FUNCTION: fn_09762
void UIGradeEnchant_BtnEnhancedUp::OnMouseHover(int x, int y) {
  if (this->tooltipValue == 0 || this->tooltipEnabled == 0) {
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  if (mode == nullptr) {
    return;
  }

  this->m_tooltip = FormatOwnedString(LookupMessage(MSI_JOB_WINDHAWK), this->tooltipValue);

  int screenX = 0;
  int screenY = -20;
  this->ToScreen(screenX, screenY);

  mode->ShowTooltip(this->m_tooltip.c_str(), screenX, screenY, -1, false, true);
}

// OFFSET: 0x007E8CC0
// FUNCTION: fn_09763
void UIGradeEnchantWnd::OnProcess() {
  const int frameCounts[3] = {6, 6, 5};
  if (this->itemFrame >= frameCounts[this->state]) {
    return;
  }

  const DWORD now = timeGetTime();
  if (now > this->itemFrameInterval + this->stateTick) {
    if (++this->itemFrame >= frameCounts[this->state]) {
      this->itemFrame = 1;
    }
    this->stateTick = timeGetTime();
    this->Invalidate();
  }

  if (now > this->wheelFrameInterval + this->wheelTick) {
    if (++this->wheelFrame >= 4) {
      this->wheelFrame = 1;
    }
    this->wheelTick = timeGetTime();
    this->Invalidate();
  }

  if (this->state == 2 && now >= this->resultTick + 3000) {
    switch (this->result) {
    case ENCHANTGRADE_UPGRADE_SUCCESS:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_GRADE_ENCHANT_FAIL_BREAK)), 0xFFFF00, 0, 0, 0);
      break;
    case ENCHANTGRADE_UPGRADE_FAILED:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_GRADE_ENCHANT_FAIL_PROTECT)), 0xFFFF00, 0, 0, 0);
      break;
    case ENCHANTGRADE_UPGRADE_DOWNGRADE:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GRADE_ENCHANT_SOURCE_NOT_SELECT)), 0xFFFF00, 0, 0, 0);
      break;
    case ENCHANTGRADE_UPGRADE_BREAK:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GRADE_ENCHANT_SOURCE_ENOUGH)), 0xFFFF00, 0, 0, 0);
      break;
    case ENCHANTGRADE_UPGRADE_PROTECTED:
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GRADE_ENCHANT_MONEY_ENOUGH)), 0xFFFF00, 0, 0, 0);
      break;
    }
    this->ResetItem();
  }

  const bool disabled = this->requestPending == 1 || this->item.identity[1] == 0 || this->selectedMaterial == -1 ||
                        LookupClientItem(g_session.m_inventoryItems, this->materials[this->selectedMaterial].itemId, 0).quantity <
                          this->materials[this->selectedMaterial].amount ||
                        g_session.m_zeny < this->SelectedMaterialZeny() || this->baseSuccessRate == 0;

  if (disabled) {
    this->startButton->SetDisabled(1);
    this->startButton->Invalidate();
    this->startButton->SetState(3);
  } else {
    this->startButton->SetDisabled(0);
    this->startButton->Invalidate();
    this->startButton->SetState(0);
  }
}

// OFFSET: 0x007E8FA0
// FUNCTION: fn_09764
void UIGradeEnchantWnd::OnRBtnDown(int x, int y) {
}

// OFFSET: 0x007E8FB0
// FUNCTION: fn_09765
void UIGradeEnchantWnd::OnRBtnUp(int x, int y) {
  if (y < 80 && x > 141) {
    return;
  }

  if (this->state == 1) {
    const int row = MaterialRowAt(x, y);
    if (row >= 0 && static_cast<unsigned>(row) < this->materials.size()) {
      UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
      if (window) {
        ITEM_INFO material = LookupClientItem(g_session.m_inventoryItems, this->materials[row].itemId, 0);
        window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&material), 0, 0, 0);
      }
      return;
    }

    if (x >= 195 && x <= 219 && y >= 228 && y <= 252) {
      UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
      if (window) {
        ITEM_INFO blessing = LookupClientItem(g_session.m_inventoryItems, this->info.blessingItemId, 0);
        window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&blessing), 0, 0, 0);
      }
    }
  }

  if (this->item.identity[1] > 0 && x >= 53 && x <= 99 && y >= 114 && y <= 160) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    if (window) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&this->item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x007E91D0
// FUNCTION: fn_09766
int UIGradeEnchantWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case UM_COMMAND:
    switch (argument1) {
    case ID_OK:
      if (this->requestPending != 1 && this->item.identity[1] > 0 && this->selectedMaterial != -1) {
        const int quantity = LookupClientItem(g_session.m_inventoryItems, this->materials[this->selectedMaterial].itemId, 0).quantity;
        if (quantity >= this->materials[this->selectedMaterial].amount && g_session.m_zeny >= this->SelectedMaterialZeny()) {
          if (this->protect != 0 || this->blessingAmount != 0 ||
              g_windowMgr.Confirm(LookupMessage(MSI_MERCHANTSHOP_TOTA_LOVER_ZENY_ERR), 2, true, false, 0, "Message", 280, 120, 0) != 188) {
            this->requestPending = 1;

            GradeEnchantRequestPacket packet;
            packet.id = 0xB5B;
            packet.index = static_cast<std::uint16_t>(this->item.identity[1]);
            packet.material = this->selectedMaterial;
            packet.useBlessing = this->blessingAmount > 0;
            packet.blessingCount = this->blessingAmount / this->info.blessingStepAmount;
            packet.protect = this->protect;
            GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
          }
        }
      }
      break;
    case ID_CANCEL:
    case ID_CLOSE: {
      GradeEnchantClosePacket packet;
      packet.id = 0xB5C;
      GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
      g_windowMgr.CloseWindow(WID_GRADEENCHANTWND);
      break;
    }
    case ID_UP:
      if (this->info.blessingItemId > 0 && this->blessingAmount < this->info.maxBlessingSteps * this->info.blessingStepAmount &&
          LookupClientItem(g_session.m_inventoryItems, this->info.blessingItemId, 0).quantity >= this->blessingAmount + this->info.blessingStepAmount) {
        this->blessingAmount += this->info.blessingStepAmount;
      }
      break;
    case ID_DOWN:
      if (this->info.blessingItemId > 0 && this->blessingAmount > 0) {
        this->blessingAmount -= this->info.blessingStepAmount;
      }
      break;
    case 510:
      ShellExecuteA(hWnd, "open", LookupMessage(MSI_ETC_PROBABILITY_URL), nullptr, nullptr, SW_SHOWNORMAL);
      break;
    }
    break;
  case UM_REFRESHITEM:
    if (this->state == 0 || this->state == 1) {
      const ITEM_INFO dropped = LookupClientItemByIdentity(g_session.m_inventoryItems, argument1);
      if (g_session.HasSwapEquipmentItem(dropped.identity[1]) == 1) {
        g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        return -1;
      }

      if (dropped.identity[1] != 0 && dropped.identified == 1) {
        GradeEnchantSelectPacket packet;
        packet.id = 0xB59;
        packet.index = static_cast<std::uint16_t>(argument1);
        GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
      }
    }
    break;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007E9650
// FUNCTION: fn_09767
void UIGradeEnchantWnd::SetEnchantInfo(
  short index, int value, int materialCount, GradeEnchantInfo info, int enchantOption, int enchantOptionCount, const GradeEnchantMaterial* materials
) {
  if (this->state != 0 && this->state != 1) {
    return;
  }

  if (index == -1) {
    return;
  }

  const ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
  if (owned.identity[1] == 0) {
    return;
  }

  this->item = owned;
  this->info = info;
  this->enchantOption = enchantOption;
  this->enchantOptionCount = enchantOptionCount;
  this->materials.assign(materials, materials + materialCount);
  this->selectedMaterial = -1;
  this->blessingAmount = 0;
  this->baseSuccessRate = value;

  this->state = 1;
  this->result = -1;
  this->stateTick = timeGetTime();
  this->itemFrame = 1;
  this->enhancedUpButton->tooltipEnabled = 1;
  this->Invalidate();

  this->requestPending = 0;
  this->enhancedUpButton->tooltipValue = this->info.blessingStepAmount;
}

// OFFSET: 0x007E9910
// FUNCTION: fn_09768
void UIGradeEnchant_BtnEnhancedDown::Enable(unsigned char value) {
  if (value == 0) {
    g_windowMgr.ReleaseCapture();
  }

  this->m_disabled = value ^ 1;
  this->Invalidate();
  this->geometry.visible = value;
}

// OFFSET: 0x007E9950
// FUNCTION: fn_09769
void UIGradeEnchant_BtnEnhancedUp::Enable(unsigned char value) {
  if (value == 0) {
    g_windowMgr.ReleaseCapture();
  }

  this->m_disabled = value ^ 1;
  this->Invalidate();
  this->geometry.visible = value;
}

// OFFSET: 0x007E9990
// FUNCTION: fn_09770
void UIGradeEnchant_BtnStart::Enable(unsigned char value) {
  if (value == 0) {
    g_windowMgr.ReleaseCapture();
  }

  this->m_disabled = value ^ 1;
  this->Invalidate();
  this->geometry.visible = value;
}

// OFFSET: 0x007E99D0
// FUNCTION: fn_09771
void UIGradeEnchantWnd::SetResult(int state, int result) {
  this->state = state;
  this->result = result;
  this->stateTick = timeGetTime();
  this->itemFrame = 1;
  this->enhancedUpButton->tooltipEnabled = state != 0;

  switch (state) {
  case 2:
    this->resultTick = this->stateTick;
    break;
  }

  this->Invalidate();
}


// OFFSET: 0x007E7050
// FUNCTION: fn_09748
void UIGradeEnchantWnd::UpdateEnchantMessages() {
  this->messageIds.clear();
  if (this->state != 1) {
    return;
  }

  std::string message;
  if (this->baseSuccessRate == 0) {
    this->messageIds.push_back(3821);
    return;
  }

  if (this->selectedMaterial != -1 && this->materials[this->selectedMaterial].breakOnFailure == 0) {
    this->messageIds.push_back(3822);
  }

  if (this->selectedMaterial == -1) {
    this->messageIds.push_back(3707);
  }

  if (this->selectedMaterial != -1 &&
      LookupClientItem(g_session.m_inventoryItems, this->materials[this->selectedMaterial].itemId, 0).quantity <
        this->materials[this->selectedMaterial].amount) {
    this->messageIds.push_back(3708);
  }

  if (g_session.m_zeny < this->SelectedMaterialZeny()) {
    this->messageIds.push_back(3709);
  }

  if (static_cast<float>(g_session.m_weight) > static_cast<float>(g_session.m_maxWeight) * 0.7f ||
      g_session.GetList0Size() + g_session.CountDistinctEquippedItems() +
          g_session.CountDistinctCostumeItems() >=
        g_session.extraInventorySlots + 100) {
    this->messageIds.push_back(3710);
  }

  if (this->selectedMaterial != -1 && this->materials[this->selectedMaterial].breakOnFailure == 1) {
    int warning = this->SuccessRate() < 100 ? 3712 : 3822;
    this->messageIds.push_back(warning);
  }

  if (this->selectedMaterial != -1 && this->materials[this->selectedMaterial].downgradeAmount > 0) {
    this->messageIds.push_back(3713);
  }
}
