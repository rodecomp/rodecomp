#include "Client/Input/InputCharset.h"
#include "Client/Input/Language.h"
#include "Contents/SkinMgr.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Chat/ChatSuggestions.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIScrollBar.h"
#include "Network/TextEncoding.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"
#include <cstring>
#include <windows.h>

// OFFSET: 0x0084F250
// FUNCTION: fn_10657
UIEditCtrl2::UIEditCtrl2(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit) {
}

// OFFSET: 0x008515E0
// OFFSET: 0x00852700
// FUNCTION: fn_10692
UIEditCtrl2::~UIEditCtrl2() = default;

// OFFSET: 0x00863BF0
// FUNCTION: fn_10911
int UIEditCtrl2::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 0) {
    this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  } else {
    InputFocus& focus = this->m_edit->focus;
    if (focus.IsLast(focus.window)) {
      focus.First();
    } else {
      focus.Next();
    }

    this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x00863C70
// FUNCTION: fn_10912
int UIEditCtrl3::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  using NextCharHandler = const char*(__thiscall*)(InputCharset*, const char*, int);
  CLanguage& input = this->m_edit->input;
  unsigned char* const scrolling = reinterpret_cast<unsigned char*>(&this->unknown_015C);
  unsigned char* const lineMove = reinterpret_cast<unsigned char*>(&this->unknown_0184);

  if (message == 0) {
    if (lineMove[1] == 0 || (static_cast<unsigned short>(GetKeyState(VK_SHIFT)) >> 8) != 0 || lineMove[1] == 0) {
      char newline[] = "\r\n";
      input.Insert(newline);
      return 0;
    }

    this->UIWindow::SendMsg(sender, 0, a, b, c, d);
    return 0;
  }

  if (message == 7 || message == 9 || message == 10) {
    const int previous = this->unknown_0158;
    const int visible = this->geometry.height / this->unknown_0138;
    scrolling[0] = 1;

    if (message == 9) {
      this->unknown_0158 = previous - visible + 1;
    } else if (message == 10) {
      this->unknown_0158 = previous - 1 + visible;
    } else {
      this->unknown_0158 = previous + a;
    }

    if (this->unknown_0158 < 0) {
      this->unknown_0158 = 0;
    }

    const int last = this->unknown_013C - this->geometry.height / this->unknown_0138;
    if (this->unknown_0158 > last) {
      this->unknown_0158 = last;
    }

    static_cast<UIScrollBar*>(this->unknown_0130)->SetPosition(this->unknown_0158);
    this->unknown_0130->Invalidate();
    this->Invalidate();
    if (this->parent) {
      this->parent->Invalidate();
    }

    return this->unknown_0158 - previous;
  }

  if (message == 18) {
    lineMove[0] = 0;

    const unsigned count = this->unknown_0168.size();
    if (count < 2 && (count != 1 || !this->unknown_0174[0])) {
      return 0;
    }

    input.Selection(this->anchor, this->cursor);
    if (this->unknown_0150 < 1 || this->unknown_0154 != this->cursor || scrolling[1] != 0) {
      const char* current = this->unknown_0160;
      this->unknown_015E = 0;
      int bytes = 0;
      int width = 0;
      const char* const end = current + std::strlen(current);
      int column = 0;

      if (this->unknown_0144 != 0) {
        while (current < end) {
          const int previousBytes = bytes;
          const int previousWidth = width;
          const char* const start = current;
          current = reinterpret_cast<NextCharHandler>((*reinterpret_cast<void***>(g_serverLanguage))[5])(g_serverLanguage, start, static_cast<int>(end - start));
          const int length = static_cast<int>(current - start);
          const TextLocale locale = {};
          width += this->TextWidth(g_fontCache, locale, start, length, 0, 12, 0, 0);
          bytes += length;

          if (length == 1 && *start == '\n') {
            column = bytes - 1;
            break;
          }

          column = bytes;
          if (width > this->unknown_014C) {
            if (width - this->unknown_014C > this->unknown_014C - previousWidth) {
              column = previousBytes;
            }
            break;
          }
        }
      }

      this->unknown_0150 = static_cast<int>(std::strlen(this->unknown_0160)) + this->unknown_0144 - column;
    }

    input.RetreatCursor(this->unknown_0150);
    scrolling[1] = 1;
    lineMove[0] = 1;
  } else if (message == 19) {
    lineMove[0] = 0;

    const unsigned count = this->unknown_0168.size();
    if ((count < 2 || count - 1 <= static_cast<unsigned>(this->textY)) && (count != 1 || !this->unknown_0174[0] || this->unknown_0148 < 1)) {
      return 0;
    }

    input.Selection(this->anchor, this->cursor);
    if (this->unknown_0150 < 1 || this->unknown_0154 != this->cursor || this->unknown_015E != 0) {
      scrolling[1] = 0;
      int bytes = 0;
      int width = 0;
      int column = 0;
      const unsigned lines = this->unknown_0168.size();

      if (lines >= 2 && lines - 1 > static_cast<unsigned>(this->textY)) {
        const char* current = this->unknown_0164;
        const char* const end = current + std::strlen(current);

        while (current < end) {
          const int previousBytes = bytes;
          const int previousWidth = width;
          const char* const start = current;
          current = reinterpret_cast<NextCharHandler>((*reinterpret_cast<void***>(g_serverLanguage))[5])(g_serverLanguage, start, static_cast<int>(end - start));
          const int length = static_cast<int>(current - start);
          const TextLocale locale = {};
          width += this->TextWidth(g_fontCache, locale, start, length, 0, 12, 0, 0);
          bytes += length;

          if (length == 1 && *start == '\n') {
            column = bytes - 1;
            break;
          }

          column = bytes;
          if (width > this->unknown_014C) {
            if (width - this->unknown_014C > this->unknown_014C - previousWidth) {
              column = previousBytes;
            }
            break;
          }
        }
      }

      this->unknown_0150 = this->unknown_0148 + column;
    }

    input.AdvanceCursor(this->unknown_0150);
    this->unknown_015E = 1;
  } else if (message == 256) {
    scrolling[0] = 0;
    this->HandleInputMessage(a, b);
    return 3;
  } else {
    this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
    return 0;
  }

  input.Selection(this->anchor, this->cursor);
  this->unknown_0154 = this->cursor;
  return 0;
}

// OFFSET: 0x0085E420
// FUNCTION: fn_10829
void UIEditCtrl3::HandleInputMessage(unsigned key, std::intptr_t data) {
  CLanguage& input = this->m_edit->input;
  const bool password = this->m_edit->password != 0;
  UIWindow* const chatWindow = g_chatSuggestions ? g_chatSuggestions->window : nullptr;

  this->unknown_0150 = 0;
  this->unknown_0154 = 0;

  switch (key) {
  case VK_BACK:
    if (this->unknown_0168.size() != 0) {
      input.Selection(this->anchor, this->cursor);
      int length = 0;
      for (int line = 0; line < this->textY; ++line) {
        length += static_cast<int>(std::strlen(this->unknown_0168[line].c_str()));
      }
      if (length == this->cursor && this->unknown_0174[this->textY - 1]) {
        input.Character(VK_BACK, data, g_textCodePage);
      }
    }
    break;
  case VK_END:
    if (this->unknown_0168.size() != 0) {
      input.Selection(this->anchor, this->cursor);
      int offset = this->cursor;
      for (int line = 0; line < this->textY; ++line) {
        offset -= static_cast<int>(std::strlen(this->unknown_0168[line].c_str()));
      }
      const int length = static_cast<int>(std::strlen(this->unknown_0168[this->textY].c_str()));
      input.AdvanceCursor(this->unknown_0174[this->textY] ? length - offset - 2 : length - offset);
      input.Selection(this->anchor, this->cursor);
    }
    reinterpret_cast<unsigned char*>(&this->unknown_0184)[0] = 1;
    return;
  case VK_HOME:
    if (this->unknown_0168.size() != 0) {
      input.Selection(this->anchor, this->cursor);
      int offset = this->cursor;
      for (int line = 0; line < this->textY; ++line) {
        offset -= static_cast<int>(std::strlen(this->unknown_0168[line].c_str()));
      }
      input.RetreatCursor(offset);
      input.Selection(this->anchor, this->cursor);
    }
    break;
  case VK_LEFT:
    if (this->textY > 0) {
      input.Selection(this->anchor, this->cursor);
      int length = 0;
      for (int line = 0; line < this->textY; ++line) {
        length += static_cast<int>(std::strlen(this->unknown_0168[line].c_str()));
      }
      if (length == this->cursor && this->unknown_0174[this->textY - 1]) {
        input.KeyDown(key, data, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
      }
    }
    input.KeyDown(key, data, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
    break;
  case VK_RIGHT:
    input.Selection(this->anchor, this->cursor);
    if (static_cast<unsigned>(this->textY) < this->unknown_0168.size()) {
      int length = 0;
      for (int line = 0; line <= this->textY; ++line) {
        length += static_cast<int>(std::strlen(this->unknown_0168[line].c_str()));
      }
      if (length - 2 == this->cursor && this->unknown_0174[this->textY]) {
        input.KeyDown(key, data, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
        input.KeyDown(key, data, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
        return;
      }
    }
    input.KeyDown(key, data, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
    break;
  case VK_INSERT:
  case VK_DELETE:
  case 'A':
  case 'C':
  case 'V':
  case 'X':
    input.KeyDown(key, data, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
    break;
  }
}

// OFFSET: 0x0084F570
// FUNCTION: fn_10659
UIEditCtrl3::UIEditCtrl3(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl2(windows, edit) {
  this->unknown_0130 = 0;
  this->unknown_0134 = 0;
  this->unknown_0138 = 14;
  this->unknown_013C = 0;
  this->unknown_0140 = 0;
  this->unknown_0144 = 0;
  this->unknown_0148 = 0;
  this->unknown_014C = 0;
  this->unknown_0150 = 0;
  this->unknown_0154 = 0;
  this->unknown_0158 = 0;
  this->unknown_015C = 0;
  this->unknown_015E = 0;
  this->unknown_0184 = 0;
  this->unknown_0160 = 0;
  this->unknown_0164 = 0;
}

// OFFSET: 0x0084F2B0
// FUNCTION: fn_10658
UIEditCtrl3::UIEditCtrl3(const UIEditCtrl3& other) : UIEditCtrl2(other) {
  this->unknown_0130 = 0;
  this->UIEditCtrl3::Slot15(other.geometry.width, other.geometry.height);

  this->unknown_0134 = other.unknown_0134;
  this->unknown_0138 = other.unknown_0138;
  this->unknown_013C = other.unknown_013C;
  this->unknown_0140 = other.unknown_0140;
  this->unknown_0144 = other.unknown_0144;
  this->unknown_0148 = other.unknown_0148;
  this->unknown_014C = other.unknown_014C;
  this->unknown_0150 = other.unknown_0150;
  this->unknown_0154 = other.unknown_0154;
  this->unknown_0158 = other.unknown_0158;
  this->unknown_015C = other.unknown_015C;
  this->unknown_015E = other.unknown_015E;
  this->unknown_0184 = other.unknown_0184;

  char* first = other.unknown_0160;
  if (first != this->unknown_0160) {
    if (IsNullOrEmpty(first)) {
      this->unknown_0160 = 0;
    } else {
      const std::size_t length = std::strlen(first) + 1;
      this->unknown_0160 = new char[length];
      strcpy_s(this->unknown_0160, length, first);
    }
  }

  char* second = other.unknown_0164;
  if (second != this->unknown_0164) {
    if (IsNullOrEmpty(second)) {
      this->unknown_0164 = 0;
    } else {
      const std::size_t length = std::strlen(second) + 1;
      this->unknown_0164 = new char[length];
      strcpy_s(this->unknown_0164, length, second);
    }
  }
}

// OFFSET: 0x00851630
// OFFSET: 0x00852770
// FUNCTION: fn_10693
// FUNCTION: fn_10715
UIEditCtrl3::~UIEditCtrl3() {
  this->ClearLineState();
  this->pendingText.clear();
}

// OFFSET: 0x0085AC40
// FUNCTION: fn_10786
void UIEditCtrl3::Slot15(int width, int height) {
  auto* scrollBar = new UIScrollBar(*this->m_context);
  this->unknown_0130 = scrollBar;
  scrollBar->Configure(width - 10, 0, 1, height);
  this->AddChild(scrollBar);
}

// OFFSET: 0x00863280
// FUNCTION: fn_10906
void UIEditCtrl3::Resize(int width, int height) {
  this->UIWindow::Resize(width, height);
  this->unknown_0130->Move(this->geometry.width - 10, 0);
  this->unknown_0130->Resize(this->unknown_0130->geometry.width, height);
  using ResizeHandler = void(__thiscall*)(UIEditCtrl3*);
  reinterpret_cast<ResizeHandler>((*reinterpret_cast<void***>(this))[58])(this);
}

// OFFSET: 0x00865220
// FUNCTION: fn_10922
int UIWhisperEditCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 18) {
    if (!this->history.empty()) {
      if (this->historyIndex != 0) {
        --this->historyIndex;
      }
      this->SetText(this->history[this->historyIndex].c_str());
      this->Slot23();
    }
  } else if (message == 19) {
    if (!this->history.empty() && static_cast<std::size_t>(this->historyIndex) < this->history.size()) {
      ++this->historyIndex;
      this->SetText(this->historyIndex == static_cast<int>(this->history.size()) ? "" : this->history[this->historyIndex].c_str());
      this->Slot23();
    }
  } else {
    return this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x00867FC0

// OFFSET: 0x00853760
// FUNCTION: fn_10741
void UIEditCtrl_OldList::RecallHistory(int step) {
  if (this->historyIndex == -1) {
    return;
  }

  this->historyIndex += step;
  if (this->historyIndex < 0) {
    this->historyIndex = static_cast<int>(this->history.size()) - 1;
  }

  if (static_cast<std::size_t>(this->historyIndex) >= this->history.size()) {
    this->historyIndex = 0;
  }

  this->SetText(this->history[this->historyIndex].c_str());
  this->Slot23();
  this->Invalidate();
}

// OFFSET: 0x00853810
// FUNCTION: fn_10742
int UIEditCtrl::MeasureText(const std::string& text, const int& bytes) {
  const TextLocale locale = {};
  return this->TextWidth(g_fontCache, locale, text.c_str(), bytes, 0, 12, 0, 0);
}

// OFFSET: 0x0084F7B0
// FUNCTION: fn_10660
UIEditCtrl4::UIEditCtrl4(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit), placeholderColor(0xFFFFFFFFu), placeholderEnabled(0) {
}

// OFFSET: 0x00852870
UIEditCtrl4::~UIEditCtrl4() = default;

// OFFSET: 0x0085AA40
// FUNCTION: fn_10782
void UIEditCtrl4::Slot23() {
  this->text.clear();
  this->m_edit->input.ResetInput();
  this->m_edit->input.ResetInput();
  this->m_edit->input.Insert(this->text.c_str());
  this->m_edit->password = this->mask != 0;
  this->SelectAll();
}

// OFFSET: 0x008563D0
// FUNCTION: fn_10754
void UIEditCtrl4::PaintText() {
  this->PaintTextWithPolicy(true, &this->placeholder, this->placeholderColor, this->placeholderEnabled == 1);
}

// OFFSET: 0x00869800
// FUNCTION: fn_10972
void UIEditCtrl4::SetPlaceholderEnabled(bool enabled) {
  this->placeholderEnabled = enabled;
}

// OFFSET: 0x00867960
// FUNCTION: fn_10943
void UIEditCtrl4::SetPlaceholder(const char* value) {
  this->placeholder.assign(value, std::strlen(value));
  this->Trim();

  if (this->m_edit->focus.window == this) {
    this->m_edit->input.ResetInput();
    this->m_edit->input.ResetInput();
    this->m_edit->input.Insert(this->placeholder.c_str());
  }

  this->SelectAll();

  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x008679F0
// FUNCTION: fn_10944
void UIEditCtrl4::SetPlaceholderColor(std::uint32_t color) {
  this->placeholderColor = color;
}

// OFFSET: 0x0084F7B0
// FUNCTION: fn_10661
UIEditCtrl5::UIEditCtrl5(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl4(windows, edit) {
}

// OFFSET: 0x008528F0
UIEditCtrl5::~UIEditCtrl5() = default;

// OFFSET: 0x00864140
// FUNCTION: fn_10913
int UIEditCtrl5::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 0) {
    this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  } else {
    InputFocus& focus = this->m_edit->focus;
    if (focus.IsLast(focus.window)) {
      focus.First();
    } else {
      focus.Next();
    }

    this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0085CBF0
// FUNCTION: fn_10806
void UIEditCtrl3::Paint() {
  const bool selected = g_skinInfo.Selected();
  const SkinPaletteCell cell = {1, 2};
  const std::uint32_t color = SkinPaletteColor(ResourceMgr::Instance(), cell, selected);
  this->red = color & 0xFF;
  this->blue = (color >> 16) & 0xFF;
  this->green = (color & 0xFFFF) >> 8;

  this->Invalidate();
  this->ClearSurface(static_cast<unsigned char>(this->red) | (static_cast<unsigned char>(this->green) << 8) | (static_cast<unsigned char>(this->blue) << 16));
  this->PaintText();
  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x0085FF60
// FUNCTION: fn_10892
void UIEditCtrl3::Slot35(int value) {
  if (this->unknown_0134 == 0) {
    return;
  }

  if (value < 0) {
    this->SendMsg(this->unknown_0130, 7, 3, 0, 0, 0);
  } else {
    this->SendMsg(this->unknown_0130, 7, -3, 0, 0, 0);
  }
}

// OFFSET: 0x00868300
// FUNCTION: fn_10958
void UIEditCtrl3::Trim() {
  const int limit = this->maximum - this->unknown_00F0;
  const auto length = this->text.size();

  if (static_cast<int>(length) <= limit) {
    return;
  }

  const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), this->text.c_str(), 0);

  while (length - std::strlen(next) <= static_cast<unsigned>(limit)) {
    next = CharNextExA(static_cast<WORD>(g_textCodePage), next, 0);
  }

  const char* previous = CharPrevExA(static_cast<WORD>(g_textCodePage), this->text.c_str(), next, 0);
  this->text.resize(length - std::strlen(previous));

  this->m_edit->input.ResetInput();
  this->m_edit->input.ResetInput();
  this->m_edit->input.Insert(this->text.c_str());

  while (this->text.size() < static_cast<unsigned>(limit)) {
    this->text += " ";
  }
}

// OFFSET: 0x0085BB60
// FUNCTION: fn_10796
void UIBitmapEditCtrl::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str())));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
  this->textY = 3;
  this->PaintText();
}

// OFFSET: 0x00865590
// FUNCTION: fn_10927
void UIBitmapEditCtrl::SetBitmapName(const char* name) {
  this->bitmapName.assign(name, std::strlen(name));

  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str())));
  this->geometry.width = bitmap->image.width;
  this->geometry.height = bitmap->image.height;
}

// OFFSET: 0x0084EE90
// FUNCTION: fn_10654
UIChatEditCtrl::UIChatEditCtrl(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit), unknown_0140(0), historyLimit(30) {
  this->history.reserve(this->historyLimit);
  this->unknown_0144 = 0;
}

// OFFSET: 0x0085EB50
// FUNCTION: fn_10840
void UIChatEditCtrl::Slot25(int x, int y) {
  if (this->disabled == 0) {
    this->m_edit->focus.Set(this);
  }

  this->parent->Slot25(x + this->geometry.x, y + this->geometry.y);
}

// OFFSET: 0x00863640
// FUNCTION: fn_10909
int UIChatEditCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 18) {
    if (this->history.size() != 0) {
      if (this->unknown_0140 == this->history.size()) {
        this->pending = this->Text();
      }
      if (this->unknown_0140 != 0) {
        --this->unknown_0140;
      }

      this->SetText(this->history[this->unknown_0140].c_str());
      this->Slot23();
    }
  } else if (message == 19) {
    if (this->history.size() != 0 && this->unknown_0140 < this->history.size()) {
      ++this->unknown_0140;
      this->SetText(this->unknown_0140 == this->history.size() ? this->pending.c_str() : this->history[this->unknown_0140].c_str());
      this->Slot23();
    }
  } else {
    return UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0085CDE0
// FUNCTION: fn_10808
void UIEditCtrl_OldList::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (!this->bitmapName.empty()) {
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str())));
    this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
  }

  this->PaintText();
}

// OFFSET: 0x0085FFC0
// FUNCTION: fn_10894
void UIEditCtrl_OldList::Slot35(int value) {
  if (value < 0) {
    this->SendMsg(nullptr, 19, 0, 0, 0, 0);
  } else {
    this->SendMsg(nullptr, 18, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0085E3D0
// FUNCTION: fn_10827
void UIEditCtrlVarId::Slot24(int) {
  this->Synchronize();
  this->Invalidate();
  this->SendMsg(this, 0, 0, 0, 0, 0);
}

// OFFSET: 0x0085FFA0
// FUNCTION: fn_10893
void UIEditCtrlVarId::Slot35(int value) {
  if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00864CE0
// FUNCTION: fn_10915
int UIEditCtrlVarId::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 0) {
    return UIEditCtrl::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (this->parent) {
    this->parent->SendMsg(this, 6, this->geometry.id, 0, 0, 0);
  }

  return 0;
}

// OFFSET: 0x00868160
// FUNCTION: fn_10953
void UIEditCtrlVarId::SetText(const char* value) {
  this->UIEditCtrl::SetText(value);
  this->Invalidate();
}

// OFFSET: 0x0085AB10
// FUNCTION: fn_10784
void UIWhisperEditCtrl::Slot23() {
  this->m_edit->input.ResetInput();
  this->m_edit->input.ResetInput();
  this->m_edit->input.Insert(this->text.c_str());
  this->m_edit->password = this->mask != 0;
  this->SelectAll();
  this->SelectAll();
}

// OFFSET: 0x00868230
// FUNCTION: fn_10956
void UIWhisperEditCtrl::SetText(const char* value) {
  this->UIEditCtrl::SetText(value);

  if (std::strlen(value) == 0) {
    this->historyIndex = static_cast<int>(this->history.size());
  }
}

// OFFSET: 0x00864D20
// FUNCTION: fn_10916
int UIEditCtrl_OldList::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 18:
    this->RecallHistory(-1);
    return 0;
  case 19:
    this->RecallHistory(1);
    return 0;
  default:
    return UIEditCtrl::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x00886F90
// FUNCTION: fn_11330
void UIShowAndEditTimeCtrl::Slot15(int, int) {
  for (int i = 0; i < 2; ++i) {
    UIEditCtrlVarId*& field = this->fields[i];
    field = new UIEditCtrlVarId(*this->m_context, *this->m_edit);
    field->SetSize(20, 18);
    field->Background(232, 232, 232);
    field->Border(192, 192, 192);
    field->flat = 0;
    field->maximum = 2;
    field->Move(i * 38, 0);
    field->SetText("0");
    field->SetId(184);
    this->AddChild(field);
  }
}

// OFFSET: 0x008996B0
// FUNCTION: fn_11556
int UIShowAndEditTimeCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6 && argument1 == 184) {
    for (UIEditCtrlVarId* field : this->fields) {
      if (sender == field) {
        int value = std::atoi(field->Text());
        if (value > 60) {
          value = 60;
        }
        const std::string text = FormatOwnedString("%d", value);
        field->SetText(text.c_str());
      }
    }
  }
  return 0;
}

// OFFSET: 0x00853F20
// FUNCTION: fn_10745
void UIEditCtrl3::ClearLineState() {
  while (!this->unknown_0168.empty()) {
    this->unknown_0168.erase(this->unknown_0168.begin());
  }
  this->unknown_0168.clear();

  for (std::vector<bool>::iterator it = this->unknown_0174.begin(); it != this->unknown_0174.end();) {
    it = this->unknown_0174.erase(it);
  }
  this->unknown_0174.clear();

  if (this->unknown_0160) {
    delete[] this->unknown_0160;
    this->unknown_0160 = nullptr;
  }

  if (this->unknown_0164) {
    delete[] this->unknown_0164;
    this->unknown_0164 = nullptr;
  }
}

// OFFSET: 0x0086E7D0
// FUNCTION: fn_11077
UIShowAndEditTimeCtrl::UIShowAndEditTimeCtrl(UIWindowContext& windows) : UIWindow(windows, 0) {
  this->unknown_0084 = 0;
  this->unknown_0088 = 0;
}
