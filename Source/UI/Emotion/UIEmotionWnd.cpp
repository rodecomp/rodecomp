#include "UI/Emotion/UIEmotionWnd.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"

#include "Client/Session.h"
#include "Contents/CashEmotionMgr.h"
#include "UI/CashEmotion/UICashEmotionListWnd.h"
#include "Contents/Hotkeys/EmotionHotkeys.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/MessageTable.h"
#include "Contents/UserData/AccountLinkedUserDataMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Contents/Async/AsyncWorkMgr.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdio>
#include <cstring>

UIBitmapButton* UIEmotionWnd::AddCloseButton(const char* tooltip, int width) {
  if (width == 0) {
    width = this->geometry.width;
  }

  UIBitmapButton* button = new UIBitmapButton();
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

// OFFSET: 0x008A2980
// FUNCTION: fn_11644
void UIEmotionWnd::OnCreate(int, int) {
  int y = 23;
  auto* emotions = &g_session.m_emotionHotkeys;
  char buffer[16];

  for (int i = 0; i < 10; ++i) {
    this->m_edits[i] = new UIEditCtrl2();
    this->m_edits[i]->SetSize(270, 16);
    this->m_edits[i]->Move(83, y - 3);
    this->m_edits[i]->maximum = 50;
    this->m_edits[i]->Background(232, 232, 232);
    this->m_edits[i]->SetText(emotions->current[i].c_str());
    this->AddChild(this->m_edits[i]);

    std::sprintf(buffer, "Alt + %d", i + 1);

    this->m_labels[i] = new UIStaticText();
    this->m_labels[i]->SetSize(70, 16);
    this->m_labels[i]->Move(20, y);
    this->m_labels[i]->SetText(buffer, 0);
    this->AddChild(this->m_labels[i]);

    y += 21;
  }

  this->m_labels[9]->SetText("Alt + 0", 0);
  this->UIFrameWnd::AddCloseButton("Alt+M", 0);

  {
    std::string path = "유저인터페이스\\";
    this->viewButton = new UIBitmapButton();
    this->viewButton->SetBitmap((path + "btn_view" + ".bmp").c_str(), 0, 0);
    this->viewButton->SetBitmap((path + "btn_view_a" + ".bmp").c_str(), 1, 0);
    this->viewButton->SetBitmap((path + "btn_view_b" + ".bmp").c_str(), 2, 0);
    this->viewButton->SetSize(this->viewButton->BitmapWidth(), this->viewButton->BitmapHeight());
    this->viewButton->Move(this->geometry.width - 50, this->geometry.height - 25);
    this->viewButton->SetId(283);
    this->AddChild(this->viewButton);
  }

  g_windowMgr.SetFocus(this->m_edits[0]);
  this->m_edits[0]->SetCursor(0xff);

  for (int i = 0; i < 10; ++i) {
    HotkeyInfo info = GetHotkeyInfo(*g_session.m_hotkeyScript, 2, i);
    if (!info.description.empty()) {
      this->m_labels[i]->SetText(info.text.c_str(), 0);
    }
  }
}

// OFFSET: 0x008A3000
// FUNCTION: fn_11645
void UIEmotionWnd::Paint() {
  int titleHeight = this->UIWindow::DrawTitleBar(true, LookupMessage(MSI_SHORTCUT_LIST), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x008A3040
// FUNCTION: fn_11646
void UIEmotionWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }
}

// OFFSET: 0x008A3060
// FUNCTION: fn_11647
int UIEmotionWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_DEFPUSH:
    break;
  case UM_COMMAND:
    switch (argument1) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_EMOTIONWND);
      break;
    case ID_OPEN_EMOTIONLIST:
      if (static_cast<UICashEmotionListWnd*>(g_windowMgr.cashEmotionListWnd) == nullptr) {
        g_windowMgr.EnsureWindow(WID_CASHEMOTIONLISTWND);
      } else {
        g_windowMgr.CloseWindow(WID_CASHEMOTIONLISTWND);
      }
      break;
    }
    break;
  case UM_SETINFO:
    this->m_saved = reinterpret_cast<EmotionWndInfo*>(argument1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }

    this->Move(this->m_saved->x, this->m_saved->y);
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
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
void UIEmotionWnd::StoreInfo() {
  if (this->m_saved != nullptr) {
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
  }

  for (unsigned i = 0; i < 10; ++i) {
    std::string value = this->m_edits[i]->Text();
    if (!value.empty()) {
      g_session.m_emotionHotkeys.current[i] = value;
    }
  }

  g_accountLinkedUserDataMgr->Save();
}
