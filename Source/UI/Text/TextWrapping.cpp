#include "Client/Input/InputCharset.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Render/Text/TextContext.h"
#include "UI/Text/TextWrapping.h"
#include "Utility/Text/TextTokens.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <intrin.h>
#include <mbstring.h>
#include <windows.h>

// INLINED
static void AppendLine(std::vector<std::string>& lines, const char* first, const char* last) {
  const std::string line(first, last);
  lines.push_back(line);
}

// OFFSET: 0x009B4220
// FUNCTION: fn_13829
void WrapPlainText(const char* text, std::vector<std::string>& lines, int width) {
  const char* begin = text;
  const char* chunk = text;
  const char* current = text;
  const char* breakAt = text;
  const char* end = text + std::strlen(text);
  const char* limit = text + width;
  const char* marker = text + TextContext::Marker(text, static_cast<unsigned>(end - text));

  while (current < end) {
    current = g_serverLanguage->NextChar(current, static_cast<int>(end - current));

    while (*current == ' ') {
      ++current;
      breakAt = current;
    }

    if (current >= limit || *current == '\n') {
      if (*current == '\n') {
        breakAt = current++;
      }

      chunk = breakAt <= begin ? current : breakAt;
      AppendLine(lines, begin, chunk);
      begin = chunk;
      limit = chunk + width;
    }
  }

  if (chunk < current) {
    AppendLine(lines, chunk, current);
  }

  if (g_inlineCharset && marker && *marker != '\0' && marker < end) {
    for (std::size_t index = 1; index < lines.size(); ++index) {
      lines[index].insert(lines[index].begin(), marker, marker + 3);
    }
  }
}

// OFFSET: 0x009B4730
// FUNCTION: fn_13833
void WrapRichText(
  const char* text, std::vector<std::string>& lines, int width, bool carriageReturns
) {
  const char* begin = text;
  const char* current = text;
  const char* breakAt = text;
  const unsigned bytes = static_cast<unsigned>(std::strlen(text));
  const char* end = text + bytes;
  const char* limit = text + width;
  const char* marker = text + TextContext::Marker(text, bytes);

  while (current < end) {
    if (carriageReturns && *current == '\r') {
      AppendLine(lines, begin, current++);

      while (current < end && *current == '\n') {
        ++current;
      }

      begin = current;
      limit = current + width;
      continue;
    }

    int length;
    int id;

    if (IsHexColorToken(current, end)) {
      current += 7;
      limit += 7;
      breakAt = current;
    } else if (ParseItemIdTag(current, end, length, id)) {
      AppendLine(lines, begin, current);
      current += length;
      limit += length;
      begin = current;

      if (id != 0) {
        ITEM_INFO item;
        item.SetItemId(id);

        char name[260];
        int capacity = sizeof(name);
        item.GetIdDisplayName(name, capacity, 0);
        const std::string line(name);
        lines.push_back(line);
      }
    } else {
      current = g_serverLanguage->NextChar(current, static_cast<int>(end - current));
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
        AppendLine(lines, begin, current);
        begin = current;
        limit = current + width;
      } else {
        AppendLine(lines, begin, split);
        begin = split;
        limit = split + width;
      }
    }
  }

  if (begin < current) {
    AppendLine(lines, begin, current);
  }

  if (g_inlineCharset && marker && *marker != '\0' && marker < end) {
    for (std::size_t index = 1; index < lines.size(); ++index) {
      const unsigned position = IsHexColorToken(lines[index].data(), lines[index].data() + lines[index].size()) ? 7 : 0;
      lines[index].insert(position, marker, 3);
    }
  }
}

// OFFSET: 0x009B5140
// FUNCTION: fn_13837
int ParseItemIdTag(const char* text, const char* end, int& length, int& itemId) {
  if (std::strncmp(text, "^nItemID^", 9) != 0) {
    return 0;
  }

  unsigned int digits = 0;
  while (text + digits < end && g_colorHexDigit[static_cast<signed char>(text[9 + digits])] != 0) {
    ++digits;
  }

  if (digits == 0) {
    return 0;
  }

  char number[10];
  std::strncpy(number, text + 9, digits);
  number[digits] = '\0';
  length = digits + 9;
  itemId = std::atoi(number);
  return 1;
}
