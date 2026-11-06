#include "Client/Input/InputCharset.h"
#include "Render/Text/TextContext.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Window/UIWindow.h"
#include "Network/TextEncoding.h"

#include <cstring>
#include <mbstring.h>

// OFFSET: 0x00A82310
// FUNCTION: fn_16993
void UIWindow::WrapPixelText(const char* text, std::vector<std::string>& lines, int width) {
  if (!text || *text == '\0') {
    return;
  }

  const int bytes = static_cast<int>(std::strlen(text));

  if (this->TextWidth(text, bytes, 0, 12, 0, 0) <= 0) {
    return;
  }

  const char* end = text + bytes;
  int offset = 0;

  for (int row = 0; offset < bytes; ++row) {
    if (row > 1000) {
      return;
    }

    const char* begin = text + offset;
    const char* current = begin;
    int count = 0;
    int extent = 0;
    bool newline = false;

    for (int step = 0; current < end; ++step) {
      if (step > 1000) {
        break;
      }

      const int previous = count;
      const char* next = g_serverLanguage->NextChar(current, static_cast<int>(end - current));
      const int length = static_cast<int>(next - current);

      extent = (extent + this->TextWidth(current, length, 0, 12, 0, 0));
      count = (count + length);

      if (length == 1 && (*current == '\r' || *current == '\n')) {
        this->TextWidth("\n", 1, 0, 12, 0, 0);
        newline = true;

        if (current + 1 < end && current[1] == '\n') {
          ++count;
        }

        break;
      }

      if (*current == '^') {
        const unsigned char red = static_cast<unsigned char>(g_hexDigits[current[1]] * 16 + g_hexDigits[current[2]]);
        const unsigned char green = static_cast<unsigned char>(g_hexDigits[current[3]] * 16 + g_hexDigits[current[4]]);
        const unsigned char blue = static_cast<unsigned char>(g_hexDigits[current[5]] * 16 + g_hexDigits[current[6]]);
        const std::uint32_t color = red | (green << 8) | (blue << 16);

        if (color != 0) {
          extent = (extent - this->TextWidth("^000000", 7, 0, 12, 0, 0));
        } else {
          char* code = new char[8];
          std::memcpy(code, current, 7);
          code[7] = '\0';

          if (std::strcmp(code, "^000000") == 0) {
            extent = (extent - this->TextWidth("^000000", 7, 0, 12, 0, 0));
          }

          delete[] code;
        }
      }

      if (*current == '[') {
        const auto* close = _mbsstr(reinterpret_cast<const unsigned char*>(current), reinterpret_cast<const unsigned char*>("]"));

        if (close) {
          const int bracketBytes = static_cast<int>(reinterpret_cast<const char*>(close) - current) + 1;
          const int bracketWidth = this->TextWidth(current, bracketBytes, 0, 12, 0, 0);
          const int characterWidth = this->TextWidth(current, length, 0, 12, 0, 0);

          if (((extent + bracketWidth) - characterWidth) > width && bracketWidth <= width) {
            count = previous;
            break;
          }
        }
      }

      if (extent > width) {
        count = previous;
        break;
      }

      current = next;
    }

    if (count <= 0) {
      continue;
    }

    char* buffer = new char[count + 1];
    std::memcpy(buffer, begin, count);

    if (newline) {
      buffer[count - 1] = '\0';

      if (std::strlen(buffer) == 0) {
        lines.push_back(" ");
      } else {
        lines.push_back(buffer);
      }
    } else {
      buffer[count] = '\0';
      lines.push_back(buffer);
    }

    delete[] buffer;
    offset = (offset + count);
  }
}

// OFFSET: 0x008932D0
// FUNCTION: fn_11504
void UIWindow::WrapLinkText(int* kind, std::vector<std::string>* lines, const std::string& text) {
  const char* characters = text.c_str();
  if (*kind != 3) {
    this->WrapPixelText(characters, *lines, this->geometry.width - 16);
    return;
  }

  std::vector<bool> wrapped;
  this->WrapTextWithBreaks(characters, lines, &wrapped, this->geometry.width - 24);
}
