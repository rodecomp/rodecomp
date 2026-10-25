#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Render/Text/TextContext.h"
#include "UI/Text/TextWrapping.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <intrin.h>
#include <mbstring.h>
#include <windows.h>

// OFFSET: 0x009B4220
// FUNCTION: fn_13829
void WrapPlainText(const char* text, std::vector<std::string>& lines, int width, unsigned short codePage, bool inlineCharset) {
  const char* begin = text;
  const char* chunk = text;
  const char* current = text;
  const char* breakAt = text;
  const unsigned bytes = static_cast<unsigned>(std::strlen(text));
  const char* end = text + bytes;
  const char* limit = text + width;
  const char* marker = text + TextContext::Marker(text, bytes, inlineCharset);

  while (current < end) {
    current = CharNextExA(codePage, current, 0);

    while (*current == ' ') {
      ++current;
      breakAt = current;
    }

    if (current >= limit || *current == '\n') {
      if (*current == '\n') {
        breakAt = current++;
      }

      chunk = breakAt <= begin ? current : breakAt;
      lines.emplace_back(begin, chunk);
      begin = chunk;
      limit = chunk + width;
    }
  }

  if (chunk < current) {
    lines.emplace_back(chunk, current);
  }

  if (inlineCharset && *marker != '\0' && marker < end) {
    for (std::size_t index = 1; index < lines.size(); ++index) {
      lines[index].insert(0, marker, 3);
    }
  }
}

namespace {
  // INLINED
  bool Hex(unsigned char value) {
    return (value >= '0' && value <= '9') || (value >= 'a' && value <= 'f') || (value >= 'A' && value <= 'F');
  }

  // INLINED
  bool Color(const char* text, std::size_t bytes) {
    if (bytes <= 6 || *text != '^') {
      return false;
    }

    for (unsigned index = 1; index <= 6; ++index) {
      if (!Hex(static_cast<unsigned char>(text[index]))) {
        return false;
      }
    }

    return true;
  }
}

// OFFSET: 0x009B4730
// FUNCTION: fn_13833
void WrapRichText(
  const char* text, std::vector<std::string>& lines, int width, bool carriageReturns, unsigned short codePage, bool inlineCharset, ItemDescriptions& items
) {
  const char* begin = text;
  const char* current = text;
  const char* breakAt = text;
  const unsigned bytes = static_cast<unsigned>(std::strlen(text));
  const char* end = text + bytes;
  const char* limit = text + width;
  const char* marker = text + TextContext::Marker(text, bytes, inlineCharset);

  while (current < end) {
    if (carriageReturns && *current == '\r') {
      lines.emplace_back(begin, current++);

      while (current < end && *current == '\n') {
        ++current;
      }

      begin = current;
      limit = current + width;
      continue;
    }

    if (Color(current, end - current)) {
      current += 7;
      limit += 7;
      breakAt = current;
    } else {
      unsigned digits = 0;

      if (std::strncmp(current, "^nItemID^", 9) == 0) {
        while (current + digits < end && Hex(static_cast<unsigned char>(current[9 + digits]))) {
          ++digits;
        }
      }

      if (digits != 0) {
        char number[12];

        if (digits >= 10) {
          __fastfail(FAST_FAIL_RANGE_CHECK_FAILURE);
        }

        std::memcpy(number, current + 9, digits);
        number[digits] = '\0';
        const int id = std::atoi(number);

        lines.emplace_back(begin, current);
        current += 9 + digits;
        limit += 9 + digits;
        begin = current;

        if (id != 0) {
          ItemInfo item;
          char itemId[16];
          std::sprintf(itemId, "%d", id);
          item.itemId = itemId;

          char name[260];
          int capacity = sizeof(name);
          items.Name(item, name, capacity, 0);
          lines.emplace_back(name);
        }
      } else {
        current = CharNextExA(codePage, current, 0);
      }
    }

    while (*current == ' ') {
      ++current;
      breakAt = current;
    }

    while (*current == ']') {
      ++current;
      breakAt = current;
    }

    while (*current == '[') {
      const unsigned char* bracket = _mbsstr(reinterpret_cast<const unsigned char*>(current), reinterpret_cast<const unsigned char*>("]"));
      const std::intptr_t extent = reinterpret_cast<std::intptr_t>(bracket) - reinterpret_cast<std::intptr_t>(current) + 1;

      if (reinterpret_cast<std::uintptr_t>(current) + extent < reinterpret_cast<std::uintptr_t>(limit)) {
        ++current;
      } else {
        if (extent >= width) {
          break;
        }

        limit -= extent;
        --current;
      }
    }

    if (current >= limit || *current == '\n') {
      const bool newline = *current == '\n';
      const char* split = newline ? ++current : breakAt;
      breakAt = split;

      if (split <= begin) {
        lines.emplace_back(begin, current);
        begin = current;
        limit = current + width;
      } else {
        lines.emplace_back(begin, split);
        begin = split;
        limit = split + width;
      }
    }
  }

  if (begin < current) {
    lines.emplace_back(begin, current);
  }

  if (inlineCharset && *marker != '\0' && marker < end) {
    for (std::size_t index = 1; index < lines.size(); ++index) {
      const unsigned position = Color(lines[index].data(), lines[index].size()) ? 7 : 0;
      lines[index].insert(position, marker, 3);
    }
  }
}
