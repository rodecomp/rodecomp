#include "UI/Window/UIWindowMgr.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Chat/UISubChatHisWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Client/Session.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UITabControl.h"

// OFFSET: 0x0091E7E0
// FUNCTION: fn_12628
bool UINewChatWnd::SuggestionsVisible() const {
  return (this->m_suggestions ? this->m_suggestions->geometry.visible : 0) != 0;
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
  if (g_windowMgr.inputSuggestionsEnabled) {
    this->unknown_00C0->Move(6, this->geometry.height - this->m_inputAreaHeight + 4);
    this->m_chatEdit->Move(110, this->geometry.height - this->m_inputAreaHeight + 4);
    UIWindow* child = this->unknown_00B8;
    child->Move(3, this->geometry.height - child->geometry.height);

    this->m_inputButtons[0]->Move(97, this->geometry.height - this->m_inputAreaHeight + 3);
    this->m_inputButtons[1]->Move(586, this->geometry.height - this->m_inputAreaHeight + 7);
    this->m_inputButtons[2]->Move(572, this->geometry.height - this->m_inputAreaHeight + 7);
  } else {
    this->unknown_00C0->Move(-100, -100);
    this->m_chatEdit->Move(-100, -100);
    this->unknown_00B8->Move(-100, -100);
    for (int index = 0; index < 3; ++index) {
      this->m_inputButtons[index]->Move(-100, -100);
    }
  }

  this->Invalidate();
}

// OFFSET: 0x009337F0
// FUNCTION: fn_12730
void UINewChatWnd::ApplyHeaderLayout() {
  if (!g_session.m_canDragWnd && this->geometry.unknown_0030 != 0) {
    this->m_headerButtons[1]->Move(-100, -100);
    this->m_headerButtons[0]->Move(-100, -100);
    this->unknown_0128->Move(-100, -100);
    this->m_chatModeButton->Move(this->m_lockButtonX - this->m_chatModeButton->geometry.width - 2, 4);
    return;
  }

  if (this->geometry.unknown_0030 == 0) {
    this->m_headerButtons[1]->Move(-100, -100);
    this->m_headerButtons[0]->Move(-100, -100);
    this->unknown_0128->Move(-100, -100);
    this->m_chatModeButton->Move(-100, -100);
    return;
  }

  UIWindow* button = this->m_headerButtons[1];
  int x = this->m_lockButtonX - button->geometry.width - 2;
  button->Move(x, 4);
  button = this->m_headerButtons[0];
  x -= (button->geometry.width + 2);
  button->Move(x, 4);
  button = this->unknown_0128;
  x -= (button->geometry.width + 2);
  button->Move(x, 4);
  button = this->m_chatModeButton;
  x -= (button->geometry.width + 2);
  button->Move(x, 4);
}

// OFFSET: 0x009338F0
// FUNCTION: fn_12731
void UISubChatHisWnd::ApplyButtonLayout() {
  if (!g_session.m_canDragWnd) {
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
    int index = 0;
    for (std::vector<UIChatHisBox*>::iterator it = this->m_tabs.begin(); it != this->m_tabs.end(); ++it, ++index) {
      (*it)->Resize(chat.geometry.width - 8, height - this->m_inputOffsetY - this->m_inputAreaHeight - 4);
      (*it)->Invalidate();

      if (index == this->m_currentTab) {
        (*it)->Move(5, this->m_inputOffsetY + 2);
      } else {
        (*it)->Move(-100 - chat.geometry.width, -100);
      }
    }

    chat.Invalidate();
  } else {
    for (std::vector<UIChatHisBox*>::iterator it = this->m_tabs.begin(); it != this->m_tabs.end(); ++it) {
      (*it)->Move(-100 - chat.geometry.width, -100);
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
  const POINT positions[] = {{97, base + 3}, {586, base + 7}, {572, base + 7}};
  for (int index = 0; index < 3; ++index) {
    this->m_inputButtons[index]->Move(positions[index].x, positions[index].y);
  }

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
  if (g_windowMgr.inputSuggestionsEnabled && g_windowMgr.chatModeChanged) {
    if (strlen(this->m_chatEdit->Text()) == 0) {
      g_windowMgr.inputSuggestionsEnabled = false;
      this->ApplyInputLayout();
      g_windowMgr.SetPressedWindowRoot(nullptr);
    }
  } else {
    g_windowMgr.inputSuggestionsEnabled = true;
    this->ApplyInputLayout();
    g_windowMgr.SetFocus(this->m_inputEdit);
    this->m_inputEdit->SetText("");

    if (!g_windowMgr.chatModeChanged) {
      this->ResizeRows(static_cast<int>(this->geometry.unknown_0030));
    }

    g_windowMgr.SetPressedWindowRoot(this->m_inputEdit);
  }

  if (UIItemTagOnChat* tags = static_cast<UIChatEditCtrl*>(this->m_chatEdit)->unknown_0144) {
    tags->ClearTags();
  }
}
