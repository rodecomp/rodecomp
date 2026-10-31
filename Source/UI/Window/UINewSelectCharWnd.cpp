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
#include <algorithm>
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
// FUNCTION: fn_09577
UINewSelectCharWnd::UINewSelectCharWnd() : UIFrameWnd(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIFrameContext*>(nullptr), 0) {
  this->m_slotsPerPage = 15;
  this->m_unknown13C = false;
  this->m_window100 = nullptr;
  this->m_textButton = nullptr;
  this->m_button416 = nullptr;
  this->m_button417 = nullptr;
  this->m_windowC0 = nullptr;
  this->m_windowC4 = nullptr;

  this->m_slotButtons.resize(12);
  this->m_slotButtons2.resize(this->m_slotsPerPage);
  this->m_characterWindows.resize(this->m_slotsPerPage);

  this->m_unknown118 = false;
  this->m_page = 0;
  this->m_pageSlot = 0;
  this->m_totalCharacters = this->m_maxCharacters = g_normalSlotCount + g_premiumSlotCount + g_billingSlotCount;

  const int slotCount = std::max(static_cast<CLoginMode*>(g_modeMgr.GetLoginMode())->GetMaxCharacterSlot() + 1, this->m_maxCharacters);
  this->m_pageCount = slotCount / this->m_slotsPerPage;
  if (slotCount % this->m_slotsPerPage != 0) {
    this->m_pageCount++;
  }
  this->m_totalSlots = this->m_slotsPerPage * this->m_pageCount;

  this->m_charSlotInfos = new CharSlotInfo[this->m_totalSlots];
  this->m_slotStatus = new char[this->m_totalSlots];
  this->m_slotShorts = new short[this->m_totalSlots];
  for (int i = 0; i < this->m_totalSlots; i++) {
    this->m_slotStatus[i] = 1;
  }

  this->m_lastTick = timeGetTime();
  this->m_unknown138 = -1;
}

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
// FUNCTION: fn_09596
void UINewSelectCharWnd::sub_7D90E0() {
  const int slot = this->m_slotsPerPage * this->m_page + this->m_pageSlot;
  if (slot >= this->m_totalSlots) {
    return;
  }

  if (this->m_slotStatus[slot] == 1) {
    static_cast<UIButton*>(this->m_windowC0)->SetDisabled(1);
    this->m_windowC0->Invalidate();
    static_cast<UIButton*>(this->m_windowC4)->Enable(0);
    static_cast<UIButton*>(this->m_window100)->Enable(0);
    return;
  }

  const CharacterInfo* info = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, slot, 0, 0, 0));
  if (!info) {
    return;
  }

  if (info->deleteDate > 0) {
    static_cast<UIButton*>(this->m_windowC0)->SetDisabled(1);
    this->m_windowC0->Invalidate();
    static_cast<UIButton*>(this->m_window100)->Enable(1);
    static_cast<UIButton*>(this->m_windowC4)->Enable(1);
    Timestamp deleteDate(info->deleteDate);
    static_cast<UIButton*>(this->m_window100)->Enable(deleteDate.IsBeforeNow() == 1 ? 1 : 0);
  } else {
    static_cast<UIButton*>(this->m_windowC0)->SetDisabled(0);
    this->m_windowC0->Invalidate();
    static_cast<UIButton*>(this->m_window100)->Enable(0);
    static_cast<UIButton*>(this->m_windowC4)->Enable(0);
  }
}

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
