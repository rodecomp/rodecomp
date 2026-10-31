#include "UI/Edit/UIEditBoxEx.h"
#include "Network/TextEncoding.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIScrollBar.h"

#include <cstdlib>
#include <windows.h>

// OFFSET: 0x009E4310
// FUNCTION: fn_14429
bool CUIEditBoxEx::Slot05() {
  return this->edit->disabled == 0;
}

// OFFSET: 0x009E4330
// FUNCTION: fn_14430
void CUIEditCtrlEx::RefreshPlaceholder() {
  if (this->placeholder.size() == 0) {
    return;
  }

  if (this->text == this->placeholder) {
    return;
  }

  std::uint32_t color;
  if (this->text.size() == 0) {
    this->SetText(this->placeholder.c_str());
    color = this->placeholderColor;
  } else {
    color = this->normalColor;
  }

  this->TextColor(color & 0xFFFFFF, 0xFFFFFFFF);
}

// OFFSET: 0x009E43D0
// FUNCTION: fn_14431
void CUIEditBoxEx::ResetSelection() {
  CUIEditCtrlEx* edit = this->edit;
  edit->unknown_0190 = 0;
  edit->unknown_018C = 0;
  edit->Invalidate();
}

// OFFSET: 0x009E45E0
// FUNCTION: fn_14433
void CUIEditBoxEx::SetCursor(std::size_t position) {
  this->edit->SetCursor(position);
}

// OFFSET: 0x009E4970
// FUNCTION: fn_14437
int CUIEditCtrlEx::LeadingBytesFitting(const std::string& text, int width) {
  if (width == 0) {
    return 0;
  }

  int bytes = 0;
  int total = 0;
  const char* current = text.c_str();
  const char* end = current + text.size();

  while (current < end) {
    const char* start = current;
    current = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
    const int count = static_cast<int>(current - start);
    const int extent = this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, start, count, 0, 12, 0, 0);

    if (width <= total + extent) {
      if (std::abs(total + extent - width) < std::abs(total - width)) {
        bytes += count;
      }
      return bytes;
    }

    bytes += count;
    total += extent;
  }

  return bytes;
}

// OFFSET: 0x009E4A30
// FUNCTION: fn_14438
int CUIEditCtrlEx::TrailingOffsetFitting(const std::string& text, int width) {
  if (width == 0) {
    return static_cast<int>(text.size());
  }

  this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, text.c_str(), 0, 0, 12, 0, 0);

  int bytes = 0;
  int total = 0;
  const char* begin = text.c_str();
  const char* current = begin + text.size();

  while (current > begin) {
    const char* next = current;
    current = CharPrevExA(static_cast<WORD>(g_textCodePage), begin, current, 0);
    const int count = static_cast<int>(next - current);
    const int extent = this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, current, count, 0, 12, 0, 0);

    if (width <= total + extent) {
      if (std::abs(total + extent - width) < std::abs(total - width)) {
        bytes += count;
      }
      return static_cast<int>(text.size()) - bytes;
    }

    bytes += count;
    total += extent;
  }

  return static_cast<int>(text.size()) - bytes;
}

// OFFSET: 0x009E4B20
// FUNCTION: fn_14439
int CUIEditBoxEx::Length() {
  return this->edit->Length();
}

// OFFSET: 0x009E4B50
// FUNCTION: fn_14440
void CUIEditCtrlEx::Invalidate() {
  this->Root()->InvalidateChildren();
}

// OFFSET: 0x009E4B60
// FUNCTION: fn_14441
void CUIEditCtrlEx::SelectAll() {
  UIEditCtrl::SelectAll();
  this->selectedAll = 1;
}

// OFFSET: 0x009E4B80
// FUNCTION: fn_14442
void CUIEditBoxEx::Paint() {
  if (this->borderWidth > 0) {
    this->ClearSurface(this->borderColor);
    const int border = this->borderWidth;
    this->FillRegion(border, border, this->geometry.width - border * 2, this->geometry.height - border * 2, this->backgroundColor);
  } else {
    this->ClearSurface(this->backgroundColor);
  }
}

// OFFSET: 0x009E4BD0
// FUNCTION: fn_14443
void CUIEditCtrlEx::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->PaintText();
}

// OFFSET: 0x009E4F90
// FUNCTION: fn_14445
void CUIEditCtrlEx::UpdateScrollBar() {
  const int total = static_cast<int>(this->lines.size());
  int visible = (this->geometry.height - this->verticalPadding) / this->unknown_0138;
  if (visible < 1) {
    visible = 1;
  }
  this->visibleLines = visible;

  int maximum = total - visible;
  if (maximum < 0) {
    maximum = 0;
  }
  this->scrollMaximum = maximum;

  int position = this->scrollPosition;
  if (this->line < position) {
    position = this->line;
  }
  if (this->line >= visible + position) {
    position = this->line - visible + 1;
  }
  if (position > maximum) {
    position = maximum;
  } else if (position < 0) {
    position = 0;
  }
  this->scrollPosition = position;

  UIScrollBar* scrollBar = static_cast<UIScrollBar*>(this->unknown_0130);
  scrollBar->SetRange(maximum, visible, total);
  scrollBar->SetPosition(this->scrollPosition);
  this->unknown_0134 = this->scrollMaximum > 0;
}

// OFFSET: 0x009E5A60
// FUNCTION: fn_14447
void CUIEditCtrlEx::ResetDragState() {
  this->unknown_01D4 = -1;
  this->unknown_01D8 = 0;
  this->unknown_01DC = 0;
}

// OFFSET: 0x009E5A80
// FUNCTION: fn_14448
void CUIEditBoxEx::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->edit->Resize(width - this->editRight - this->editLeft, height - this->editBottom - this->editTop);
  this->edit->Move(this->editLeft, this->editTop);
}

// OFFSET: 0x009E5AE0
// FUNCTION: fn_14449
int CUIEditBoxEx::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 0 && this->onSubmit) {
    this->onSubmit();
  }
  return UIWindow::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x009E6090
// FUNCTION: fn_14451
void CUIEditBoxEx::SetAlignment(int value) {
  CUIEditCtrlEx* edit = this->edit;
  edit->alignment = value;
  edit->Invalidate();
}

// OFFSET: 0x009E60D0
// FUNCTION: fn_14452
void CUIEditBoxEx::SetBackgroundColor(std::uint32_t color) {
  this->backgroundColor = color;
  this->Invalidate();
}

#include "Client/Input/Language.h"
#include "UI/Chat/ChatSuggestions.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x009E4BF0
// FUNCTION: fn_14444
void CUIEditCtrlEx::HandleKeyDown(int key, std::intptr_t lparam) {
  CLanguage& input = this->m_edit->input;
  const bool password = this->m_edit->password != 0;
  UIWindow* const chatWindow = g_chatSuggestions ? g_chatSuggestions->window : nullptr;

  switch (key) {
  case VK_BACK:
    if (std::abs(input.cursor - input.anchor) == 0 && input.cursor >= 2 && input.cursor - 1 < this->text.size()) {
      const bool lineBreak = this->text.substr(input.cursor - 2, 2) == "\r\n";
      if (lineBreak) {
        input.Character(VK_BACK, lparam, g_textCodePage);
      }
    }
    this->caretStart = std::chrono::system_clock::now();
    break;
  case VK_END:
    if ((static_cast<unsigned short>(GetKeyState(VK_CONTROL)) >> 8) != 0) {
      input.AdvanceCursor(static_cast<int>(this->text.size()) - input.cursor);
    } else {
      input.AdvanceCursor(static_cast<int>(this->lines[this->line].text.size()) - this->column);
      this->unknown_01DC = 1;
    }
    return;
  case VK_HOME:
    if ((static_cast<unsigned short>(GetKeyState(VK_CONTROL)) >> 8) != 0) {
      input.RetreatCursor(input.cursor);
    } else {
      input.RetreatCursor(this->column);
    }
    break;
  case VK_LEFT:
    if (input.cursor >= 2 && input.cursor - 1 < this->text.size()) {
      const bool lineBreak = this->text.substr(input.cursor - 2, 2) == "\r\n";
      if (lineBreak) {
        input.KeyDown(key, lparam, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
      }
    }
    input.KeyDown(key, lparam, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
    this->caretStart = std::chrono::system_clock::now();
    break;
  case VK_UP:
  case VK_DOWN:
    return;
  case VK_RIGHT:
    if (input.cursor >= 0 && input.cursor + 1 < this->text.size()) {
      const bool lineBreak = this->text.substr(input.cursor, 2) == "\r\n";
      if (lineBreak) {
        input.KeyDown(key, lparam, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
      }
    }
    input.KeyDown(key, lparam, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
    this->caretStart = std::chrono::system_clock::now();
    break;
  case VK_INSERT:
  case 'C':
  case 'V':
  case 'X':
    input.KeyDown(key, lparam, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
    break;
  case VK_DELETE:
    if (std::abs(input.cursor - input.anchor) == 0 && input.cursor >= 0 && input.cursor + 1 < this->text.size()) {
      const bool lineBreak = this->text.substr(input.cursor, 2) == "\r\n";
      if (lineBreak) {
        input.KeyDown(key, lparam, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
      }
    }
    input.KeyDown(key, lparam, g_textCodePage, password, g_windowMgr.window_4F1FC, this->m_edit->windows, chatWindow);
    this->caretStart = std::chrono::system_clock::now();
    break;
  case 'A':
    if ((static_cast<unsigned short>(GetKeyState(VK_CONTROL)) >> 8) != 0) {
      input.Select(0, static_cast<int>(this->text.size()));
    }
    break;
  }

  this->ResetDragState();
}

// OFFSET: 0x009E5B20
// FUNCTION: fn_14450
int CUIEditCtrlEx::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  CLanguage& input = this->m_edit->input;
  int position;
  switch (message) {
  case 0:
    if (reinterpret_cast<const unsigned char*>(&this->unknown_0184)[1] != 0 && (static_cast<unsigned short>(GetKeyState(VK_SHIFT)) >> 8) != 0) {
      input.Insert("\r\n");
      return 0;
    }
    UIWindow::SendMsg(sender, 0, a, b, c, d);
    return 0;
  case 7:
    position = this->scrollPosition + a;
    break;
  case 9:
    position = this->unknown_01F0 + this->scrollPosition;
    break;
  case 10:
    position = this->scrollPosition - this->unknown_01F0;
    break;
  case 18:
    if (this->line > 0) {
      if (this->unknown_01D4 == this->line - 1) {
        input.RetreatCursor(this->unknown_01D8);
      } else {
        const EditLine& previous = this->lines[this->line - 1];
        const std::string& current = this->lines[this->line].text;
        int offset;
        if (this->alignment == 2) {
          const int width = this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, current.c_str(), 0, 0, 12, 0, 0);
          const int columnWidth =
            this->column > 0 ? this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, current.c_str(), this->column, 0, 12, 0, 0) : 0;
          offset = this->TrailingOffsetFitting(previous.text, width - columnWidth);
        } else {
          const int columnWidth =
            this->column > 0 ? this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, current.c_str(), this->column, 0, 12, 0, 0) : 0;
          offset = this->LeadingBytesFitting(previous.text, columnWidth);
        }

        const int delta = (previous.unknown_0018 != 0 ? 2 : 0) + static_cast<int>(previous.text.size()) - offset + this->column;
        input.RetreatCursor(delta);
        this->unknown_01D8 = delta;
      }
      this->unknown_01D4 = this->line;
      this->unknown_01DC = 1;
    }
    this->selectedAll = 1;
    return 0;
  case 19:
    if (static_cast<unsigned>(this->line) < this->lines.size() - 1) {
      if (this->unknown_01D4 == this->line + 1) {
        input.AdvanceCursor(this->unknown_01D8);
      } else {
        const EditLine& next = this->lines[this->line + 1];
        const std::string& current = this->lines[this->line].text;
        int offset;
        if (this->alignment == 2) {
          const int width = this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, current.c_str(), 0, 0, 12, 0, 0);
          const int columnWidth =
            this->column > 0 ? this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, current.c_str(), this->column, 0, 12, 0, 0) : 0;
          offset = this->TrailingOffsetFitting(next.text, width - columnWidth);
        } else {
          const int columnWidth =
            this->column > 0 ? this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, current.c_str(), this->column, 0, 12, 0, 0) : 0;
          offset = this->LeadingBytesFitting(next.text, columnWidth);
        }

        const EditLine& line = this->lines[this->line];
        const int delta = (line.unknown_0018 != 0 ? 2 : 0) + static_cast<int>(line.text.size()) - this->column + offset;
        input.AdvanceCursor(delta);
        this->unknown_01D8 = delta;
      }
      this->unknown_01D4 = this->line;
      this->unknown_01DC = 1;
    }
    this->selectedAll = 1;
    return 0;
  case WM_KEYDOWN:
    this->HandleKeyDown(a, b);
    this->selectedAll = 1;
    return 3;
  case WM_CHAR:
    if (this->unknown_01F8 != 0) {
      if (this->unknown_01F8 == 1) {
        if ((a < '0' || a > '9') && a != VK_BACK) {
          return 3;
        }
      } else if (this->unknown_01F8 == 2) {
        if ((a < '0' || a > '9') && a != '.' && a != VK_BACK) {
          return 3;
        }
      } else {
        return 3;
      }
    }
    input.Character(a, b, g_textCodePage);
    this->selectedAll = 1;
    return 3;
  case WM_IME_COMPOSITION:
    this->selectedAll = 1;
    return 0;
  default:
    UIEditCtrl::SendMsg(sender, message, a, b, c, d);
    return 0;
  }

  if (position > this->scrollMaximum) {
    position = this->scrollMaximum;
  } else if (position < 0) {
    position = 0;
  }
  this->scrollPosition = position;
  static_cast<UIScrollBar*>(this->unknown_0130)->SetPosition(position);
  this->unknown_0134 = this->scrollMaximum > 0;
  return 0;
}

// OFFSET: 0x009E6300
// FUNCTION: fn_14453
void CUIEditBoxEx::SetFocus() {
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->edit);
}

// OFFSET: 0x009E6320
// FUNCTION: fn_14454
void CUIEditBoxEx::SetTextColor(std::uint32_t color) {
  CUIEditCtrlEx* edit = this->edit;
  edit->normalColor = color;
  edit->textColor = color;
  edit->selectedAll = 1;
}

// OFFSET: 0x009E6370
// FUNCTION: fn_14455
void CUIEditBoxEx::SetHighlight(int value) {
  CUIEditCtrlEx* edit = this->edit;
  edit->unknown_018C = value;
  edit->unknown_0190 = 1;
  edit->Invalidate();
}

// OFFSET: 0x009E63C0
// FUNCTION: fn_14456
void CUIEditBoxEx::SetBorderColor(std::uint32_t color) {
  this->borderColor = color;
  this->Invalidate();
}

// OFFSET: 0x009E63E0
// FUNCTION: fn_14457
void CUIEditBoxEx::SetBorderWidth(int width) {
  this->borderWidth = width;
  this->Invalidate();
}

// OFFSET: 0x009E6430
// FUNCTION: fn_14458
void CUIEditBoxEx::SetMaximum(int value) {
  this->edit->maximum = value;
  this->edit->SetText(this->edit->Text());
}

// OFFSET: 0x009E6470
// FUNCTION: fn_14459
void CUIEditBoxEx::SetEditBounds(int left, int top, int right, int bottom) {
  this->editLeft = left;
  this->editTop = top;
  this->editRight = right;
  this->editBottom = bottom;
  this->Resize(this->geometry.width, this->geometry.height);
}

// OFFSET: 0x009E64B0
// FUNCTION: fn_14460
void CUIEditBoxEx::SetMask(char value) {
  this->edit->mask = value;
}

// OFFSET: 0x009E64D0
// FUNCTION: fn_14461
void CUIEditBoxEx::SetText(const std::string& text) {
  this->edit->SetText(text.c_str());
}

#include <chrono>

// OFFSET: 0x009E4410
// FUNCTION: fn_14432
void CUIEditBoxEx::Create(int width, int height, bool withScrollBar) {
  UIEditContext& edits = this->edit->EditContext();
  this->RemoveChild(this->edit, 1);
  CUIEditCtrlEx* edit = new CUIEditCtrlEx(*this->m_context, edits);
  this->edit = edit;
  edit->SetSize(width, height);
  if (withScrollBar) {
    reinterpret_cast<unsigned char*>(&edit->unknown_0184)[1] = 1;
  }
  this->AddChild(edit);
  this->SetSize(width, height);
}

#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x009E4660
// FUNCTION: fn_14435
void CUIEditCtrlEx::PaintTextLines() {
  if (this->selectedAll) {
    void** vtable = *reinterpret_cast<void***>(this);
    reinterpret_cast<void(__thiscall*)(CUIEditCtrlEx*)>(vtable[56])(this);
    this->selectedAll = 0;
  }

  int width = this->geometry.width;
  if (reinterpret_cast<const unsigned char*>(&this->unknown_0184)[1] != 0) {
    width -= this->unknown_0130->geometry.width;
  }

  if (this->lines.empty()) {
    int x = this->unknown_01B4;
    if (this->alignment == 2) {
      const int caretWidth = this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, "|", 0, 0, 12, 0, 0);
      x = width - this->unknown_01B4 - caretWidth;
    } else if (this->alignment == 6) {
      x = width / 2;
    }
    this->DrawCaret(x, this->verticalPadding);
    return;
  }

  const int lineCount = static_cast<int>(this->lines.size());
  const int visibleLines = (this->geometry.height - this->verticalPadding) / this->unknown_0138;
  int linesToDraw = visibleLines;
  if (linesToDraw < 1) {
    linesToDraw = 1;
  }

  int endLine = this->scrollPosition + linesToDraw;
  if (lineCount < endLine) {
    endLine = lineCount;
  }

  for (int lineIndex = this->scrollPosition; lineIndex < endLine; ++lineIndex) {
    EditLine& line = this->lines[lineIndex];
    const char* text = line.text.c_str();
    const int textWidth = this->TextWidth(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, text, 0, 0, 12, 0, 0);

    int x = this->unknown_01B4;
    if (this->alignment == 2) {
      x = width - x - textWidth;
    } else if (this->alignment == 6) {
      x = (width - textWidth) / 2;
    }
    const int y = (lineIndex - this->scrollPosition) * this->unknown_0138 + this->verticalPadding;

    if (!line.text.empty()) {
      if (g_windowMgr.GetHoverWindow() == this && line.selectionEnd > line.selectionBegin) {
        int selectionBeginWidth = 0;
        if (line.selectionBegin >= 1) {
          selectionBeginWidth = this->TextWidth(
            this->m_edit->buttons.fonts, this->m_edit->buttons.locale, text, static_cast<unsigned>(line.selectionBegin), 0, 12, 0, 0
          );
        }

        int selectionEndWidth = 0;
        if (line.selectionEnd >= 1) {
          selectionEndWidth = this->TextWidth(
            this->m_edit->buttons.fonts, this->m_edit->buttons.locale, text, static_cast<unsigned>(line.selectionEnd), 0, 12, 0, 0
          );
        }

        this->DrawSelectionAt(x + selectionBeginWidth, y, selectionEndWidth - selectionBeginWidth);
      }

      this->PlaceLinks(line.text, y, lineIndex, this->scrollPosition, endLine);

      if (this->unknown_0190 == 0) {
        this->DrawTextAt(
          this->m_edit->buttons.fonts, this->m_edit->buttons.locale, x, this->verticalPadding + y, text, 0, 0, 12, this->textColor, 0, 0
        );
      } else {
        this->DrawEmboldenedText(x, this->verticalPadding + y, text, this->textColor, this->secondColor, 0, 0, 12);
      }

      this->PlaceTags(x, y, this->scrollPosition, endLine, lineIndex, this->unknown_013C);
    }

    if (g_windowMgr.GetHoverWindow() == this && lineIndex == this->line) {
      int caretOffset = 0;
      if (this->column >= 1) {
        caretOffset = this->TextWidth(
          this->m_edit->buttons.fonts, this->m_edit->buttons.locale, text, static_cast<unsigned>(this->column), 0, 12, 0, 0
        );
      }
      this->DrawCaret(x - 2 + caretOffset, y);
    }
  }
}

CUIEditCtrlEx::CUIEditCtrlEx(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl3(windows, edit) {
  this->normalColor = 0;
  this->unknown_018C = 0;
  this->unknown_0190 = 0;
  this->placeholderColor = 0x808080;
  this->alignment = 0;
  this->unknown_01B4 = 3;
  this->verticalPadding = 1;
  this->selectedAll = 0;
  this->column = 0;
  this->line = 0;
  this->unknown_01D4 = -1;
  this->unknown_01D8 = 0;
  this->unknown_01DC = 0;
  this->caretStart = std::chrono::system_clock::now();
  this->scrollMaximum = 0;
  this->visibleLines = 0;
  this->unknown_01F0 = 1;
  this->scrollPosition = 0;
  this->unknown_01F8 = 0;
}

// OFFSET: 0x009E45F0
// FUNCTION: fn_14434
void CUIEditCtrlEx::DrawCaret(int x, int y) {
  const int elapsed = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->caretStart).count());
  if (((elapsed / 500) & 1) == 0 && IsInputSuggestionAllowed()) {
    this->DrawTextAt(this->m_edit->buttons.fonts, this->m_edit->buttons.locale, x, y, "|", 0, 0, 12, this->textColor, 0, 0);
  }
}
