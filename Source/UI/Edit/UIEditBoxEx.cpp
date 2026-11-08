#include "UI/Edit/UIEditBoxEx.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/UIEnums.h"
#include "Network/TextEncoding.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIScrollBar.h"

#include <algorithm>
#include <cstdlib>
#include <windows.h>

// OFFSET: 0x009E4310
// FUNCTION: fn_14429
bool CUIEditBoxEx::IsEditable() {
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
  edit->drawOutline = 0;
  edit->outlineColor = 0;
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
    const int extent = this->TextWidth(start, count, 0, 12, 0, 0);

    if (width > total + extent) {
      bytes += count;
      total += extent;
      continue;
    }

    if (std::abs(total + extent - width) < std::abs(total - width)) {
      bytes += count;
    }
    break;
  }

  return bytes;
}

// OFFSET: 0x009E4A30
// FUNCTION: fn_14438
int CUIEditCtrlEx::TrailingOffsetFitting(const std::string& text, int width) {
  if (width == 0) {
    return static_cast<int>(text.size());
  }

  this->TextWidth(text.c_str(), 0, 0, 12, 0, 0);

  int bytes = 0;
  int total = 0;
  const char* begin = text.c_str();
  const char* current = begin + text.size();

  while (current > begin) {
    const char* previous = current;
    current = CharPrevExA(static_cast<WORD>(g_textCodePage), begin, current, 0);
    const int count = static_cast<int>(previous - current);
    const int extent = this->TextWidth(current, count, 0, 12, 0, 0);

    if (width > total + extent) {
      bytes += count;
      total += extent;
      continue;
    }

    if (std::abs(total + extent - width) < std::abs(total - width)) {
      bytes += count;
    }
    break;
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
  this->visibleLines = std::max(1, (this->geometry.height - this->verticalPadding) / this->lineHeight);
  this->scrollMaximum = std::max(0, total - this->visibleLines);

  this->scrollPosition = std::min(this->scrollPosition, this->line);
  if (this->line >= this->visibleLines + this->scrollPosition) {
    this->scrollPosition = this->line - this->visibleLines + 1;
  }
  this->scrollPosition = std::clamp(this->scrollPosition, 0, this->scrollMaximum);

  static_cast<UIScrollBar*>(this->scrollBar)->SetRange(this->scrollMaximum, this->visibleLines, total);
  static_cast<UIScrollBar*>(this->scrollBar)->SetPosition(this->scrollPosition);
  this->scrollEnabled = this->scrollMaximum > 0;
}

#include "Client/Input/InputCharset.h"
#include "Client/Input/Language.h"
#include "UI/Window/UIWindowMgr.h"
#include <algorithm>

// OFFSET: 0x009E5050
// FUNCTION: fn_14446
void CUIEditCtrlEx::Synchronize() {
  if (this == g_windowMgr.GetHoverWindow()) {
    g_serverLanguage->CopyDisplayText(this->text);
    this->RefreshPlaceholder();
  }
  this->Trim();

  const std::string prefix = g_serverLanguage->Prefix(false);
  const std::string fullText = prefix + this->text;
  std::vector<std::string> lines;
  std::vector<bool> breaks;
  if (reinterpret_cast<const unsigned char*>(&this->editFlags)[1] != 0) {
    int wrapWidth = this->geometry.width - this->scrollBar->geometry.width - this->horizontalPadding;
    this->WrapTextWithBreaks(fullText.c_str(), &lines, &breaks, wrapWidth);
    this->WrapLines(lines);
  } else {
    lines.push_back(fullText);
    breaks.push_back(false);
  }

  this->lines.clear();
  if (!lines.empty()) {
    int anchor = 0;
    int cursor = 0;
    g_serverLanguage->Selection(anchor, cursor);
    if (anchor > cursor) {
      std::swap(anchor, cursor);
    }

    int currentOffset = 0;
    for (const std::string& rawLine : lines) {
      std::size_t crPos = rawLine.find("\r\n");
      EditLine line;
      line.hasCarriageReturn = crPos != std::string::npos;
      line.text = crPos != std::string::npos ? rawLine.substr(0, crPos) : rawLine;

      int lineLen = static_cast<int>(line.text.size());
      std::size_t lineEnd = currentOffset + rawLine.size();
      if (static_cast<std::size_t>(anchor) <= lineEnd) {
        int offset = anchor - currentOffset;
        if (lineLen < offset) {
          line.selectionBegin = lineLen;
        } else {
          line.selectionBegin = std::max(offset, 0);
        }
        line.selectionEnd = lineLen;
      }
      if (static_cast<std::size_t>(cursor) <= lineEnd) {
        int offset = cursor - currentOffset;
        if (lineLen < offset) {
          line.selectionEnd = lineLen;
        } else {
          line.selectionEnd = std::max(offset, 0);
        }
      }
      currentOffset = static_cast<int>(lineEnd);

      this->lines.push_back(line);
    }

    if (this->lines.back().hasCarriageReturn != 0) {
      this->lines.emplace_back();
    }

    int remaining = cursor + static_cast<int>(prefix.size());
    this->line = 0;
    this->column = 0;
    for (std::size_t i = 0; i < this->lines.size(); ++i) {
      int fullLen = static_cast<int>(this->lines[i].text.size()) + (this->lines[i].hasCarriageReturn ? 2 : 0);
      if (remaining - fullLen <= 0) {
        this->column = remaining;
        this->line = static_cast<int>(i);
        break;
      }
      remaining -= fullLen;
    }

    const EditLine& current = this->lines[this->line];
    if (current.hasCarriageReturn != 0 || this->wrapPending == 0) {
      int fullLen = static_cast<int>(current.text.size()) + (current.hasCarriageReturn ? 2 : 0);
      if (remaining == fullLen && static_cast<std::size_t>(this->line) < this->lines.size() - 1) {
        this->column = 0;
        this->line++;
      }
    }

    const int candidateCursor = g_serverLanguage->compositionCursor;
    if (candidateCursor != 0) {
      this->column += candidateCursor;
      int cursorX = 0;
      if (this->column > 0) {
        cursorX = this->TextWidth(this->lines[this->line].text.c_str(), this->column, 0, 12, 0, 0);
      }

      int end = std::max(anchor, cursor) + candidateCursor + static_cast<int>(prefix.size());
      int readingOffset = end - currentOffset;
      int readingX = 0;
      if (readingOffset > 0) {
        readingX = this->TextWidth(this->lines[this->line].text.c_str(), readingOffset, 0, 12, 0, 0);
      }
      int delta = std::max(cursorX - readingX, 2);
      this->MoveCandidateWindow(cursorX + 2 + delta);
      this->MoveReadingWindow(readingX);
    }
  }

  this->UpdateScrollBar();
  this->Invalidate();
}

// OFFSET: 0x009E5A60
// FUNCTION: fn_14447
void CUIEditCtrlEx::ResetDragState() {
  this->previousCursorLine = -1;
  this->cursorAdvance = 0;
  this->wrapPending = 0;
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
  if (message == UM_DEFPUSH && this->onSubmit) {
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
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x009E4BF0
// FUNCTION: fn_14444
void CUIEditCtrlEx::HandleKeyDown(int key, std::intptr_t lparam) {
  UIWindow* const chatWindow = ChatWindow() ? ChatWindow()->m_suggestions : nullptr;

  switch (key) {
  case VK_LEFT:
    if (g_serverLanguage->cursor >= 2 && g_serverLanguage->cursor - 1 < this->text.size()) {
      const bool lineBreak = this->text.substr(g_serverLanguage->cursor - 2, 2) == "\r\n";
      if (lineBreak) {
        g_serverLanguage->KeyDown(key, lparam);
      }
    }
    g_serverLanguage->KeyDown(key, lparam);
    this->caretStart = std::chrono::system_clock::now();
    break;
  case VK_RIGHT:
    if (g_serverLanguage->cursor >= 0 && g_serverLanguage->cursor + 1 < this->text.size()) {
      const bool lineBreak = this->text.substr(g_serverLanguage->cursor, 2) == "\r\n";
      if (lineBreak) {
        g_serverLanguage->KeyDown(key, lparam);
      }
    }
    g_serverLanguage->KeyDown(key, lparam);
    this->caretStart = std::chrono::system_clock::now();
    break;
  case VK_BACK:
    if (std::abs(g_serverLanguage->cursor - g_serverLanguage->anchor) == 0 && g_serverLanguage->cursor >= 2 && g_serverLanguage->cursor - 1 < this->text.size()) {
      const bool lineBreak = this->text.substr(g_serverLanguage->cursor - 2, 2) == "\r\n";
      if (lineBreak) {
        g_serverLanguage->Character(VK_BACK, lparam);
      }
    }
    this->caretStart = std::chrono::system_clock::now();
    break;
  case VK_DELETE:
    if (std::abs(g_serverLanguage->cursor - g_serverLanguage->anchor) == 0 && g_serverLanguage->cursor >= 0 && g_serverLanguage->cursor + 1 < this->text.size()) {
      const bool lineBreak = this->text.substr(g_serverLanguage->cursor, 2) == "\r\n";
      if (lineBreak) {
        g_serverLanguage->KeyDown(key, lparam);
      }
    }
    g_serverLanguage->KeyDown(key, lparam);
    this->caretStart = std::chrono::system_clock::now();
    break;
  case VK_INSERT:
  case 'C':
  case 'V':
  case 'X':
    g_serverLanguage->KeyDown(key, lparam);
    break;
  case 'A':
    if ((static_cast<unsigned short>(GetKeyState(VK_CONTROL)) >> 8) != 0) {
      g_serverLanguage->Select(0, static_cast<int>(this->text.size()));
    }
    break;
  case VK_HOME:
    if ((static_cast<unsigned short>(GetKeyState(VK_CONTROL)) >> 8) != 0) {
      g_serverLanguage->RetreatCursor(g_serverLanguage->cursor);
    } else {
      g_serverLanguage->RetreatCursor(this->column);
    }
    break;
  case VK_UP:
  case VK_DOWN:
    return;
  case VK_END:
    if ((static_cast<unsigned short>(GetKeyState(VK_CONTROL)) >> 8) != 0) {
      g_serverLanguage->AdvanceCursor(static_cast<int>(this->text.size()) - g_serverLanguage->cursor);
    } else {
      g_serverLanguage->AdvanceCursor(static_cast<int>(this->lines[this->line].text.size()) - this->column);
      this->wrapPending = 1;
    }
    return;
  }

  this->ResetDragState();
}

// OFFSET: 0x009E5B20
// FUNCTION: fn_14450
int CUIEditCtrlEx::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  int position;
  switch (message) {
  case UM_SCROLLVERT:
    position = this->scrollPosition + a;
    break;
  case UM_SCROLLVERTPAGEUP:
    position = this->scrollStep + this->scrollPosition;
    break;
  case UM_SCROLLVERTPAGEDOWN:
    position = this->scrollPosition - this->scrollStep;
    break;
  case UM_DEFPUSH:
    if (reinterpret_cast<const unsigned char*>(&this->editFlags)[1] != 0 && HIBYTE(GetKeyState(VK_SHIFT))) {
      g_serverLanguage->Insert("\r\n");
      return 0;
    }
    UIWindow::SendMsg(sender, UM_DEFPUSH, a, b, c, d);
    return 0;
  case UM_UPPUSH:
    if (this->line > 0) {
      if (this->previousCursorLine == this->line - 1) {
        g_serverLanguage->RetreatCursor(this->cursorAdvance);
      } else {
        const EditLine& previous = this->lines[this->line - 1];
        int offset;
        if (this->alignment == 2) {
          const int width = this->TextWidth(this->lines[this->line].text.c_str(), 0, 0, 12, 0, 0);
          const int columnWidth =
            this->column > 0 ? this->TextWidth(this->lines[this->line].text.c_str(), this->column, 0, 12, 0, 0) : 0;
          offset = this->TrailingOffsetFitting(previous.text, width - columnWidth);
        } else {
          const int columnWidth =
            this->column > 0 ? this->TextWidth(this->lines[this->line].text.c_str(), this->column, 0, 12, 0, 0) : 0;
          offset = this->LeadingBytesFitting(previous.text, columnWidth);
        }

        const EditLine& line = this->lines[this->line - 1];
        const int delta = (line.hasCarriageReturn != 0 ? 2 : 0) + static_cast<int>(line.text.size()) - offset + this->column;
        g_serverLanguage->RetreatCursor(delta);
        this->cursorAdvance = delta;
      }
      this->previousCursorLine = this->line;
      this->wrapPending = 1;
    }
    this->selectedAll = 1;
    return 0;
  case WM_KEYDOWN:
    this->HandleKeyDown(a, b);
    this->selectedAll = 1;
    return 3;
  case WM_CHAR:
    if (this->textAlignment != 0) {
      if (this->textAlignment == 1) {
        if ((a < '0' || a > '9') && a != VK_BACK) {
          return 3;
        }
      } else if (this->textAlignment == 2) {
        if ((a < '0' || a > '9') && a != '.' && a != VK_BACK) {
          return 3;
        }
      } else {
        return 3;
      }
    }
    g_serverLanguage->Character(a, b);
    this->selectedAll = 1;
    return 3;
  case UM_DOWNPUSH:
    if (this->lines.size() - 1 > static_cast<unsigned>(this->line)) {
      if (this->previousCursorLine == this->line + 1) {
        g_serverLanguage->AdvanceCursor(this->cursorAdvance);
      } else {
        const EditLine& next = this->lines[this->line + 1];
        int offset;
        if (this->alignment == 2) {
          const int width = this->TextWidth(this->lines[this->line].text.c_str(), 0, 0, 12, 0, 0);
          const int columnWidth =
            this->column > 0 ? this->TextWidth(this->lines[this->line].text.c_str(), this->column, 0, 12, 0, 0) : 0;
          offset = this->TrailingOffsetFitting(next.text, width - columnWidth);
        } else {
          const int columnWidth =
            this->column > 0 ? this->TextWidth(this->lines[this->line].text.c_str(), this->column, 0, 12, 0, 0) : 0;
          offset = this->LeadingBytesFitting(next.text, columnWidth);
        }

        const EditLine& line = this->lines[this->line];
        const int delta = (line.hasCarriageReturn != 0 ? 2 : 0) + static_cast<int>(line.text.size()) - this->column + offset;
        g_serverLanguage->AdvanceCursor(delta);
        this->cursorAdvance = delta;
      }
      this->previousCursorLine = this->line;
      this->wrapPending = 1;
    }
    this->selectedAll = 1;
    return 0;
  case WM_IME_COMPOSITION:
    this->selectedAll = 1;
    return 0;
  default:
    UIEditCtrl::SendMsg(sender, message, a, b, c, d);
    return 0;
  }

  this->scrollPosition = std::clamp(position, 0, this->scrollMaximum);
  static_cast<UIScrollBar*>(this->scrollBar)->SetPosition(this->scrollPosition);
  this->scrollEnabled = this->scrollMaximum > 0;
  return 0;
}

// OFFSET: 0x009E6300
// FUNCTION: fn_14453
void CUIEditBoxEx::SetFocus() {
  g_windowMgr.SetFocus(this->edit);
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
  edit->outlineColor = value;
  edit->drawOutline = 1;
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

// STRING: st_16839
static const std::string s_editExItemLinkOpen = "<ITEML>";

// STRING: st_16840
static const std::string s_editExItemLinkClose = "</ITEML>";

// OFFSET: 0x009E6500
// FUNCTION: fn_14462
void CUIEditCtrlEx::SetText(const char* value) {
  this->text.assign(value);

  if (this == g_windowMgr.GetHoverWindow()) {
    g_serverLanguage->ResetInput();
    g_serverLanguage->Insert(this->text.c_str());
    g_serverLanguage->Select(0, static_cast<int>(g_serverLanguage->text.size()));
  }

  if (this->text.find(s_editExItemLinkOpen) != std::string::npos || this->text.find(s_editExItemLinkClose) != std::string::npos) {
    this->SetLinkedText(this->text.c_str());
  }

  this->selectedAll = 1;
}

#include <chrono>

// OFFSET: 0x009E4410
// FUNCTION: fn_14432
void CUIEditBoxEx::Create(int width, int height, bool withScrollBar) {
  this->RemoveChild(this->edit, 1);
  this->edit = new CUIEditCtrlEx();
  this->edit->SetSize(width, height);
  if (withScrollBar) {
    reinterpret_cast<unsigned char*>(&this->edit->editFlags)[1] = 1;
  }
  this->AddChild(this->edit);
  this->SetSize(width, height);
}

#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x009E4660
// FUNCTION: fn_14435
void CUIEditCtrlEx::PaintTextLines() {
  if (this->selectedAll) {
    this->Synchronize();
    this->selectedAll = 0;
  }

  int width = this->geometry.width;
  if (reinterpret_cast<const unsigned char*>(&this->editFlags)[1] != 0) {
    width -= this->scrollBar->geometry.width;
  }

  if (this->lines.empty()) {
    int x = this->horizontalPadding;
    if (this->alignment == 2) {
      const int caretWidth = this->TextWidth("|", 0, 0, 12, 0, 0);
      x = width - this->horizontalPadding - caretWidth;
    } else if (this->alignment == 6) {
      x = width / 2;
    }
    this->DrawCaret(x, this->verticalPadding);
    return;
  }

  const int lineCount = static_cast<int>(this->lines.size());
  const int visibleLines = std::max(1, (this->geometry.height - this->verticalPadding) / this->lineHeight);
  int firstLine = this->scrollPosition;
  int endLine = std::min(firstLine + visibleLines, lineCount);

  for (int lineIndex = firstLine; lineIndex < endLine; ++lineIndex) {
    EditLine& line = this->lines[lineIndex];
    const int textWidth = this->TextWidth(line.text.c_str(), 0, 0, 12, 0, 0);

    int x = this->horizontalPadding;
    if (this->alignment == 2) {
      x = width - x - textWidth;
    } else if (this->alignment == 6) {
      x = (width - textWidth) / 2;
    }
    int y = (lineIndex - firstLine) * this->lineHeight + this->verticalPadding;

    if (!line.text.empty()) {
      if (g_windowMgr.GetHoverWindow() == this && line.selectionEnd - line.selectionBegin > 0) {
        const int selectionBeginWidth =
          line.selectionBegin > 0 ? this->TextWidth(line.text.c_str(), line.selectionBegin, 0, 12, 0, 0) : 0;
        const int selectionEndWidth =
          line.selectionEnd > 0 ? this->TextWidth(line.text.c_str(), line.selectionEnd, 0, 12, 0, 0) : 0;
        this->DrawSelectionAt(x + selectionBeginWidth, y, selectionEndWidth - selectionBeginWidth);
      }

      this->PlaceLinks(line.text, y, lineIndex, firstLine, endLine);

      if (this->drawOutline) {
        this->DrawEmboldenedText(x, this->verticalPadding + y, line.text.c_str(), this->textColor, this->outlineColor, 0, 0, 12);
      } else {
        this->DrawTextAt(x, this->verticalPadding + y, line.text.c_str(), 0, 0, 12, this->textColor, 0, 0);
      }

      this->PlaceTags(x, y, firstLine, endLine, lineIndex, this->lineCount);
    }

    if (g_windowMgr.GetHoverWindow() == this && lineIndex == this->line) {
      const int caretOffset = this->column > 0 ? this->TextWidth(line.text.c_str(), this->column, 0, 12, 0, 0) : 0;
      this->DrawCaret(x - 2 + caretOffset, y);
    }
  }
}

CUIEditCtrlEx::CUIEditCtrlEx()
    : UIEditCtrl3(),
      normalColor(0),
      outlineColor(0),
      drawOutline(0),
      placeholderColor(0x808080),
      alignment(0),
      horizontalPadding(3),
      verticalPadding(1),
      selectedAll(0),
      column(0),
      line(0),
      previousCursorLine(-1),
      cursorAdvance(0),
      wrapPending(0),
      caretStart(std::chrono::system_clock::now()),
      scrollMaximum(0),
      visibleLines(0),
      scrollStep(1),
      scrollPosition(0),
      textAlignment(0) {
}

// OFFSET: 0x009E45F0
// FUNCTION: fn_14434
void CUIEditCtrlEx::DrawCaret(int x, int y) {
  const int elapsed = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->caretStart).count());
  if (((elapsed / 500) & 1) == 0 && IsInputSuggestionAllowed()) {
    this->DrawTextAt(x, y, "|", 0, 0, 12, this->textColor, 0, 0);
  }
}
