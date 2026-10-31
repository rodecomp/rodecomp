#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Buttons/UISlotBmpButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowOrdering.h"
#include "UI/Window/FuncDrawWNDChar.h"
#include "Client/Registry.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CLoginMode.h"
#include "Utility/Timestamp.h"
#include "Utility/Text/FormatString.h"
#include "Client/Session.h"
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
// FUNCTION: fn_09591
void UINewSelectCharWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);

  for (int index = 0; index < this->m_slotsPerPage; ++index) {
    const int slot = this->m_page * this->m_slotsPerPage + index;
    if (slot >= this->m_totalSlots) {
      continue;
    }

    const CharacterInfo* info = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, slot, 0, 0, 0));
    const int column = slot % this->m_slotsPerPage;
    const char* label = nullptr;
    if (g_premiumSlotCount > 0 && slot >= g_normalSlotCount && slot < g_normalSlotCount + g_premiumSlotCount) {
      label = "Premium ";
    } else if (g_billingSlotCount > 0 && slot >= g_normalSlotCount + g_premiumSlotCount && slot < g_billingSlotCount + g_normalSlotCount + g_premiumSlotCount) {
      label = "Billing ";
    }

    if (label) {
      this->DrawTextAt(this->m_context->fonts, this->m_context->locale, column * 163 + 136, 50, label, 0, 1, 15, 0xFF, 0, 0);
      this->DrawTextAt(this->m_context->fonts, this->m_context->locale, column * 163 + 142, 63, "Service", 0, 1, 15, 0xFF, 0, 0);
    }

    if (this->m_slotStatus[slot] == 0 && info) {
      const CharSlotInfo& slotInfo = this->m_charSlotInfos[slot];
      const unsigned char sex = info->sex == 99 ? static_cast<unsigned char>(g_doramSex) : info->sex;
      FuncDrawWNDChar(
        this, slotInfo.x, slotInfo.y, sex, info->job, info->job, info->unknown_0058, info->head, info->accessory, info->accessory2, info->accessory3, info->robe,
        0, 0, info->bodyStyle, info->headPalette, slotInfo.action, slotInfo.frame, this->m_lastTick
      )
        .Draw(true);
    }

    const int page = slot / this->m_slotsPerPage;
    const int buttonIndex = page != 0 ? slot % (page * this->m_slotsPerPage) : slot;
    if (buttonIndex < this->m_slotsPerPage) {
      if (slot >= this->m_totalSlots) {
        continue;
      }

      static_cast<UIButton*>(this->m_slotButtons2[buttonIndex])->Enable(1);
      if (slot < g_producibleSlotCount ? this->m_slotStatus[slot] == 1 && slot >= g_validSlotCount : this->m_slotStatus[slot] == 1) {
        auto* button = static_cast<UIButton*>(this->m_slotButtons2[buttonIndex]);
        button->SetDisabled(1);
        button->Invalidate();
      }
    }

    if (slot >= this->m_totalSlots || !info) {
      continue;
    }

    if (info->deleteDate == 0) {
      this->m_charSlotInfos[slot].unknown_0148 = 0;
      this->m_charSlotInfos[slot].action = 0;
    } else {
      Timestamp deleteDate(info->deleteDate);
      if (slot == this->m_pageSlot && deleteDate.IsBeforeNow() == 1 && static_cast<UIButton*>(this->m_window100)->Disabled() == 1) {
        static_cast<UIButton*>(this->m_window100)->Enable(1);
      }
      this->m_charSlotInfos[slot].unknown_0148 = 16;
      this->m_charSlotInfos[slot].action = 16;
    }
  }

  const CharacterInfo* selected = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, static_cast<unsigned char>(g_selectedSlotIndex), 0, 0, 0));
  if (selected && selected->deleteDate > 0) {
    Timestamp deleteDate(selected->deleteDate);
    auto* button = static_cast<UIButton*>(this->m_window100);
    if (deleteDate.IsBeforeNow() == 1) {
      button->SetDisabled(0);
    } else {
      button->SetDisabled(1);
    }
    button->Invalidate();
  }
}

// OFFSET: 0x007D7740
// FUNCTION: fn_09592
void __stdcall Nullsub244(int, int) {
}

// OFFSET: 0x007D7750
// FUNCTION: fn_09593
void UINewSelectCharWnd::Slot19() {
  if (g_sub_1319eb4 == 3) {
    if (g_windowMgr.unknown_0210 != 0) {
      g_windowMgr.CloseWindow(0x12);
    }
  }
  if (g_sub_1319eb4 != 0) {
    this->Invalidate();
  }

  UIWindow* wnd = g_windowMgr.LookupWindow(93);
  if (wnd) {
    reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(wnd);
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
// FUNCTION: fn_09597
void UINewSelectCharWnd::sub_7D9260() {
  for (int index = 0; index < this->m_slotsPerPage; ++index) {
    const int slot = this->m_page * this->m_slotsPerPage + index;
    if (slot >= this->m_totalSlots) {
      continue;
    }

    static_cast<UISlotBmpButton*>(this->m_slotButtons2[index])->SetSlot(this->m_slotStatus[slot] ? -1 : slot, slot);
    if (slot < g_producibleSlotCount && this->m_slotStatus[slot] == 1) {
      this->m_slotButtons2[index]->SendMsg(nullptr, 431, slot >= g_validSlotCount ? 1 : 0, 0, 0, 0);
    } else {
      this->m_slotButtons2[index]->SendMsg(nullptr, 431, 1, 0, 0, 0);
    }
  }

  this->InvalidateChildren();
}

// OFFSET: 0x007D9310
// FUNCTION: fn_09598
void UINewSelectCharWnd::sub_7D9310() {
  const int current = this->m_page * this->m_slotsPerPage + this->m_pageSlot;
  if (current >= this->m_totalSlots) {
    return;
  }

  const CharacterInfo* info = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, current, 0, 0, 0));
  if (info) {
    static_cast<UIStaticText*>(this->m_slotButtons[0])->SetTextFitted(g_session.GetJobName(info->job, info->sex));
    static_cast<UIStaticText*>(this->m_slotButtons[1])->SetTextFitted(FormatOwnedString("%d", info->level).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[2])->SetTextFitted(FormatOwnedString("%lld", info->exp).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[3])->SetTextFitted(FormatOwnedString("%lld", info->hp).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[4])->SetTextFitted(FormatOwnedString("%lld", info->sp).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[5])->SetTextFitted(FormatOwnedString("%d", info->stats[0]).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[6])->SetTextFitted(FormatOwnedString("%d", info->stats[1]).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[7])->SetTextFitted(FormatOwnedString("%d", info->stats[2]).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[8])->SetTextFitted(FormatOwnedString("%d", info->stats[3]).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[9])->SetTextFitted(FormatOwnedString("%d", info->stats[4]).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[10])->SetTextFitted(FormatOwnedString("%d", info->stats[5]).c_str());
    static_cast<UIStaticText*>(this->m_slotButtons[11])->SetTextFitted(g_session.GetMapDisplayName(info->mapName));
  } else {
    for (int index = 0; index < 12; ++index) {
      static_cast<UIStaticText*>(this->m_slotButtons[index])->SetText("", 0);
    }
  }

  auto* button = static_cast<UIButton*>(this->m_windowFC);
  if (this->m_slotStatus[current] == 1) {
    button->SetDisabled(1);
  } else {
    button->SetDisabled(0);
  }
  button->Invalidate();

  this->sub_7D9260();
  this->sub_7D90E0();
}

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
