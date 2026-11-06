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

// OFFSET: 0x009B4220
// FUNCTION: fn_13829
void WrapPlainText(const char* text, std::vector<std::string>& lines, int width) {
  const char* begin = text;
  const char* chunk = text;
  const char* current = text;
  const char* breakAt = text;
  const unsigned bytes = static_cast<unsigned>(std::strlen(text));
  const char* end = text + bytes;
  const char* limit = text + width;
  const char* marker = text + TextContext::Marker(text, bytes);

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
      lines.emplace_back(begin, chunk);
      begin = chunk;
      limit = chunk + width;
    }
  }

  if (chunk < current) {
    lines.emplace_back(chunk, current);
  }

  if (g_inlineCharset && *marker != '\0' && marker < end) {
    for (std::size_t index = 1; index < lines.size(); ++index) {
      lines[index].insert(0, marker, 3);
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
      lines.emplace_back(begin, current++);

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
      lines.emplace_back(begin, current);
      current += length;
      limit += length;
      begin = current;

      if (id != 0) {
        ITEM_INFO item;
        item.SetItemId(id);

        char name[260];
        int capacity = sizeof(name);
        item.GetIdDisplayName(name, capacity, 0);
        lines.push_back(name);
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

  if (g_inlineCharset && marker && *marker != '\0' && marker < end) {
    for (std::size_t index = 1; index < lines.size(); ++index) {
      const unsigned position = IsHexColorToken(lines[index].data(), lines[index].data() + lines[index].size()) ? 7 : 0;
      lines[index].insert(position, marker, 3);
    }
  }
}
