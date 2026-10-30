#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/FuncDrawWNDChar.h"
#include "Client/Registry.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CLoginMode.h"
#include "Utility/Timestamp.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"

#include <windows.h>
#include <mmsystem.h>
#include <cstring>

// GLOBAL: gl_25006
int g_sub_1319eb4 = 0;

// GLOBAL: gl_25007
int g_normalSlotCount;

// GLOBAL: gl_25008
int g_premiumSlotCount;

// GLOBAL: gl_25009
int g_billingSlotCount;

// GLOBAL: gl_25010
int g_producibleSlotCount;

// GLOBAL: gl_25011
int g_validSlotCount;

// GLOBAL: gl_25012
int g_doramSex;

// GLOBAL: gl_25013
char g_selectedSlotIndex;

// GLOBAL: gl_24792
void* g_charViewMode;

extern const char* LookupMessage(int id);

// OFFSET: 0x007D4190

UINewSelectCharWnd::~UINewSelectCharWnd() = default;

// OFFSET: 0x007D4520
// FUNCTION: fn_09578
CharSlotInfo::CharSlotInfo() = default;

// OFFSET: 0x007D4A00
// FUNCTION: fn_09583
CharSlotInfo::~CharSlotInfo() = default;

// OFFSET: 0x007D5000
// FUNCTION: fn_09586
void Nullsub243() {
}

// OFFSET: 0x007D50D0
// FUNCTION: fn_09587
std::string UIStaticText::GetText() const {
  return this->text;
}

// OFFSET: 0x007D5130
// FUNCTION: fn_09588
void UINewSelectCharWnd::DisableButtonById(int id) {
  switch (id) {
  case 416:
    this->m_button416->Enable(0);
    break;
  case 417:
    this->m_button417->Enable(0);
    break;
  }
}

// OFFSET: 0x007D52B0

// OFFSET: 0x007D7330

// OFFSET: 0x007D7740
// FUNCTION: fn_09592
void __stdcall Nullsub244(int, int) {
}

// OFFSET: 0x007D7750
// FUNCTION: fn_09593
void UINewSelectCharWnd::Slot19() {
  if (g_sub_1319eb4 == 3) {
    if (g_windowMgr.unknown_0210 != 0) {
      reinterpret_cast<void(__thiscall*)(UIWindowMgr*, int)>(0x00A8CF20)(&g_windowMgr, 0x12);
    }
  }
  if (g_sub_1319eb4 != 0) {
    this->Invalidate();
  }

  UIWindow* wnd = g_windowMgr.LookupWindow(93);
  if (wnd) {
    reinterpret_cast<void(__thiscall*)(UIWindowMgr*, UIWindow*)>(0x00A976C0)(&g_windowMgr, wnd);
  }

  if (this->m_pageSlot >= 0 && this->m_pageSlot < static_cast<int>(this->m_slotButtons2.size())) {
    UIWindow* btn = this->m_slotButtons2[this->m_pageSlot];
    if (btn) {
      btn->Slot19();
      btn->InvalidateChildren();
    }
  }
}

// OFFSET: 0x007D77D0

// OFFSET: 0x007D9090
// FUNCTION: fn_09595
void UINewSelectCharWnd::EnableButtonById(int id) {
  switch (id) {
  case 416:
    this->m_button416->Enable(1);
    break;
  case 417:
    this->m_button417->Enable(1);
    break;
  }
}

// OFFSET: 0x007D90E0

// OFFSET: 0x007D9260

// OFFSET: 0x007D9310

// OFFSET: 0x007D97B0
// FUNCTION: fn_09599
void UINewSelectCharWnd::SaveCurrentSlot() {
  HKEY key;
  DWORD disposition = 0;
  if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
    DWORD slot = this->m_slotsPerPage * this->m_page + this->m_pageSlot;
    RegSetValueExA(key, "CURSLOT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&slot), sizeof(slot));
    RegCloseKey(key);
  }
}
