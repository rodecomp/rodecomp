#include "UI/Window/UIEnums.h"
#include "Client/Input/InputCharset.h"
#include "UI/Window/UIEnums.h"
#include "Client/Input/Language.h"
#include "Contents/Skin/SkinMgr.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIScrollBarVariants.h"
#include "Network/TextEncoding.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"
#include <cstring>
#include <windows.h>
#include <timeapi.h>

template __declspec(noinline) std::vector<bool>::iterator std::vector<bool>::erase(std::vector<bool>::const_iterator) noexcept;

// OFFSET: 0x0084F250
// FUNCTION: fn_10657
UIEditCtrl2::UIEditCtrl2() : UIEditCtrl() {
}

// OFFSET: 0x008515E0
// FUNCTION: fn_10692
UIEditCtrl2::~UIEditCtrl2() = default;

// OFFSET: 0x00863BF0
// FUNCTION: fn_10911
int UIEditCtrl2::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != UM_DEFPUSH) {
    this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  } else {
    if (g_windowMgr.IsLastFocus(g_windowMgr.GetHoverWindow())) {
      g_windowMgr.FocusFirst();
    } else {
      g_windowMgr.FocusNext();
    }

    this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x00863C70
// FUNCTION: fn_10912
int UIEditCtrl3::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  using NextCharHandler = const char*(__thiscall*)(InputCharset*, const char*, int);

  switch (message) {
  case UM_DEFPUSH:
    if (this->multiline == 0 || HIBYTE(GetKeyState(VK_SHIFT)) != 0 || this->multiline == 0) {
      char newline[3] = {'\r', '\n', '\0'};
      g_serverLanguage->Insert(newline);
    } else {
      this->UIWindow::SendMsg(sender, UM_DEFPUSH, a, b, c, d);
    }
    return 0;
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->firstVisibleLine;
    this->manualScroll = 1;

    if (message == UM_SCROLLVERTPAGEUP) {
      this->firstVisibleLine = previous - this->geometry.height / this->lineHeight + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      this->firstVisibleLine = previous - 1 + this->geometry.height / this->lineHeight;
    } else {
      this->firstVisibleLine = previous + a;
    }

    if (this->firstVisibleLine < 0) {
      this->firstVisibleLine = 0;
    }

    const int last = this->lineCount - this->geometry.height / this->lineHeight;
    if (this->firstVisibleLine > last) {
      this->firstVisibleLine = last;
    }

    this->scrollBar->SetPosition(this->firstVisibleLine);
    this->scrollBar->Invalidate();
    this->Invalidate();
    if (this->parent) {
      this->parent->Invalidate();
    }

    return this->firstVisibleLine - previous;
  }
  case UM_UPPUSH: {
    this->caretAtLineEnd = 0;

    const unsigned count = this->wrappedLines.size();
    if (count <= 1 && (count != 1 || !this->lineBreaks[0])) {
      return 0;
    }

    g_serverLanguage->Selection(this->anchor, this->cursor);
    if (this->verticalCursorStep <= 0 || this->previousCursor != this->cursor || this->movingUp != 0) {
      const char* current = this->previousLineText;
      this->movingDown = 0;
      int bytes = 0;
      int width = 0;
      const char* const end = current + std::strlen(current);
      int column = 0;

      if (this->previousLineLength != 0) {
        while (current < end) {
          const int previousBytes = bytes;
          const int previousWidth = width;
          const char* const start = current;
          current = reinterpret_cast<NextCharHandler>((*reinterpret_cast<void***>(g_serverLanguage))[5])(g_serverLanguage, start, static_cast<int>(end - start));
          const int length = static_cast<int>(current - start);
          width += this->TextWidth(start, length, 0, 12, 0, 0);
          bytes += length;

          if (length == 1 && *start == '\n') {
            column = bytes - 1;
            break;
          }

          column = bytes;
          if (width > this->caretPixelX) {
            if (width - this->caretPixelX > this->caretPixelX - previousWidth) {
              column = previousBytes;
            }
            break;
          }
        }
      }

      this->verticalCursorStep = this->previousLineLength - column + static_cast<int>(std::strlen(this->previousLineText));
    }

    g_serverLanguage->RetreatCursor(this->verticalCursorStep);
    this->movingUp = 1;
    this->caretAtLineEnd = 1;
    break;
  }
  case UM_DOWNPUSH: {
    this->caretAtLineEnd = 0;

    const unsigned count = this->wrappedLines.size();
    if ((count <= 1 || static_cast<unsigned>(this->textY) >= count - 1) && (count != 1 || !this->lineBreaks[0] || this->nextLineLength <= 0)) {
      return 0;
    }

    g_serverLanguage->Selection(this->anchor, this->cursor);
    if (this->verticalCursorStep <= 0 || this->previousCursor != this->cursor || this->movingDown != 0) {
      this->movingUp = 0;
      int bytes = 0;
      int width = 0;
      int column = 0;
      const unsigned lines = this->wrappedLines.size();

      if (lines > 1 && static_cast<unsigned>(this->textY) < lines - 1) {
        const char* current = this->nextLineText;
        const char* const end = current + std::strlen(current);

        while (current < end) {
          const int previousBytes = bytes;
          const int previousWidth = width;
          const char* const start = current;
          current = reinterpret_cast<NextCharHandler>((*reinterpret_cast<void***>(g_serverLanguage))[5])(g_serverLanguage, start, static_cast<int>(end - start));
          const int length = static_cast<int>(current - start);
          width += this->TextWidth(start, length, 0, 12, 0, 0);
          bytes += length;

          if (length == 1 && *start == '\n') {
            column = bytes - 1;
            break;
          }

          column = bytes;
          if (width > this->caretPixelX) {
            if (width - this->caretPixelX > this->caretPixelX - previousWidth) {
              column = previousBytes;
            }
            break;
          }
        }
      }

      this->verticalCursorStep = this->nextLineLength + column;
    }

    g_serverLanguage->AdvanceCursor(this->verticalCursorStep);
    this->movingDown = 1;
    break;
  }
  case WM_KEYDOWN:
    this->manualScroll = 0;
    this->HandleInputMessage(a, b);
    return 3;
  default:
    this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
    return 0;
  }

  g_serverLanguage->Selection(this->anchor, this->cursor);
  this->previousCursor = this->cursor;
  return 0;
}

// OFFSET: 0x0085E420
// FUNCTION: fn_10829
void UIEditCtrl3::HandleInputMessage(unsigned key, std::intptr_t data) {
  UIWindow* const chatWindow = ChatWindow() ? ChatWindow()->m_suggestions : nullptr;

  this->verticalCursorStep = 0;
  this->previousCursor = 0;

  switch (key) {
  case VK_LEFT:
    if (this->textY > 0) {
      g_serverLanguage->Selection(this->anchor, this->cursor);
      int length = 0;
      for (int line = 0; line < this->textY; ++line) {
        length += static_cast<int>(std::strlen(this->wrappedLines[line].c_str()));
      }
      if (length == this->cursor && this->lineBreaks[this->textY - 1]) {
        g_serverLanguage->KeyDown(key, data);
      }
    }
    g_serverLanguage->KeyDown(key, data);
    break;
  case VK_RIGHT:
    g_serverLanguage->Selection(this->anchor, this->cursor);
    if (static_cast<unsigned>(this->textY) < this->wrappedLines.size()) {
      int length = 0;
      for (int line = 0; line <= this->textY; ++line) {
        length += static_cast<int>(std::strlen(this->wrappedLines[line].c_str()));
      }
      if (length - 2 == this->cursor && this->lineBreaks[this->textY]) {
        g_serverLanguage->KeyDown(key, data);
        g_serverLanguage->KeyDown(key, data);
        return;
      }
    }
    g_serverLanguage->KeyDown(key, data);
    break;
  case VK_HOME:
    if (this->wrappedLines.size() != 0) {
      g_serverLanguage->Selection(this->anchor, this->cursor);
      int offset = this->cursor;
      for (int line = 0; line < this->textY; ++line) {
        offset -= static_cast<int>(std::strlen(this->wrappedLines[line].c_str()));
      }
      g_serverLanguage->RetreatCursor(offset);
      g_serverLanguage->Selection(this->anchor, this->cursor);
    }
    break;
  case VK_END:
    if (this->wrappedLines.size() != 0) {
      g_serverLanguage->Selection(this->anchor, this->cursor);
      int offset = this->cursor;
      for (int line = 0; line < this->textY; ++line) {
        offset -= static_cast<int>(std::strlen(this->wrappedLines[line].c_str()));
      }
      const int length = static_cast<int>(std::strlen(this->wrappedLines[this->textY].c_str()));
      g_serverLanguage->AdvanceCursor(this->lineBreaks[this->textY] ? length - offset - 2 : length - offset);
      g_serverLanguage->Selection(this->anchor, this->cursor);
    }
    this->caretAtLineEnd = 1;
    return;
  case VK_BACK:
    if (this->wrappedLines.size() != 0) {
      g_serverLanguage->Selection(this->anchor, this->cursor);
      int length = 0;
      for (int line = 0; line < this->textY; ++line) {
        length += static_cast<int>(std::strlen(this->wrappedLines[line].c_str()));
      }
      if (length == this->cursor && this->lineBreaks[this->textY - 1]) {
        g_serverLanguage->Character(VK_BACK, data);
      }
    }
    break;
  case VK_INSERT:
  case VK_DELETE:
  case 'A':
  case 'C':
  case 'V':
  case 'X':
    g_serverLanguage->KeyDown(key, data);
    break;
  }
}

// OFFSET: 0x0084F570
// FUNCTION: fn_10659
UIEditCtrl3::UIEditCtrl3() : UIEditCtrl2() {
  this->scrollBar = 0;
  this->scrollEnabled = 0;
  this->lineHeight = 14;
  this->lineCount = 0;
  this->caretLineLength = 0;
  this->previousLineLength = 0;
  this->nextLineLength = 0;
  this->caretPixelX = 0;
  this->verticalCursorStep = 0;
  this->previousCursor = 0;
  this->firstVisibleLine = 0;
  this->manualScroll = 0;
  this->movingUp = 0;
  this->movingDown = 0;
  this->caretAtLineEnd = 0;
  this->multiline = 0;
  this->previousLineText = 0;
  this->nextLineText = 0;
}

// OFFSET: 0x0084F2B0
// FUNCTION: fn_10658
UIEditCtrl3::UIEditCtrl3(const UIEditCtrl3& other) : UIEditCtrl2(other) {
  this->scrollBar = 0;
  this->UIEditCtrl3::OnCreate(other.geometry.width, other.geometry.height);

  this->scrollEnabled = other.scrollEnabled;
  this->lineHeight = other.lineHeight;
  this->lineCount = other.lineCount;
  this->caretLineLength = other.caretLineLength;
  this->previousLineLength = other.previousLineLength;
  this->nextLineLength = other.nextLineLength;
  this->caretPixelX = other.caretPixelX;
  this->verticalCursorStep = other.verticalCursorStep;
  this->previousCursor = other.previousCursor;
  this->firstVisibleLine = other.firstVisibleLine;
  this->manualScroll = other.manualScroll;
  this->movingUp = other.movingUp;
  this->movingDown = other.movingDown;
  this->caretAtLineEnd = other.caretAtLineEnd;
  this->multiline = other.multiline;

  char* first = other.previousLineText;
  if (first != this->previousLineText) {
    if (IsNullOrEmpty(first)) {
      this->previousLineText = 0;
    } else {
      const std::size_t length = std::strlen(first) + 1;
      this->previousLineText = new char[length];
      strcpy_s(this->previousLineText, length, first);
    }
  }

  char* second = other.nextLineText;
  if (second != this->nextLineText) {
    if (IsNullOrEmpty(second)) {
      this->nextLineText = 0;
    } else {
      const std::size_t length = std::strlen(second) + 1;
      this->nextLineText = new char[length];
      strcpy_s(this->nextLineText, length, second);
    }
  }
}

// OFFSET: 0x00851630
// FUNCTION: fn_10693
// OFFSET: 0x00852770
// FUNCTION: fn_10715
UIEditCtrl3::~UIEditCtrl3() {
  this->ClearLineState();
  this->pendingText.clear();
}

// OFFSET: 0x0085AC40
// FUNCTION: fn_10786
void UIEditCtrl3::OnCreate(int width, int height) {
  this->scrollBar = new UIScrollBar3();
  static_cast<UIScrollBar3*>(this->scrollBar)->Configure(width - 10, 0, 1, height);
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00863280
// FUNCTION: fn_10906
void UIEditCtrl3::Resize(int width, int height) {
  this->UIWindow::Resize(width, height);
  this->scrollBar->Move(this->geometry.width - 10, 0);
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->UpdateScrollBar();
}

// OFFSET: 0x00865220
// FUNCTION: fn_10922
int UIWhisperEditCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_UPPUSH:
    if (this->history.size() != 0) {
      if (this->historyIndex != 0) {
        --this->historyIndex;
      }
      this->SetText(this->history[this->historyIndex].c_str());
      this->OnBeginEdit();
    }
    break;
  case UM_DOWNPUSH:
    if (this->history.size() != 0 && static_cast<std::size_t>(this->historyIndex) < this->history.size()) {
      ++this->historyIndex;
      if (this->historyIndex == static_cast<int>(this->history.size())) {
        this->SetText("");
        this->OnBeginEdit();
      } else {
        this->SetText(this->history[this->historyIndex].c_str());
        this->OnBeginEdit();
      }
    }
    break;
  default:
    return this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

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

  if (this->history.size() <= static_cast<std::size_t>(this->historyIndex)) {
    this->historyIndex = 0;
  }

  this->SetText(this->history[this->historyIndex].c_str());
  this->OnBeginEdit();
  this->Invalidate();
}

// OFFSET: 0x008551B0
// FUNCTION: fn_10753
void UIEditCtrl3::PaintText() {
  FontCache& fonts = g_fontCache;

  int x = 3;
  if (this->invalidatesParent == 0 && this->firstVisible != 0) {
    x = 3 - this->TextWidth(this->text.c_str(), this->firstVisible, 0, 12, 0, 0);
  }

  std::string display;
  std::uint32_t color = this->textColor;
  if (g_windowMgr.GetHoverWindow() == this) {
    this->Synchronize();
  }

  const char* prefix = g_serverLanguage->Prefix(false);
  display = prefix + this->text;

  int width = this->geometry.width - 6;
  if (this->scrollEnabled != 0) {
    width += 3 - this->scrollBar->geometry.width;
  }

  this->wrappedLines.clear();
  this->lineBreaks.clear();
  this->WrapTextWithBreaks(display.c_str(), &this->wrappedLines, &this->lineBreaks, width);
  this->WrapLines(this->wrappedLines);

  for (std::size_t index = 0; index < this->wrappedLines.size() - this->lineBreaks.size(); ++index) {
    this->lineBreaks.push_back(false);
  }

  this->lineCount = static_cast<int>(this->wrappedLines.size());
  if (this->multiline != 0) {
    ++this->lineCount;
  }

  if (this->wrappedLines.size() == 0) {
    this->DrawBlinkingCaret(color);
    return;
  }

  const int rows = this->geometry.height / this->lineHeight;
  const int first = this->firstVisibleLine;
  const int last = std::min<std::size_t>(rows + first, this->wrappedLines.size());

  int offset = 0;
  for (int line = 0; line < first; ++line) {
    offset += static_cast<int>(std::strlen(this->wrappedLines[line].c_str()));
  }

  for (int line = first; line < last; ++line) {
    const int y = (line - first) * this->lineHeight;
    offset += static_cast<int>(std::strlen(this->wrappedLines[line].c_str()));

    if (g_windowMgr.GetHoverWindow() == this && std::abs(g_serverLanguage->cursor - g_serverLanguage->anchor) != 0) {
      const int selectionBegin = std::min(this->anchor, this->cursor);
      const int selectionEnd = std::max(this->anchor, this->cursor);
      int startX = 0;
      int endX = 0;

      if (selectionBegin < offset) {
        int begin = selectionBegin + static_cast<int>(std::strlen(this->wrappedLines[line].c_str())) - offset;
        if (begin < 0) {
          begin = 0;
        }

        startX = begin != 0 ? this->TextWidth(this->wrappedLines[line].c_str(), begin, 0, 12, 0, 0) : 0;

        int length = static_cast<int>(std::strlen(this->wrappedLines[line].c_str()));
        if (this->lineBreaks[line]) {
          length -= 2;
        }
        endX = this->TextWidth(this->wrappedLines[line].c_str(), length, 0, 12, 0, 0);
      }

      if (selectionEnd <= offset) {
        int end = selectionEnd + static_cast<int>(std::strlen(this->wrappedLines[line].c_str())) - offset;
        if (end < 0) {
          end = 0;
        }

        int length = end;
        if (static_cast<int>(std::strlen(this->wrappedLines[line].c_str())) == end && this->lineBreaks[line]) {
          length = end - 2;
        }
        endX = length != 0 ? this->TextWidth(this->wrappedLines[line].c_str(), length, 0, 12, 0, 0) : 0;
      }

      int left = x + startX;
      int span = endX - startX;
      if (left < 3) {
        span += left - 3;
        left = 3;
      }

      const int windowWidth = this->geometry.width;
      if (left < windowWidth && span > 0) {
        unsigned red, green, blue;
        g_skinInfo.PaletteChannels(6, 2, red, green, blue);
        this->FillRegion(left, y, left + span <= windowWidth ? span : windowWidth - left, 14, RGB(red, green, blue));
      }
    }

    int tagOffset = 0;
    if (this->geometry.id == 0x1F8 || this->geometry.id == 0x1EB) {
      tagOffset = 1;
    }

    if (this->pendingText.size() < static_cast<unsigned>(line + 1)) {
      this->pendingText.push_back(this->wrappedLines[line]);
    } else if (&this->pendingText[line] != &this->wrappedLines[line]) {
      this->pendingText[line] = this->wrappedLines[line];
    }

    this->PlaceLinks(this->wrappedLines[line], y, line, first, last);

    const std::size_t length = std::strlen(this->wrappedLines[line].c_str());
    char* buffer = new char[length + 1];
    std::strcpy(buffer, this->wrappedLines[line].c_str());
    buffer[length] = '\0';
    if (buffer[length - 2] == '\r') {
      buffer[length - 2] = '\0';
    }

    this->DrawTextAt(x, tagOffset + y, buffer, 0, 0, 12, color, 0, 0);

    for (auto entry = this->entries.begin(); entry != this->entries.end(); ++entry) {
      for (std::size_t index = 0; index < entry->size(); ++index) {
        UIItemTagButton* button = (*entry)[index];
        const int buttonLine = button->line;
        if (buttonLine < first || last - 1 < buttonLine) {
          button->Move(-100, -100);
        } else if (line == buttonLine) {
          button->Move(button->column + x - 1, y);
        }
      }
    }

    if (buffer) {
      delete[] buffer;
    }
  }

  int column = static_cast<int>(std::strlen(prefix)) + this->cursor;
  int reading = std::max(this->anchor, this->cursor) + static_cast<int>(g_serverLanguage->candidate.size() + std::strlen(prefix));

  int remaining = column;
  for (std::size_t index = 0; index < this->wrappedLines.size(); ++index) {
    remaining -= static_cast<int>(std::strlen(this->wrappedLines[index].c_str()));
    this->textY = static_cast<int>(index);
    if (remaining < 0 || (this->caretAtLineEnd != 0 && remaining == 0)) {
      break;
    }
  }

  if (g_windowMgr.GetHoverWindow() == this) {
    int caretLine = this->textY;
    if (this->caretLineLength != static_cast<int>(std::strlen(this->wrappedLines[caretLine].c_str()))) {
      this->manualScroll = 0;
      this->caretLineLength = static_cast<int>(std::strlen(this->wrappedLines[caretLine].c_str()));
    }

    int before = 0;
    for (int index = 0; index < caretLine; ++index) {
      before += static_cast<int>(std::strlen(this->wrappedLines[index].c_str()));
    }

    int wrapped = 0;
    if (column - before >= 0) {
      column -= before;
    }

    if (static_cast<int>(std::strlen(this->wrappedLines[caretLine].c_str())) == column && this->lineBreaks[caretLine]) {
      wrapped = 1;
      column = 0;
    }

    if (this->nextLineLength != static_cast<int>(std::strlen(this->wrappedLines[caretLine].c_str())) - column) {
      this->nextLineLength = static_cast<int>(std::strlen(this->wrappedLines[caretLine].c_str())) - column;
    }

    if (this->previousLineLength != column) {
      this->previousLineLength = column;
    }

    int caretX = 0;
    if (column != 0) {
      caretX = this->TextWidth(this->wrappedLines[caretLine].c_str(), column, 0, 12, 0, 0);
      caretLine = this->textY;
    }

    if (reading - before >= 0) {
      reading -= before;
    }

    int readingLength = reading;
    if (static_cast<int>(std::strlen(this->wrappedLines[caretLine].c_str())) == reading && this->lineBreaks[caretLine]) {
      readingLength = reading - 2;
    }

    const int readingX = readingLength != 0 ? this->TextWidth(this->wrappedLines[caretLine].c_str(), readingLength, 0, 12, 0, 0) : 0;

    if (g_serverLanguage->candidate.size() != 0) {
      int gap = readingX - caretX;
      if (gap < 2) {
        gap = 2;
      }
      caretX += gap + 2;
      this->previousLineLength += 2;
      this->MoveCandidateWindow(caretX);
      this->MoveReadingWindow(readingX);
    }

    if (this->caretPixelX != caretX) {
      this->caretPixelX = caretX;
    }

    if ((timeGetTime() / 500 & 1) == 0 && (!g_windowMgr.IsHoverOnWindow01C8Button() || g_windowMgr.inputSuggestionsEnabled)) {
      this->DrawTextAt(x - 2 + caretX, (wrapped - first + this->textY) * this->lineHeight, "|", 0, 0, 12, color, 0, 0);

      if (this->manualScroll == 0) {
        if (this->movingUp != 0) {
          const int current = this->textY;
          if (current - wrapped < this->firstVisibleLine) {
            if (current >= this->lineCount - rows) {
              this->firstVisibleLine = this->lineCount - rows;
            } else {
              this->firstVisibleLine = current + wrapped;
            }
          }
        } else if (this->movingDown != 0) {
          const int current = this->textY;
          if (this->firstVisibleLine - 1 + rows < current) {
            if (current < rows) {
              this->firstVisibleLine = 0;
            } else {
              this->firstVisibleLine = current - rows + 1;
            }
          }
        } else {
          const int top = this->lineCount <= rows ? this->textY : this->lineCount - rows + wrapped;
          this->firstVisibleLine = top;
          if (this->multiline != 0 && top > 0) {
            this->firstVisibleLine = top - 1;
          }
        }
      }
    }

    if (wrapped != 0) {
      ++this->textY;
    }
  }

  std::size_t count = this->wrappedLines.size();
  if (count > 1 && this->textY > 0) {
    const std::size_t length = std::strlen(this->wrappedLines[this->textY - 1].c_str());
    if (this->previousLineText) {
      delete[] this->previousLineText;
      this->previousLineText = nullptr;
    }
    this->previousLineText = new char[length + 1];
    std::strcpy(this->previousLineText, this->wrappedLines[this->textY - 1].c_str());
  } else if ((count == 1 || (count > 1 && this->textY == 0)) && this->lineBreaks[0]) {
    const std::size_t length = std::strlen(this->wrappedLines[0].c_str());
    if (this->previousLineText) {
      delete[] this->previousLineText;
      this->previousLineText = nullptr;
    }
    this->previousLineText = new char[length + 1];
    std::strcpy(this->previousLineText, this->wrappedLines[0].c_str());
  }

  count = this->wrappedLines.size();
  if (count > 1 && static_cast<unsigned>(this->textY) < count - 1) {
    const std::size_t length = std::strlen(this->wrappedLines[this->textY + 1].c_str());
    if (this->nextLineText) {
      delete[] this->nextLineText;
      this->nextLineText = nullptr;
    }
    this->nextLineText = new char[length + 1];
    std::strcpy(this->nextLineText, this->wrappedLines[this->textY + 1].c_str());
  } else if (count == 1 && this->lineBreaks[0]) {
    const std::size_t length = std::strlen(this->wrappedLines[0].c_str());
    if (this->nextLineText) {
      delete[] this->nextLineText;
      this->nextLineText = nullptr;
    }
    this->nextLineText = new char[length + 1];
    std::strcpy(this->nextLineText, this->wrappedLines[0].c_str());
  }

  this->UpdateScrollBar();
}

// OFFSET: 0x00853810
// FUNCTION: fn_10742
int UIEditCtrl::MeasureText(const std::string& text, const int& bytes) {
  return this->TextWidth(text.c_str(), bytes, 0, 12, 0, 0);
}

// OFFSET: 0x0084F710
// FUNCTION: fn_10660
UIEditCtrl4::UIEditCtrl4() : UIEditCtrl(), placeholderColor(0xFFFFFFFFu), placeholder(""), placeholderEnabled(0) {
}

UIEditCtrl4::~UIEditCtrl4() = default;

// OFFSET: 0x0085AA40
// FUNCTION: fn_10782
void UIEditCtrl4::OnBeginEdit() {
  this->text = "";
  g_serverLanguage->ResetInput();
  g_serverLanguage->ReplaceInput(this->text.c_str());
  g_serverLanguage->SetPassword(this->mask != 0);
  this->SelectAll();
}

// OFFSET: 0x008563D0
// FUNCTION: fn_10754
void UIEditCtrl4::PaintText() {
  int x = 3;
  if (this->invalidatesParent == 0 && this->firstVisible != 0) {
    x -= this->TextWidth(this->text.c_str(), this->firstVisible, 0, 12, 0, 0);
  }

  std::string display = "";
  if (g_windowMgr.GetHoverWindow() == this) {
    this->Synchronize();
    unsigned begin;
    unsigned end;
    if (this->mask != 0) {
      display.assign(this->text.size(), static_cast<char>(this->mask));
      begin = std::min(this->anchor, this->cursor);
      end = std::max(this->anchor, this->cursor);
    } else {
      const char* prefix = g_serverLanguage->Prefix(false);
      display = prefix + this->text;
      begin = std::min(this->anchor, this->cursor) + std::strlen(prefix);
      end = std::max(this->anchor, this->cursor) + g_serverLanguage->candidate.size() + std::strlen(prefix);
    }

    const int left = begin ? this->TextWidth(display.c_str(), begin, 0, 12, 0, 0) : 0;
    const int right = end ? this->TextWidth(display.c_str(), end, 0, 12, 0, 0) : 0;

    if (std::abs(g_serverLanguage->cursor - g_serverLanguage->anchor) != 0) {
      this->DrawSelection(left + x, right - left);
      if (this->secondColor == 0xFFFFFFFF) {
        this->DrawTextAt(x, this->textY, display.c_str(), 0, 0, 12, this->textColor, 0, 0);
      } else {
        this->DrawEmboldenedText(x, this->textY, display.c_str(), this->textColor, this->secondColor, 0, 0, 12);
      }
    } else if (!g_serverLanguage->candidate.empty()) {
      this->MoveCandidateWindow(left);
      this->MoveReadingWindow(right);
      if (0 == timeGetTime() / 500 % 2) {
        this->DrawSelection(left + x, std::max(right - left, 2));
      }
      if (this->secondColor == 0xFFFFFFFF) {
        this->DrawTextAt(x, this->textY, display.c_str(), 0, 0, 12, this->textColor, 0, 0);
      } else {
        this->DrawEmboldenedText(x, this->textY, display.c_str(), this->textColor, this->secondColor, 0, 0, 12);
      }
    } else {
      if (this->secondColor == 0xFFFFFFFF) {
        this->DrawTextAt(x, this->textY, display.c_str(), 0, 0, 12, this->textColor, 0, 0);
      } else {
        this->DrawEmboldenedText(x, this->textY, display.c_str(), this->textColor, this->secondColor, 0, 0, 12);
      }
      if (0 == timeGetTime() / 500 % 2 && IsInputSuggestionAllowed()) {
        this->DrawTextAt(left + x - 2, this->textY, "|", 0, 0, 12, this->textColor, 0, 0);
      }
      if (this->invalidatesParent != 0 && this->parent) {
        this->parent->Invalidate();
      }
    }
  } else {
    if (this->mask != 0) {
      display.assign(this->text.size(), static_cast<char>(this->mask));
    } else {
      display = this->text;
    }
    if (this->secondColor == 0xFFFFFFFF) {
      if (this->placeholderEnabled == 1) {
        this->DrawTextAt(x, this->textY, std::strcmp(display.c_str(), "") == 0 ? this->placeholder.c_str() : display.c_str(), 0, 0, 12, this->placeholderColor, 0, 0);
      } else {
        this->DrawTextAt(x, this->textY, display.c_str(), 0, 0, 12, this->textColor, 0, 0);
      }
    } else {
      this->DrawEmboldenedText(x, this->textY, display.c_str(), this->textColor, this->secondColor, 0, 0, 12);
    }
    if (this->invalidatesParent != 0 && this->parent) {
      this->parent->Invalidate();
    }
  }

  this->UpdateLinks();
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

  if (g_windowMgr.GetHoverWindow() == this) {
    g_serverLanguage->ResetInput();
    g_serverLanguage->ReplaceInput(this->placeholder.c_str());
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
UIEditCtrl5::UIEditCtrl5() : UIEditCtrl4() {
}

UIEditCtrl5::~UIEditCtrl5() = default;

// OFFSET: 0x00864140
// FUNCTION: fn_10913
int UIEditCtrl5::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != UM_DEFPUSH) {
    this->UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  } else {
    if (g_windowMgr.IsLastFocus(g_windowMgr.GetHoverWindow())) {
      g_windowMgr.FocusFirst();
    } else {
      g_windowMgr.FocusNext();
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
  const std::uint32_t color = SkinPaletteColor(cell, selected);
  this->red = GetRValue(color);
  this->green = GetGValue(color);
  this->blue = GetBValue(color);

  this->Invalidate();
  this->ClearSurface(RGB(this->red, this->green, this->blue));
  this->PaintText();
  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x0085FF60
// FUNCTION: fn_10892
void UIEditCtrl3::OnWheel(int value) {
  if (this->scrollEnabled == 0) {
    return;
  }

  if (value < 0) {
    this->SendMsg(this->scrollBar, UM_SCROLLVERT, 3, 0, 0, 0);
  } else {
    this->SendMsg(this->scrollBar, UM_SCROLLVERT, -3, 0, 0, 0);
  }
}

// OFFSET: 0x00868300
// FUNCTION: fn_10958
void UIEditCtrl3::Trim() {
  const int limit = this->maximum - this->tagTextLength;
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

  g_serverLanguage->ResetInput();
  g_serverLanguage->ReplaceInput(this->text.c_str());

  while (this->text.size() < static_cast<unsigned>(limit)) {
    this->text += " ";
  }
}

// OFFSET: 0x0085BB60
// FUNCTION: fn_10796
void UIBitmapEditCtrl::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str())));
  this->DrawBitmap(0, 0, bitmap, false);
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
UIChatEditCtrl::UIChatEditCtrl() : UIEditCtrl(), historyLimit(30) {
  this->historyIndex = 0;
  this->history.reserve(this->historyLimit);
  this->itemTags = 0;
}

// OFFSET: 0x0085EB50
// FUNCTION: fn_10840
void UIChatEditCtrl::OnLBtnDown(int x, int y) {
  if (this->disabled == 0) {
    g_windowMgr.SetFocus(this);
  }

  this->parent->OnLBtnDown(x + this->geometry.x, y + this->geometry.y);
}

// OFFSET: 0x00863640
// FUNCTION: fn_10909
int UIChatEditCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_UPPUSH:
    if (this->history.size() != 0) {
      if (this->historyIndex == this->history.size()) {
        this->pending = this->Text();
      }
      if (this->historyIndex != 0) {
        --this->historyIndex;
      }

      this->SetText(this->history[this->historyIndex].c_str());
      this->OnBeginEdit();
    }
    break;
  case UM_DOWNPUSH:
    if (this->history.size() != 0 && this->historyIndex < this->history.size()) {
      ++this->historyIndex;
      if (this->historyIndex == this->history.size()) {
    this->SetText(this->pending.c_str());
  } else {
    this->SetText(this->history[this->historyIndex].c_str());
  }
      this->OnBeginEdit();
    }
    break;
  default:
    return UIEditCtrl::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0085CDE0
// FUNCTION: fn_10808
void UIEditCtrl_OldList::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (!this->bitmapName.empty()) {
    this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str()))), false);
  }

  this->PaintText();
}

// OFFSET: 0x0085FFC0
// FUNCTION: fn_10894
void UIEditCtrl_OldList::OnWheel(int value) {
  if (value < 0) {
    this->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
  } else {
    this->SendMsg(nullptr, UM_UPPUSH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0085E3D0
// FUNCTION: fn_10827
void UIEditCtrlVarId::OnFinishEdit(int) {
  this->Synchronize();
  this->Invalidate();
  this->SendMsg(this, UM_DEFPUSH, 0, 0, 0, 0);
}

// OFFSET: 0x0085FFA0
// FUNCTION: fn_10893
void UIEditCtrlVarId::OnWheel(int value) {
  if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x00864CE0
// FUNCTION: fn_10915
int UIEditCtrlVarId::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_DEFPUSH) {
    return UIEditCtrl::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (this->parent) {
    this->parent->SendMsg(this, UM_COMMAND, this->geometry.id, 0, 0, 0);
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
void UIWhisperEditCtrl::OnBeginEdit() {
  g_serverLanguage->ResetInput();
  g_serverLanguage->ReplaceInput(this->text.c_str());
  g_serverLanguage->SetPassword(this->mask != 0);
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
  int step;
  switch (message) {
  case UM_UPPUSH:
    step = -1;
    break;
  case UM_DOWNPUSH:
    step = 1;
    break;
  default:
    return UIEditCtrl::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->RecallHistory(step);
  return 0;
}

// OFFSET: 0x00886F90
// FUNCTION: fn_11330
void UIShowAndEditTimeCtrl::OnCreate(int, int) {
  for (int i = 0; i < 2; ++i) {
    UIEditCtrlVarId*& field = this->fields[i];
    field = new UIEditCtrlVarId();
    field->SetSize(20, 18);
    field->Background(232, 232, 232);
    field->Border(192, 192, 192);
    field->flat = 0;
    field->maximum = 2;
    field->Move(i * 38, 0);
    field->SetText("0");
    field->SetId(ID_OK);
    this->AddChild(field);
  }
}

// OFFSET: 0x008996B0
// FUNCTION: fn_11556
int UIShowAndEditTimeCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == UM_COMMAND && argument1 == ID_OK) {
    for (int index = 0; index < 2; ++index) {
      if (sender == this->fields[index]) {
        int value = std::atoi(this->fields[index]->Text());
        if (value > 60) {
          value = 60;
        }
        const std::string text = FormatOwnedString("%d", value);
        this->fields[index]->SetText(text.c_str());
      }
    }
  }
  return 0;
}

// OFFSET: 0x00853F20
// FUNCTION: fn_10745
void UIEditCtrl3::ClearLineState() {
  auto line = this->wrappedLines.begin();
  while (line != this->wrappedLines.end()) {
    line = this->wrappedLines.erase(line);
  }
  this->wrappedLines.clear();

  auto lineBreak = this->lineBreaks.begin();
  while (lineBreak != this->lineBreaks.end()) {
    lineBreak = this->lineBreaks.erase(lineBreak);
  }
  this->lineBreaks.clear();

  if (this->previousLineText) {
    delete[] this->previousLineText;
    this->previousLineText = nullptr;
  }

  if (this->nextLineText) {
    delete[] this->nextLineText;
    this->nextLineText = nullptr;
  }
}

// OFFSET: 0x0086E7D0
// FUNCTION: fn_11077
UIShowAndEditTimeCtrl::UIShowAndEditTimeCtrl() : UIWindow(0) {
  this->padding_0084 = 0;
  this->padding_0088 = 0;
}

#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Edit/UIEditPaint.inl"

// OFFSET: 0x0085C4F0
// FUNCTION: fn_10801
void UIChatEditCtrl::Paint() {
  int backRed = this->red;
  int backGreen = this->green;
  int backBlue = this->blue;
  if (this->disabled) {
    backRed = backRed > 30 ? backRed - 30 : backRed;
    backGreen = backGreen > 30 ? backGreen - 30 : backGreen;
    if (backBlue > 30) {
      backBlue -= 30;
    }
  }

  this->textY = 0;

  if (this->flat == 0) {
    RECT rectangle{0, 0, this->geometry.width, this->geometry.height};
    this->FillSurface(RGB(this->borderRed, this->borderGreen, this->borderBlue), &rectangle);
    SetRect(&rectangle, 1, 1, this->geometry.width - 1, this->geometry.height - 1);
    this->FillSurface(RGB(backRed, backGreen, backBlue), &rectangle);
  } else {
    this->ClearSurface(RGB(backRed, backGreen, backBlue));
  }

  this->textY = this->topPadding + 2;
  this->PaintText();

  UIItemTagOnChat* tag = this->itemTags;
  if (tag != nullptr) {
    tag->SetTaggedText(this->text, this->geometry.width);
    this->itemTags->UpdateItemTags();
    UIItemTagOnChat* current = this->itemTags;
    const int span = static_cast<int>(current->linkedText.size() - current->taggedText.size());
    this->tagTextLength = span > 0 ? span : 0;
    if (current->previousTagCount == 0) {
      delete this->itemTags;
      this->itemTags = nullptr;
    }
  }
}

// OFFSET: 0x00867FC0
// FUNCTION: fn_10951
void UIEditCtrl3::SetText(const char* value) {
  this->text.assign(value, std::strlen(value));
  this->Trim();

  if (g_windowMgr.GetHoverWindow() == this) {
    g_serverLanguage->ResetInput();
    g_serverLanguage->ReplaceInput(this->text.c_str());
  }

  this->SelectAll();

  if (this->text.find("<ITEML>") != std::string::npos || this->text.find("</ITEML>") != std::string::npos) {
    this->SetLinkedText(this->text.c_str());
  }

  if (this->parent != nullptr) {
    this->parent->Invalidate();
  }
}
