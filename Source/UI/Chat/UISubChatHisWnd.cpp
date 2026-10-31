#include "UI/Chat/UISubChatHisWnd.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Client/Session.h"

#include "Network/TextEncoding.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Chat/ChatSuggestions.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/InputFocus.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstring>
#include <string>

// OFFSET: 0x009312E0
// FUNCTION: fn_12700
void UISubChatHisWnd::Slot25(int x, int y) {
  if (g_chatInputFlag) {
    if (UIWindow* window = g_windowMgr.LookupWindow(132)) {
      window->SendMsg(this, 6, 193, this->m_index, 0, 2);
    }
    this->UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x009309B0
// FUNCTION: fn_12690
void UISubChatHisWnd::Slot26(int x, int y) {
  if (g_chatInputFlag && x > 0 && x < this->m_inputWidth && y > 0 && y < this->m_inputHeight) {
    this->m_edit->SetText("");
    this->m_edit->Resize(this->m_inputWidth - 10, this->m_inputHeight - 2);
    this->m_edit->Move(5, 2);
    InputFocus focus{g_windowMgr.hoverWindow};
    focus.Set(this->m_edit);
  }
}

// OFFSET: 0x00932B70
// FUNCTION: fn_12715
void UISubChatHisWnd::Slot27(int x, int y) {
  if (g_windowMgr.Capture() == this) {
    this->UIFrameWnd::Slot27(x, y);
  }
}

// OFFSET: 0x00931710
// FUNCTION: fn_12706
void UISubChatHisWnd::Slot31(int x, int y) {
  this->UIFrameWnd::Slot31(x, y);
  if (g_chatInputFlag) {
    int screenX = 0;
    int screenY = 0;
    this->ToScreen(screenX, screenY);

    UIWindow* window = g_windowMgr.LookupWindow(1);
    if (window && window->geometry.x <= screenX && screenX <= window->geometry.width + window->geometry.x) {
      const int bottom = this->geometry.height + screenY;
      if (window->geometry.y <= bottom && bottom <= window->geometry.height + window->geometry.y) {
        window->SendMsg(this, 118, 0, 0, 0, 0);
      }
    }
  }
}

// OFFSET: 0x0091EA90
// FUNCTION: fn_12633
void UISubChatHisWnd::Move(int x, int y) {
  this->UIFrameWnd::Move(x, y);
  int screenX = 0;
  int screenY = 0;
  this->ToScreen(screenX, screenY);
  this->m_state.unknown_0000 = screenX;
  this->m_state.unknown_0004 = screenY + this->m_inputHeight;
  g_session.m_subChatWnds[this->m_index].x = screenX;
  g_session.m_subChatWnds[this->m_index].y = screenY;
}

// OFFSET: 0x0091C9E0
// FUNCTION: fn_12619
bool UISubChatHisWnd::Slot42(void* output) {
  *static_cast<SubChatHistoryState*>(output) = this->m_state;
  return true;
}

// OFFSET: 0x00930370
// FUNCTION: fn_12682
void UISubChatHisWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  std::uint32_t color = 0xFFFFFF;
  const TextLocale locale = {};
  this->DrawTextAt(g_fontCache, locale, 5, 4, this->m_name.c_str(), 0, 0, 12, 0, 0, 0);
  DrawColoredText(*this, static_cast<unsigned short>(g_textCodePage), 4, 3, this->m_name.c_str(), 0, color, 0, 12, false);
  this->FillRegion(2, 0, this->m_inputWidth - 5, 1, color);
  this->FillRegion(1, 1, 1, 1, color);
  this->FillRegion(0, 2, 1, this->geometry.height - 4, color);
  this->FillRegion(this->m_inputWidth, this->m_inputHeight, this->geometry.width - this->m_inputWidth - 3, 1, color);
  this->FillRegion(this->m_inputWidth - 3, 0, 1, 1, 0x8E8E8E);
  this->FillRegion(this->m_inputWidth - 2, 1, 1, 1, 0x8E8E8E);
  this->FillRegion(this->m_inputWidth - 1, 2, 1, this->m_inputHeight - 1, 0x8E8E8E);
  this->FillRegion(this->geometry.width - 3, this->m_inputHeight, 1, 1, 0x8E8E8E);
  this->FillRegion(this->geometry.width - 2, this->m_inputHeight + 1, 1, 1, 0x8E8E8E);
  this->FillRegion(this->geometry.width - 1, this->m_inputHeight + 2, 1, this->geometry.height - this->m_inputHeight - 4, 0x8E8E8E);
  this->FillRegion(1, this->geometry.height - 2, 1, 1, 0x8E8E8E);
  this->FillRegion(this->geometry.width - 2, this->geometry.height - 2, 1, 1, 0x8E8E8E);
  this->FillRegion(2, this->geometry.height - 1, this->geometry.width - 4, 1, 0x8E8E8E);
}

// OFFSET: 0x00941020
// FUNCTION: fn_12788
void UISubChatHisWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
  }
}

// OFFSET: 0x0092A710
// FUNCTION: fn_12655
void UISubChatHisWnd::Slot15(int width, int height) {
  {
    const std::string name = "유저인터페이스\\basic_interface\\stickon";
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->m_stickButton = button;
    button->SetBitmap((name + "_a.bmp").c_str(), 0, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 1, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(this->m_inputWidth + 1, 4);
    button->SetId(357);
    this->AddChild(button);
  }

  {
    std::string name = "유저인터페이스\\basic_interface\\battle_option";
    name += "2";
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->m_optionButton = button;
    button->SetBitmap((name + "_a.bmp").c_str(), 0, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 1, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(this->m_inputWidth + 15, 5);
    button->SetId(193);
    this->AddChild(button);
  }

  {
    const std::string name = "유저인터페이스\\basic_interface\\wnd_mini";
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->m_miniButton = button;
    button->SetBitmap((name + "_a.bmp").c_str(), 0, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 1, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->SetId(202);
    this->AddChild(button);
  }

  this->m_chatBox = new UIChatHisBox(*this->m_context, *this->m_listContext);
  this->m_chatBox->Configure(0, 0, this->geometry.width - 8, this->geometry.height - this->unknown_00F0 - this->m_inputHeight - 4, 0);
  this->m_chatBox->Move(5, this->m_inputHeight + 4);
  this->AddChild(this->m_chatBox);

  this->m_resizer = new UIResizer(*this->m_context);
  this->m_resizer->SetSize(14, 14);
  this->m_resizer->SetDirection(7);
  this->m_resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->m_resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  this->m_resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->m_resizer);

  int x = 0;
  int y = 0;
  this->ToScreen(x, y);
  this->m_state.unknown_0000 = x;
  this->m_state.unknown_0014 = 0x66000000;
  this->m_state.unknown_0004 = y + this->m_inputHeight;
  this->m_state.unknown_0008 = this->geometry.width;
  this->m_state.unknown_0018 = 0;
  this->m_state.unknown_000C = this->geometry.height - this->m_inputHeight;
  this->m_state.unknown_0010 = 0;

  const auto found = g_session.m_subChatWnds.find(this->m_index);
  if (found != g_session.m_subChatWnds.end()) {
    this->m_name = found->second.name;
    this->m_inputWidth = this->TextWidth(this->m_controls->fonts, this->m_controls->locale, this->m_name.c_str(), 0, 0, 12, 0, 0) + 10;
    std::memcpy(this->m_options, found->second.options, sizeof(this->m_options));
    if (this->m_inputWidth < 25) {
      this->m_inputWidth = 25;
    }
  }

  this->m_edit = new UIEditCtrl2(*this->m_context, *this->m_editContext);
  this->m_edit->SetSize(100, 19);
  this->m_edit->Move(-100, -100);
  this->AddChild(this->m_edit);

  ChatButtonLayout{g_chatInputFlag, this->geometry.width, {&this->m_miniButton, &this->m_stickButton, &this->m_optionButton}}.Apply();
}
