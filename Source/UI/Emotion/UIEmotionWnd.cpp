#include "UI/Emotion/UIEmotionWnd.h"

#include "Client/Session.h"
#include "Contents/CashEmotionMgr.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/MessageTable.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <cstring>

UIBitmapButton* UIEmotionWnd::AddCloseButton(const char* tooltip, int width) {
  if (width == 0) {
    width = this->geometry.width;
  }

  UIBitmapButton* button = new UIBitmapButton(*this->m_context, *reinterpret_cast<UIButtonContext*>(nullptr));
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(width - 14, 3);
  button->SetId(201);
  button->SetTooltip(tooltip);
  this->AddChild(button);

  return button;
}

int UIEmotionWnd::DrawTitleBar(bool closeButton, const char* title, int width) {
  return reinterpret_cast<int(__thiscall*)(UIWindow*, bool, const char*, int)>(0x008D2CE0)(this, closeButton, title, width);
}

// OFFSET: 0x008A2980

// OFFSET: 0x008A3000
// FUNCTION: fn_11645
void UIEmotionWnd::Paint() {
  int titleHeight = this->DrawTitleBar(true, LookupMessage(574), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x008A3040
// FUNCTION: fn_11646
void UIEmotionWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x008A3060
// FUNCTION: fn_11647
int UIEmotionWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 0) {
    return 0;
  }

  if (message == 6) {
    if (argument1 == 201) {
      reinterpret_cast<void(__thiscall*)(UIWindowMgr*, int)>(0x00A8CF20)(&g_windowMgr, 0x56);
      return 0;
    }
    if (argument1 == 283) {
      if (g_cashEmotionListWnd == nullptr) {
        reinterpret_cast<void(__thiscall*)(UIWindowMgr*, int)>(0x00A978D0)(&g_windowMgr, 0x57);
      } else {
        reinterpret_cast<void(__thiscall*)(UIWindowMgr*, int)>(0x00A8CF20)(&g_windowMgr, 0x57);
      }
      return 0;
    }
    return 0;
  }

  if (message == 34) {
    this->m_saved = reinterpret_cast<EmotionWndInfo*>(argument1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }
    this->Move(this->m_saved->x, this->m_saved->y);
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x008A3110
// FUNCTION: fn_11648
void UIEmotionWnd::SetMacroText(const char* text, int index) {
  if (index == -1) {
    UIWindow* hover = g_windowMgr.GetHoverWindow();
    for (int i = 0; i < 10; ++i) {
      if (hover == this->m_edits[i]) {
        index = i;
      }
    }
    if (index == -1) {
      return;
    }
  }

  this->m_edits[index]->SetText(text);
}

// OFFSET: 0x008A31E0
// FUNCTION: fn_11649
void UIEmotionWnd::Slot11() {
  if (this->m_saved != nullptr) {
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
  }

  std::string* macros = reinterpret_cast<std::string*>(reinterpret_cast<char*>(&g_session) + 0xFF8);
  for (int i = 0; i < 10; ++i) {
    const char* text = this->m_edits[i]->Text();
    std::string value(text, std::strlen(text));
    if (!value.empty() && &macros[i] != &value) {
      macros[i].assign(value.data(), value.size());
    }
  }

  reinterpret_cast<void(__thiscall*)(CAccountLinkedUserDataMgr*)>(0x005BCE50)(g_accountLinkedUserDataMgr);
}
