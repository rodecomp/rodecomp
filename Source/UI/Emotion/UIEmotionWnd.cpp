#include "UI/Emotion/UIEmotionWnd.h"

#include "Client/Session.h"
#include "Contents/CashEmotionMgr.h"
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

#include <cstdio>
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

// OFFSET: 0x008A2980
// FUNCTION: fn_11644
void UIEmotionWnd::Slot15(int, int) {
  int y = 23;
  auto* emotions = reinterpret_cast<EmotionHotkeys*>(&g_session.unknown_0FF8);

  for (int i = 0; i < 10; ++i) {
    UIEditCtrl2* edit = new UIEditCtrl2(*this->m_context, *reinterpret_cast<UIEditContext*>(nullptr));
    this->m_edits[i] = edit;
    edit->SetSize(270, 16);
    edit->Move(83, y - 3);
    edit->maximum = 50;
    edit->Background(232, 232, 232);
    edit->SetText(emotions->current[i].c_str());
    this->AddChild(edit);

    char buffer[16];
    std::sprintf(buffer, "Alt + %d", i + 1);

    UIStaticText* label = new UIStaticText(*this->m_context, *reinterpret_cast<UIButtonContext*>(nullptr));
    this->m_labels[i] = label;
    label->SetSize(70, 16);
    label->Move(20, y);
    label->SetText(buffer, 0);
    this->AddChild(label);

    y += 21;
  }

  this->m_labels[9]->SetText("Alt + 0", 0);
  this->UIFrameWnd::AddCloseButton(*reinterpret_cast<UIButtonContext*>(nullptr), "Alt+M", 0);

  std::string path = "유저인터페이스\\";
  this->viewButton = new UIBitmapButton(*this->m_context, *reinterpret_cast<UIButtonContext*>(nullptr));
  this->viewButton->SetBitmap((path + "btn_view" + ".bmp").c_str(), 0, 0);
  this->viewButton->SetBitmap((path + "btn_view_a" + ".bmp").c_str(), 1, 0);
  this->viewButton->SetBitmap((path + "btn_view_b" + ".bmp").c_str(), 2, 0);
  this->viewButton->SetSize(this->viewButton->BitmapWidth(), this->viewButton->BitmapHeight());
  this->viewButton->Move(this->geometry.width - 50, this->geometry.height - 25);
  this->viewButton->SetId(283);
  this->AddChild(this->viewButton);

  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->m_edits[0]);
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
  const TextLocale locale = {};
  int titleHeight = this->UIWindow::DrawTitleBar(g_fontCache, locale, true, LookupMessage(574), 0);
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
      g_windowMgr.CloseWindow(0x56);
      return 0;
    }
    if (argument1 == 283) {
      if (g_cashEmotionListWnd == nullptr) {
        g_windowMgr.EnsureWindow(0x57);
      } else {
        g_windowMgr.CloseWindow(0x57);
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

  g_accountLinkedUserDataMgr->Save(
    *reinterpret_cast<EmotionHotkeys*>(&g_session.unknown_0FF8), *g_luaScript, g_session.m_aid, GetRenderSystemHolder()->system,
    reinterpret_cast<const char*>(&g_session.unknown_807C[8]), *g_asyncWorkMgr
  );
}
