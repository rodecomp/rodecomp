#include "Render/Text/TextContext.h"
#include "UI/Text/PixelTextWrapping.h"

#include <cstring>
#include <mbstring.h>

namespace {
  // INLINED
  unsigned Digit(unsigned char value) {
    if (value >= '0' && value <= '9') {
      return value - '0';
    }

    if (value >= 'a' && value <= 'f') {
      return value - 'a' + 10;
    }

    if (value >= 'A' && value <= 'F') {
      return value - 'A' + 10;
    }

    return 0;
  }
}

// OFFSET: 0x00A82310
// FUNCTION: fn_16993
void WrapPixelText(
  CDC& surface, FontCache& fonts, const TextLocale& locale, unsigned short codePage, const char* text, std::vector<std::string>& lines, int width
) {
  if (!text || *text == '\0') {
    return;
  }

  const auto measure = [&](const char* part, int bytes) {
    if (bytes == 0) {
      bytes = static_cast<int>(std::strlen(part));
    }

    if (bytes == 0) {
      return 0;
    }

    TextContext context(surface, fonts, locale);
    context.SetFont(0, 12, 0, 0);
    return static_cast<int>(context.Measure(part, bytes).cx);
  };

  const int bytes = static_cast<int>(std::strlen(text));

  if (measure(text, bytes) <= 0) {
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
      const char* next = CharNextExA(codePage, current, 0);
      const int length = static_cast<int>(next - current);

      extent = (extent + measure(current, length));
      count = (count + length);

      if (length == 1 && (*current == '\r' || *current == '\n')) {
        measure("\n", 1);
        newline = true;

        if (current + 1 < end && current[1] == '\n') {
          ++count;
        }

        break;
      }

      if (*current == '^') {
        const unsigned color = (Digit(current[2]) + 16 * Digit(current[1])) | ((Digit(current[4]) + 16 * Digit(current[3])) << 8) |
                               ((Digit(current[6]) + 16 * Digit(current[5])) << 16);

        if (color != 0 || std::strncmp(current, "^000000", 7) == 0) {
          extent = (extent - measure("^000000", 7));
        }
      }

      if (*current == '[') {
        const auto* close = _mbsstr(reinterpret_cast<const unsigned char*>(current), reinterpret_cast<const unsigned char*>("]"));

        if (close) {
          const int bracketBytes = static_cast<int>(reinterpret_cast<const char*>(close) - current) + 1;
          const int bracketWidth = measure(current, bracketBytes);
          const int characterWidth = measure(current, length);

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

    std::string line(begin, static_cast<std::size_t>(count));

    if (newline) {
      line[static_cast<std::size_t>(count - 1)] = '\0';
      line.resize(std::strlen(line.c_str()));
      lines.push_back(line.empty() ? " " : line);
    } else {
      lines.push_back(std::move(line));
    }

    offset = (offset + count);
  }
}
