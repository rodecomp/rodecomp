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

  int lineStarts[9999];
  std::memset(&lineStarts[1], 0, 9995);

  int end = 0;
  lineStarts[0] = 0;
  int lineCount = 1;
  if (size > 0) {
    char* cursor = buffer;
    do {
      switch (*cursor) {
      case '\n':
        if (end != 0) {
          break;
        }
        lineStarts[lineCount] = static_cast<int>(cursor + 1 - buffer);
        ++lineCount;
        [[fallthrough]];
      case '\r':
        *cursor = 0;
        break;
      case '^':
        if (end == 0 && lineCount > this->m_scrollPositions[this->m_index]) {
          end = lineCount - 1;
          if (lineCount < 0) {
            lineCount = 0;
          }
        }
        break;
      }
      ++cursor;
    } while (--size != 0);

    if (end > 0) {
      lineCount = end - this->m_scrollPositions[this->m_index];
    }
  }

  --lineCount;
  if (lineCount > 19) {
    lineCount = 19;
  }

  for (int i = 0; i < 19; ++i) {
    if (i < lineCount) {
      this->m_texts[i]->SetText(buffer + lineStarts[this->m_scroll + i], 1);
    } else {
      this->m_texts[i]->SetText("", 0);
    }
  }

  this->Invalidate();
}
