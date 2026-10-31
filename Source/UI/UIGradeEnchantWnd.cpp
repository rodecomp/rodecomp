#include "UI/UIGradeEnchantWnd.h"

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
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

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

// OFFSET: 0x007E68F0
// FUNCTION: fn_09738
UIGradeEnchantWnd::UIGradeEnchantWnd()
    : UIFrameWnd(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIFrameContext*>(nullptr), 0), enhancedUpButton(nullptr), state(0),
      result(-1), unknown_00C8(200), unknown_00CC(100), stateTick(0), unknown_00D4(0), unknown_00D8(1), unknown_00DC(1), unknown_00E0(0),
      selectedMaterial(-1), hoverMaterial(-1), materials(), info{}, unknown_0108(0), unknown_010C(0), unknown_0110(0), unknown_0114(0), unknown_0118(0),
      unknown_011C(), item(0), unknown_0220(0), startButton(nullptr), enhancedDownButton(nullptr) {
}

// OFFSET: 0x007E6A60
// FUNCTION: fn_09739
UIGradeEnchant_BtnEnhancedDown::UIGradeEnchant_BtnEnhancedDown(UIWindowContext& windows, UIButtonContext& controls) : UIBitmapButton(windows, controls) {
}

// OFFSET: 0x007E6AC0
// FUNCTION: fn_09740
UIGradeEnchant_BtnEnhancedUp::UIGradeEnchant_BtnEnhancedUp(UIWindowContext& windows, UIButtonContext& controls)
    : UIBitmapButton(windows, controls), tooltipValue(0), tooltipEnabled(0) {
}

// OFFSET: 0x007E6B30
// FUNCTION: fn_09741
UIGradeEnchant_BtnStart::UIGradeEnchant_BtnStart(UIWindowContext& windows, UIButtonContext& controls) : UIBitmapButton(windows, controls) {
}

// OFFSET: 0x007E7500
// FUNCTION: fn_09749
void UIGradeEnchantWnd::Slot10(int x, int y, std::intptr_t value) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (x >= 53 && x <= 99 && y >= 114 && y <= 160 && drag->source == 1) {
    this->SendMsg(this, 23, drag->id, 0, 0, 0);
  }

  g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x007E7560
// FUNCTION: fn_09750
void UIGradeEnchantWnd::ResetItem() {
  this->unknown_0220 = 0;
  this->unknown_00D8 = 1;
  this->unknown_00DC = 1;
  this->item = ITEM_INFO(0);

  this->materials.clear();
  this->selectedMaterial = -1;
  this->info = GradeEnchantInfo{};
  this->unknown_0108 = 0;
  this->unknown_010C = 0;
  this->unknown_0114 = 0;
  this->unknown_0110 = 0;
  this->unknown_0118 = 0;
  this->unknown_00E0 = 0;

  this->SetResult(0, -1);
  this->Invalidate();
}

// OFFSET: 0x007E76A0
// FUNCTION: fn_09751
int UIGradeEnchantWnd::SelectedMaterialZeny() {
  if (this->item.unknown_0000[1] > 0 && !this->materials.empty() && this->selectedMaterial >= 0 &&
      static_cast<unsigned>(this->selectedMaterial) < this->materials.size()) {
    return this->materials[this->selectedMaterial].zeny;
  }

  return 0;
}

// OFFSET: 0x007E7740
// FUNCTION: fn_09752
int UIGradeEnchantWnd::SuccessRate() {
  if (this->item.unknown_0000[1] == 0) {
    return 0;
  }

  int rate = this->unknown_0118;
  if (this->unknown_0114 > 0) {
    rate += this->unknown_0114 / this->info.unknown_0004 * this->info.unknown_000C;
  }

  return rate > 100 ? 100 : rate;
}

// OFFSET: 0x007E7780
// FUNCTION: fn_09753
void UIGradeEnchantWnd::Slot15(int, int) {
  if (UIBitmapButton* close = this->AddCloseButton(*this->m_controls, "", 0)) {
    close->Move(0x180, 0x14);
  }

  auto* title = new UIBasicButton(*this->m_context, *this->m_controls);
  title->Configure(0x2A, LookupMessage(0x513));
  title->Move(0x159, 299);
  title->SetId(0xB9);
  this->AddChild(title);

  this->startButton = new UIGradeEnchant_BtnStart(*this->m_context, *this->m_controls);
  this->startButton->SetBitmap("유저인터페이스\\grade_enchant\\btn_start.bmp", 0, 0);
  this->startButton->SetBitmap("유저인터페이스\\grade_enchant\\btn_start_over.bmp", 1, 0);
  this->startButton->SetBitmap("유저인터페이스\\grade_enchant\\btn_start_press.bmp", 2, 0);
  this->startButton->SetBitmap("유저인터페이스\\grade_enchant\\btn_start_disable.bmp", 3, 0);
  this->startButton->SetSize(this->startButton->BitmapWidth(), this->startButton->BitmapHeight());
  this->startButton->Move(0x23, 0x123);
  this->startButton->SetId(0xB8);
  this->startButton->SetDisabled(0);
  this->startButton->Invalidate();
  this->AddChild(this->startButton);

  this->enhancedUpButton = new UIGradeEnchant_BtnEnhancedUp(*this->m_context, *this->m_controls);
  this->enhancedUpButton->SetBitmap("유저인터페이스\\grade_enchant\\btn_chance_up.bmp", 0, 0);
  this->enhancedUpButton->SetBitmap("유저인터페이스\\grade_enchant\\btn_chance_up_over.bmp", 1, 0);
  this->enhancedUpButton->SetBitmap("유저인터페이스\\grade_enchant\\btn_chance_up_pick.bmp", 2, 0);
  this->enhancedUpButton->SetSize(this->enhancedUpButton->BitmapWidth(), this->enhancedUpButton->BitmapHeight());
  this->enhancedUpButton->Move(0x9E, 0xE1);
  this->enhancedUpButton->SetId(0x117);
  this->enhancedUpButton->SetDisabled(0);
  this->enhancedUpButton->Invalidate();
  this->AddChild(this->enhancedUpButton);

  this->enhancedDownButton = new UIGradeEnchant_BtnEnhancedDown(*this->m_context, *this->m_controls);
  this->enhancedDownButton->SetBitmap("유저인터페이스\\grade_enchant\\btn_chance_down.bmp", 0, 0);
  this->enhancedDownButton->SetBitmap("유저인터페이스\\grade_enchant\\btn_chance_down_over.bmp", 1, 0);
  this->enhancedDownButton->SetBitmap("유저인터페이스\\grade_enchant\\btn_chance_down_pick.bmp", 2, 0);
  this->enhancedDownButton->SetSize(this->enhancedDownButton->BitmapWidth(), this->enhancedDownButton->BitmapHeight());
  this->enhancedDownButton->Move(0x9E, 0xF0);
  this->enhancedDownButton->SetId(0x118);
  this->enhancedDownButton->SetDisabled(0);
  this->enhancedDownButton->Invalidate();
  this->AddChild(this->enhancedDownButton);

  auto* help = new UIBitmapButton(*this->m_context, *this->m_controls);
  help->SetBitmap("유저인터페이스\\basic_interface\\btn_help_normal.bmp", 0, 0);
  help->SetBitmap("유저인터페이스\\basic_interface\\btn_help_over.bmp", 1, 0);
  help->SetBitmap("유저인터페이스\\basic_interface\\btn_help_pressed.bmp", 2, 0);
  help->SetBitmap("유저인터페이스\\basic_interface\\btn_help_disabled.bmp", 3, 0);
  help->SetSize(help->BitmapWidth(), help->BitmapHeight());
  help->Move(0x8C, 100);
  help->SetTooltip(LookupMessage(0x1013));
  help->SetId(0x1FE);
  this->AddChild(help);
}

// OFFSET: 0x007E7BA0
// FUNCTION: fn_09754
void UIGradeEnchantWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawBitmap(0, 0, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\grade_enchant\\bg_grade_enchant.bmp"))), false);

  this->DrawBitmap(158, 142, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\btn_material.bmp").c_str()))), false);
  this->DrawBitmap(158, 182, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\btn_material.bmp").c_str()))), false);
  this->DrawBitmap(158, 102, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\btn_material.bmp").c_str()))), false);

  if (this->state == 0) {
    this->DrawBitmap(152, 46, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\caution.bmp").c_str()))), true);
    this->DrawBitmap(152, 62, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\caution.bmp").c_str()))), true);
    this->DrawBitmap(152, 78, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\caution.bmp").c_str()))), true);

    this->sub_A85850(166, 46, LookupMessage(3870), 228, 0, 0x00A0ADC, 0, 0, 11, 1, 0);
    this->sub_A85850(166, 62, LookupMessage(3871), 228, 0, 0x5AEB00, 0, 0, 11, 1, 0);
    this->sub_A85850(166, 78, LookupMessage(3872), 228, 0, 0x5AEB00, 0, 0, 11, 1, 0);
  }

  if (this->state == 1 || this->state == 2) {
    this->DrawBitmap(192, 225, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\btn_unbroken.bmp").c_str()))), false);
    this->DrawBitmap(24, 54, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\bg_gradeitem%d.bmp", this->unknown_00D8).c_str()))), false);

    if (this->hoverMaterial >= 0 && this->hoverMaterial != this->selectedMaterial) {
      this->DrawBitmap(158, this->hoverMaterial * 40 + 102, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\btn_material_over.bmp").c_str()))), false);
    }

    if (this->selectedMaterial >= 0) {
      this->DrawBitmap(158, this->selectedMaterial * 40 + 102, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\btn_material_pick.bmp").c_str()))), false);
    }

    char description[260];
    std::memset(description, 0, sizeof(description));
    int capacity = sizeof(description);

    int y = 118;
    for (size_t i = 0; i < this->materials.size(); ++i) {
      ITEM_INFO itemInfo;
      itemInfo.SetItemId(this->materials[i].itemId);
      itemInfo.sub_6D5810(description, capacity, 0);
      this->sub_8A8EE0(170, y - 9, &itemInfo, 0);
      this->sub_A85850(203, y, FormatOwnedString(LookupMessage(3553), description, this->materials[i].amount), 180, 0, 0, 0, 0, 11, 0, 0);
      y += 40;
    }

    if (this->info.unknown_0000 > 0) {
      ITEM_INFO itemInfo;
      itemInfo.SetItemId(this->info.unknown_0000);
      itemInfo.sub_6D5810(description, capacity, 0);
      this->sub_8A8EE0(195, 228, &itemInfo, 0);
      this->sub_A85850(228, 237, FormatOwnedString(LookupMessage(3553), description, this->unknown_0114), 160, 0, 0xFFFFFF, 0, 0, 11, 1, 1);
    }

    this->sub_7E7050();

    for (size_t i = 0; i < this->unknown_011C.size() && i < 3; ++i) {
      int itemY = 46 + i * 16;
      this->DrawBitmap(152, itemY, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\caution.bmp").c_str()))), true);
      int msgId = this->unknown_011C[i];
      std::uint32_t color = (msgId == 3711 || msgId == 3822) ? 0x5AEB00 : 0x00A0ADC;
      this->sub_A85850(166, itemY, LookupMessage(msgId), 228, 0, color, 0, 0, 11, 1, 0);
    }

    this->sub_A85850(307, 303, FormatOwnedString("%d", this->SelectedMaterialZeny()), 120, 2, 0, 0, 0, 16, 1, 0);
  }

  if (this->state == 2) {
    this->DrawBitmap(56, 268, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\gradewheel%d.bmp", this->unknown_00DC).c_str()))), true);
  } else {
    this->DrawBitmap(56, 268, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\gradewheel%d.bmp", 1).c_str()))), true);
  }

  if (this->item.unknown_0000[1] > 0) {
    this->sub_8A8EE0(64, 125, &this->item, 0);
    if (this->item.unknown_0088 > 0) {
      this->DrawBitmap(64, 137, reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", this->item.unknown_0088).c_str()))), true);
    }
  }

  if (this->state == 1 || this->state == 2) {
    this->sub_A85850(39, 88, FormatOwnedString("%d%%", this->SuccessRate()), 74, 6, 0xFFFFFF, 0, 29, 28, 1, 1);
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
void UIGradeEnchantWnd::Slot26(int x, int y) {
  if (this->item.unknown_0000[1] > 0 && x >= 53 && x <= 99 && y >= 114 && y <= 160) {
    this->ResetItem();
  }
}

// OFFSET: 0x007E89C0
// FUNCTION: fn_09759
void UIGradeEnchantWnd::Slot25(int x, int y) {
  if (y < 38 && x > 141) {
    UIFrameWnd::Slot25(x, y);
    return;
  }

  if (this->state == 1 && this->item.unknown_0000[1] > 0 && x >= 64 && x <= 110 && y >= 125 && y <= 171) {
    ItemDescriptions descriptions{g_itemDescriptions};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(this->m_frameContext->windows, descriptions, 33, this->item, 1, -1);
  }
}

// OFFSET: 0x007E8A30
// FUNCTION: fn_09760
void UIGradeEnchantWnd::Slot31(int x, int y) {
  if (y < 80 && x > 141) {
    UIFrameWnd::Slot31(x, y);
    return;
  }

  if (x < 158 || x > 393 || y < 102 || y > 219) {
    return;
  }

  const int row = (y - 102) / 40;
  if (y > row * 40 + 139 || row >= 3 || row < 0 || static_cast<unsigned>(row) >= this->materials.size()) {
    return;
  }

  if (this->selectedMaterial != row) {
    if (this->unknown_0114 > 0) {
      this->m_controls->services.Confirm(LookupMessage(3831), 0, 1, 0, "Message", 280, 120);
    }

    this->selectedMaterial = row;
    this->unknown_0114 = 0;
    this->unknown_0110 = 0;
  }

  this->Invalidate();
}

// OFFSET: 0x007E8B30
// FUNCTION: fn_09761
void UIGradeEnchantWnd::Slot28(int x, int y) {
  if (x >= 158 && x <= 393 && y >= 102 && y <= 219) {
    const int row = (y - 102) / 40;
    if (y <= row * 40 + 139) {
      this->hoverMaterial = row >= 3 ? -1 : row;
      return;
    }
  }

  this->hoverMaterial = -1;
}

// OFFSET: 0x007E8BB0
// FUNCTION: fn_09762
void UIGradeEnchant_BtnEnhancedUp::Slot28(int x, int y) {
  if (this->tooltipValue == 0 || this->tooltipEnabled == 0) {
    return;
  }

  if (g_modeMgr.GetGameMode() == nullptr) {
    return;
  }

  this->m_tooltip = FormatOwnedString(LookupMessage(3976), this->tooltipValue);

  int screenX = 0;
  int screenY = -20;
  for (const UIWindow* window = this; window; window = window->parent) {
    screenX += window->geometry.x;
    screenY += window->geometry.y;
  }

  this->m_buttonContext->services.Tooltip(this->m_tooltip.c_str(), screenX, screenY, -1, false, true);
}

// OFFSET: 0x007E8CC0
// FUNCTION: fn_09763
void UIGradeEnchantWnd::Slot19() {
  const int frameCounts[3] = {6, 6, 5};
  if (this->unknown_00D8 >= frameCounts[this->state]) {
    return;
  }

  const DWORD now = timeGetTime();
  if (now > this->unknown_00C8 + this->stateTick) {
    if (++this->unknown_00D8 >= frameCounts[this->state]) {
      this->unknown_00D8 = 1;
    }
    this->stateTick = timeGetTime();
    this->Invalidate();
  }

  if (now > this->unknown_00CC + this->unknown_00D4) {
    if (++this->unknown_00DC >= 4) {
      this->unknown_00DC = 1;
    }
    this->unknown_00D4 = timeGetTime();
    this->Invalidate();
  }

  if (this->state == 2 && now >= this->unknown_00E0 + 3000) {
    switch (this->result) {
    case 0:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3702)), 0xFFFF00, 0, 0, 0);
      break;
    case 1:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3703)), 0xFFFF00, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3704)), 0xFFFF00, 0, 0, 0);
      break;
    case 3:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3705)), 0xFFFF00, 0, 0, 0);
      break;
    case 4:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3706)), 0xFFFF00, 0, 0, 0);
      break;
    }
    this->ResetItem();
  }

  const bool disabled = this->unknown_0220 == 1 || this->item.unknown_0000[1] == 0 || this->selectedMaterial == -1 ||
                        LookupClientItem(g_session.m_inventoryItems, this->materials[this->selectedMaterial].itemId, 0).quantity <
                          this->materials[this->selectedMaterial].amount ||
                        g_session.m_zeny < this->SelectedMaterialZeny() || this->unknown_0118 == 0;

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
void UIGradeEnchantWnd::Slot32(int x, int y) {
}

// OFFSET: 0x007E8FB0
// FUNCTION: fn_09765
void UIGradeEnchantWnd::Slot33(int x, int y) {
  if (y < 80 && x > 141) {
    return;
  }

  if (this->state == 1) {
    if (x >= 158 && x <= 393 && y >= 102 && y <= 219) {
      const int row = (y - 102) / 40;
      if (y <= row * 40 + 139 && row < 3 && row >= 0 && static_cast<unsigned>(row) < this->materials.size()) {
        UIWindow* window = this->m_frameContext->windows.EnsureWindow(12);
        if (window) {
          ITEM_INFO material = LookupClientItem(g_session.m_inventoryItems, this->materials[row].itemId, 0);
          window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&material), 0, 0, 0);
        }
        return;
      }
    }

    if (x >= 195 && x <= 219 && y >= 228 && y <= 252) {
      UIWindow* window = this->m_frameContext->windows.EnsureWindow(12);
      if (window) {
        ITEM_INFO blessing = LookupClientItem(g_session.m_inventoryItems, this->info.unknown_0000, 0);
        window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&blessing), 0, 0, 0);
      }
    }
  }

  if (this->item.unknown_0000[1] > 0 && x >= 53 && x <= 99 && y >= 114 && y <= 160) {
    UIWindow* window = this->m_frameContext->windows.EnsureWindow(12);
    if (window) {
      window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&this->item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x007E91D0
// FUNCTION: fn_09766
int UIGradeEnchantWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message == 6) {
    if (argument1 == 184) {
      if (this->unknown_0220 != 1 && this->item.unknown_0000[1] > 0 && this->selectedMaterial != -1) {
        const int quantity = LookupClientItem(g_session.m_inventoryItems, this->materials[this->selectedMaterial].itemId, 0).quantity;
        if (quantity >= this->materials[this->selectedMaterial].amount && g_session.m_zeny >= this->SelectedMaterialZeny()) {
          if (this->unknown_0110 != 0 || this->unknown_0114 != 0 ||
              this->m_controls->services.Confirm(LookupMessage(3823), 2, 1, 0, "Message", 280, 120) != 188) {
            this->unknown_0220 = 1;

            GradeEnchantRequestPacket packet;
            packet.id = 0xB5B;
            packet.index = static_cast<std::uint16_t>(this->item.unknown_0000[1]);
            packet.material = this->selectedMaterial;
            packet.useBlessing = this->unknown_0114 > 0;
            packet.blessingCount = this->unknown_0114 / this->info.unknown_0004;
            packet.protect = this->unknown_0110;
            GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
          }
        }
      }
    } else if (argument1 == 185 || argument1 == 201) {
      GradeEnchantClosePacket packet;
      packet.id = 0xB5C;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
      this->m_frameContext->windows.CloseWindow(343);
    } else if (argument1 == 279) {
      if (this->info.unknown_0000 > 0 && this->unknown_0114 < this->info.unknown_0008 * this->info.unknown_0004 &&
          LookupClientItem(g_session.m_inventoryItems, this->info.unknown_0000, 0).quantity >= this->unknown_0114 + this->info.unknown_0004) {
        this->unknown_0114 += this->info.unknown_0004;
      }
    } else if (argument1 == 280) {
      if (this->info.unknown_0000 > 0 && this->unknown_0114 > 0) {
        this->unknown_0114 -= this->info.unknown_0004;
      }
    } else if (argument1 == 510) {
      ShellExecuteA(hWnd, "open", LookupMessage(4124), nullptr, nullptr, SW_SHOWNORMAL);
    }
  } else if (message == 23 && (this->state == 0 || this->state == 1)) {
    const ITEM_INFO dropped = LookupClientItemByIdentity(g_session.m_inventoryItems, argument1);
    if (g_session.HasItem61B8(dropped.unknown_0000[1]) == 1) {
      g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
      return -1;
    }

    if (dropped.unknown_0000[1] != 0 && dropped.identified == 1) {
      GradeEnchantSelectPacket packet;
      packet.id = 0xB59;
      packet.index = static_cast<std::uint16_t>(argument1);
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
    }
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007E9650
// FUNCTION: fn_09767
void UIGradeEnchantWnd::SetEnchantInfo(
  short index, int value, int materialCount, GradeEnchantInfo info, int value0108, int value010C, const GradeEnchantMaterial* materials
) {
  if (this->state != 0 && this->state != 1) {
    return;
  }

  if (index == -1) {
    return;
  }

  const ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, index);
  if (owned.unknown_0000[1] == 0) {
    return;
  }

  this->item = owned;
  this->info = info;
  this->unknown_0108 = value0108;
  this->unknown_010C = value010C;
  this->materials.assign(materials, materials + materialCount);
  this->selectedMaterial = -1;
  this->unknown_0114 = 0;
  this->unknown_0118 = value;

  this->state = 1;
  this->result = -1;
  this->stateTick = timeGetTime();
  this->unknown_00D8 = 1;
  this->enhancedUpButton->tooltipEnabled = 1;
  this->Invalidate();

  this->unknown_0220 = 0;
  this->enhancedUpButton->tooltipValue = this->info.unknown_0004;
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
  this->unknown_00D8 = 1;
  this->enhancedUpButton->tooltipEnabled = state != 0;

  if (state == 2) {
    this->unknown_00E0 = this->stateTick;
  }

  this->Invalidate();
}

struct SessionMessengerContext {
  int GetList0Size() const;
};

// OFFSET: 0x007E7050
// FUNCTION: fn_09748
void UIGradeEnchantWnd::sub_7E7050() {
  this->unknown_011C.clear();
  if (this->state != 1) {
    return;
  }

  std::string message;
  if (this->unknown_0118 == 0) {
    this->unknown_011C.push_back(3821);
    return;
  }

  if (this->selectedMaterial != -1 && this->materials[this->selectedMaterial].unknown_0010 == 0) {
    this->unknown_011C.push_back(3822);
  }

  if (this->selectedMaterial == -1) {
    this->unknown_011C.push_back(3707);
  }

  if (this->selectedMaterial != -1 &&
      LookupClientItem(g_session.m_inventoryItems, this->materials[this->selectedMaterial].itemId, 0).quantity <
        this->materials[this->selectedMaterial].amount) {
    this->unknown_011C.push_back(3708);
  }

  if (g_session.m_zeny < this->SelectedMaterialZeny()) {
    this->unknown_011C.push_back(3709);
  }

  if (static_cast<float>(g_session.m_weight) > static_cast<float>(g_session.m_maxWeight) * 0.7f ||
      reinterpret_cast<const SessionMessengerContext*>(&g_session)->GetList0Size() + g_session.CountItems17F0() +
          g_session.CountItems2B50() >=
        g_session.unknown_7FBC + 100) {
    this->unknown_011C.push_back(3710);
  }

  if (this->selectedMaterial != -1 && this->materials[this->selectedMaterial].unknown_0010 == 1) {
    int warning = this->SuccessRate() < 100 ? 3712 : 3822;
    this->unknown_011C.push_back(warning);
  }

  if (this->selectedMaterial != -1 && this->materials[this->selectedMaterial].unknown_000C > 0) {
    this->unknown_011C.push_back(3713);
  }
}
