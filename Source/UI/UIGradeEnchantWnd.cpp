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

// OFFSET: 0x007E8A30

// OFFSET: 0x007E8B30

// OFFSET: 0x007E8BB0

// OFFSET: 0x007E8FA0
// FUNCTION: fn_09764
void UIGradeEnchantWnd::Slot32(int x, int y) {
}

// OFFSET: 0x007E9650

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
