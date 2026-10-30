#include "UI/Text/UIStaticText.h"
#include "UI/Window/UINoticeWnd.h"
#include "UI/Lists/UIScrollBar.h"

#include <cstdio>
#include <cstring>

// OFFSET: 0x008BB0D0
// FUNCTION: fn_11795
void UINoticeWnd::LoadNotice() {
  UIScrollBar* scrollBar = static_cast<UIScrollBar*>(this->m_scrollBar);
  const int lines = this->m_bottomPositions[this->m_index] - this->m_scrollPositions[this->m_index];
  scrollBar->SetRange(lines - 19, 19, lines);
  if (this->m_bottomPositions[this->m_index] - this->m_scrollPositions[this->m_index] < 19) {
    static_cast<UIScrollBar*>(this->m_scrollBar)->SetRange(0, 19, 19);
  }
  static_cast<UIScrollBar*>(this->m_scrollBar)->SetPosition(this->m_scroll - this->m_scrollPositions[this->m_index]);
  this->m_scrollBar->Invalidate();

  FILE* file = std::fopen(this->m_patchFile, "r");
  std::fseek(file, 0, SEEK_END);
  int size = std::ftell(file);
  char* buffer = new char[size + 1];
  std::fseek(file, 0, SEEK_SET);
  std::fread(buffer, size, 1, file);
  buffer[size] = 0;
  std::fclose(file);

  int lineStarts[10000];
  std::memset(lineStarts, 0, 9999);

  int end = 0;
  int lineCount = 1;
  if (size > 0) {
    char* cursor = buffer;
    do {
      const char c = *cursor;
      if (c == '\n') {
        if (end == 0) {
          lineStarts[lineCount] = static_cast<int>(cursor + 1 - buffer);
          ++lineCount;
          *cursor = 0;
        }
      } else if (c == '\r') {
        *cursor = 0;
      } else if (c == '^' && end == 0 && lineCount > this->m_scrollPositions[this->m_index]) {
        end = lineCount - 1;
        if (lineCount < 0) {
          lineCount = 0;
        }
      }
      ++cursor;
    } while (--size != 0);

    if (end > 0) {
      lineCount = end - this->m_scrollPositions[this->m_index];
    }
  }

  int visible = lineCount - 1;
  if (visible > 19) {
    visible = 19;
  }

  for (int i = 0; i < 19; ++i) {
    if (i < visible) {
      this->m_texts[i]->SetText(buffer + lineStarts[this->m_scroll + i], 1);
    } else {
      this->m_texts[i]->SetText("", 0);
    }
  }

  this->Invalidate();
}
