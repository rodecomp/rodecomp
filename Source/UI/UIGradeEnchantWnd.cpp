#include "UI/UIGradeEnchantWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>
#include <timeapi.h>

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

// OFFSET: 0x007E8FA0
// FUNCTION: fn_09764
void UIGradeEnchantWnd::Slot32(int x, int y) {
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
