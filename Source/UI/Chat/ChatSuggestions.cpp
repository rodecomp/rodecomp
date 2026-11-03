#include "UI/Window/UIWindowMgr.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Chat/UISubChatHisWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Edit/ItemTagRetirement.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindow.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UITabControl.h"

// OFFSET: 0x0091E7E0
// FUNCTION: fn_12628
bool UINewChatWnd::SuggestionsVisible() const {
  return this->m_suggestions && this->m_suggestions->geometry.visible != 0;
}

// OFFSET: 0x0093D270
// FUNCTION: fn_12767
void UINewChatWnd::ShowSuggestions(std::uint8_t visible) {
  UIWindow* child = this->m_suggestions;
  if (!child) {
    return;
  }

  child->Show(visible);
  if (visible != 0) {
    this->RemoveChild(child, 0);
    if (!this->FindUid(child->Uid())) {
      this->AddChild(child);
    }
  }
}

// OFFSET: 0x00933980
// FUNCTION: fn_12732
void UINewChatWnd::ApplyInputLayout() {
  if (g_inputSuggestionsEnabled) {
    const int base = this->geometry.height - this->m_inputAreaHeight;
    this->unknown_00C0->Move(6, base + 4);
    this->m_chatEdit->Move(110, base + 4);
    UIWindow* child = this->unknown_00B8;
    child->Move(3, this->geometry.height - child->geometry.height);
    this->m_inputButtons[0]->Move(97, base + 3);
    this->m_inputButtons[1]->Move(586, base + 7);
    this->m_inputButtons[2]->Move(572, base + 7);
  } else {
    this->unknown_00C0->Move(-100, -100);
    this->m_chatEdit->Move(-100, -100);
    this->unknown_00B8->Move(-100, -100);
    for (UIWindow* child : this->m_inputButtons) {
      child->Move(-100, -100);
    }
  }

  this->Invalidate();
}

// OFFSET: 0x009337F0
// FUNCTION: fn_12730
void UINewChatWnd::ApplyHeaderLayout() {
  UIWindow* const buttons[] = {this->m_headerButtons[1], this->m_headerButtons[0], this->unknown_0128, this->m_chatModeButton};
  if (g_chatInputFlag == 0 && this->geometry.unknown_0030 != 0) {
    buttons[0]->Move(-100, -100);
    buttons[1]->Move(-100, -100);
    buttons[2]->Move(-100, -100);
    UIWindow* button = buttons[3];
    button->Move(this->m_lockButtonX - button->geometry.width - 2, 4);

    return;
  }

  if (this->geometry.unknown_0030 == 0) {
    for (UIWindow* button : buttons) {
      button->Move(-100, -100);
    }

    return;
  }

  UIWindow* button = buttons[0];
  int x = this->m_lockButtonX - button->geometry.width - 2;
  button->Move(x, 4);

  for (unsigned index = 1; index < 4; ++index) {
    button = buttons[index];
    x -= (button->geometry.width + 2);
    button->Move(x, 4);
  }
}

// OFFSET: 0x009338F0
// FUNCTION: fn_12731
void UISubChatHisWnd::ApplyButtonLayout() {
  if (g_chatInputFlag == 0) {
    this->m_miniButton->Move(-100, -100);
    this->m_stickButton->Move(-100, -100);
    UIWindow* button = this->m_optionButton;
    button->Move(this->geometry.width - button->geometry.width - 2, 4);
    return;
  }

  UIWindow* button = this->m_miniButton;
  int x = this->geometry.width - button->geometry.width - 2;
  button->Move(x, 4);

  button = this->m_stickButton;
  x -= (button->geometry.width + 2);
  button->Move(x, 4);

  button = this->m_optionButton;
  x -= (button->geometry.width + 2);
  button->Move(x, 4);
}

// OFFSET: 0x0093D2C0
// FUNCTION: fn_12768
void UINewChatWnd::ResizeRows(int count) {
  if (g_windowMgr.unknown_0214) {
    g_windowMgr.CloseWindow(26);
  }

  UIWindow& chat = *this;
  if (g_windowMgr.Capture() == &chat) {
    return;
  }

  const int rowCount = count > 15 ? 0 : count < 0 ? 15 : count;
  chat.geometry.unknown_0030 = static_cast<unsigned>(rowCount);
  chat.Invalidate();
  const int previousHeight = chat.geometry.height;
  const int height = rowCount == 0 ? this->m_inputAreaHeight : this->m_inputAreaHeight + (rowCount + 3) * 14 + this->m_inputOffsetY;
  chat.Resize(chat.geometry.width, height);
  chat.Move(chat.geometry.x, chat.geometry.y - height + previousHeight);

  if (rowCount != 0) {
    for (std::size_t index = 0; index < this->m_tabs.size(); ++index) {
      UIWindow* row = this->m_tabs[index];
      row->Resize(chat.geometry.width - 8, height - this->m_inputOffsetY - this->m_inputAreaHeight - 4);
      this->m_tabs[index]->Invalidate();
      row = this->m_tabs[index];

      if (index == static_cast<unsigned>(this->m_currentTab)) {
        row->Move(5, this->m_inputOffsetY + 2);
      } else {
        row->Move(-100 - chat.geometry.width, -100);
      }
    }

    chat.Invalidate();
  } else {
    for (std::size_t index = 0; index < this->m_tabs.size(); ++index) {
      this->m_tabs[index]->Move(-100 - chat.geometry.width, -100);
    }
  }

  this->unknown_00C0->Move(6, chat.geometry.height - this->m_inputAreaHeight + 4);
  this->m_chatEdit->Move(110, chat.geometry.height - this->m_inputAreaHeight + 4);
  UIWindow* child = this->unknown_00B8;
  child->Move(3, chat.geometry.height - child->geometry.height);

  if (rowCount != 0) {
    this->unknown_0128->Move(this->m_tabWidth * 2 + 4, 4);
    this->m_chatModeButton->Move(this->m_tabWidth * 2 + 19, 4);
    this->m_tabControl->Move(3, 0);
  } else {
    this->unknown_0128->Move(-100, -100);
    this->m_chatModeButton->Move(-100, -100);
    this->m_tabControl->Move(-100, -100);
  }

  const int base = chat.geometry.height - this->m_inputAreaHeight;
  const int firstY = base + 3;
  const int lastY = base + 7;
  this->m_inputButtons[0]->Move(97, firstY);
  this->m_inputButtons[1]->Move(586, lastY);
  this->m_inputButtons[2]->Move(572, lastY);

  if (this->m_suggestions) {
    UIWindow* edit = this->m_chatEdit;
    this->m_suggestions->Move(edit->geometry.x, edit->geometry.y - this->m_suggestions->geometry.height);
  }

  this->ApplyHeaderLayout();
  this->ApplyInputLayout();
}

// OFFSET: 0x00916170
// FUNCTION: fn_12582
void UINewChatWnd::ToggleInput() {
  if (g_inputSuggestionsEnabled && g_windowCloseByte) {
    if (*this->m_chatEdit->Text() == '\0') {
      g_inputSuggestionsEnabled = false;
      this->ApplyInputLayout();
      SetPressedWindowRoot(g_windowMgr.window_4F1D4, nullptr);
    }
  } else {
    g_inputSuggestionsEnabled = true;
    this->ApplyInputLayout();
    g_windowMgr.SetFocus(this->m_inputEdit);
    this->m_inputEdit->SetText("");

    if (!g_windowCloseByte) {
      this->ResizeRows(static_cast<int>(this->geometry.unknown_0030));
    }

    SetPressedWindowRoot(g_windowMgr.window_4F1D4, this->m_inputEdit);
  }

  if (UIItemTagOnChat* tags = static_cast<UIChatEditCtrl*>(this->m_chatEdit)->unknown_0144) {
    reinterpret_cast<ItemTagRetirement*>(tags)->Clear();
  }
}
