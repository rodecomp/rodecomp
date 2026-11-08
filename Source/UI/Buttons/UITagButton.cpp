#include "UI/Buttons/UITagButton.h"
#include "UI/Window/UIEnums.h"

#include "UI/Window/UIWindowMgr.h"

#include <windows.h>

// GLOBAL: gl_24936
std::string g_emptyString;

// OFFSET: 0x009B52E0
// FUNCTION: fn_13842
UITagButton::UITagButton() : UITextButton(), item(), m_strings() {
}

// OFFSET: 0x009B54A0
// FUNCTION: fn_13844
void UITagButton::OnLBtnUp(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
    if (g_windowMgr.GetHoverWindow() && this->item.identified) {
      g_windowMgr.GetHoverWindow()->SendMsg(nullptr, 148, reinterpret_cast<std::intptr_t>(&this->item), 0, 0, 0);
    }

    return;
  }

  if (this != g_windowMgr.Capture()) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.state = 0;
  } else {
    this->geometry.state = 1;

    if (this->parent) {
      this->parent->SendMsg(this, UM_TEXT_LBTN_UP, this->geometry.id, this->m_value, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x009B5580
// FUNCTION: fn_13845
void UITagButton::AddString(std::string text) {
  this->m_strings.push_back(text);
}

// OFFSET: 0x009B5600
// FUNCTION: fn_13846
std::string UITagButton::GetString(std::size_t index) {
  if (index >= this->m_strings.size()) {
    return std::string();
  }

  return this->m_strings[index];
}

// OFFSET: 0x009B56C0
// FUNCTION: fn_13847
int UITagButton::GetStringCount() {
  return static_cast<int>(this->m_strings.size());
}
