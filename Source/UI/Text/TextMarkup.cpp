#include "UI/Text/TextMarkup.h"

#include <windows.h>
#include <utility>

namespace {
  // INLINED
  bool Hex(unsigned char value) {
    return (value >= '0' && value <= '9') || (value >= 'a' && value <= 'f') || (value >= 'A' && value <= 'F');
  }

  // INLINED
  bool Color(const char* text, const char* end) {
    if (*text != '^' || end - text < 6) {
      return false;
    }

    for (int index = 1; index <= 6; ++index) {
      if (!Hex(static_cast<unsigned char>(text[index]))) {
        return false;
      }
    }

    return true;
  }
}

// GLOBAL: gl_24593
static const std::string g_naviTagBegin = "<NAVI>";

// GLOBAL: gl_24594
static const std::string g_naviTagEnd = "</NAVI>";

// OFFSET: 0x00A7CA90
// FUNCTION: fn_16949
void ExtractNavigationTextLinks(std::string&& text, std::vector<TextLink>& links) {
  ExtractTextLinks(std::move(text), g_naviTagBegin, g_naviTagEnd, links);
}

// GLOBAL: gl_24597
static const std::string g_tipBoxTagBegin = "<TIPBOX>";

// GLOBAL: gl_24598
static const std::string g_tipBoxTagEnd = "</TIPBOX>";

// OFFSET: 0x00A7D690
// FUNCTION: fn_16951
void ExtractTipBoxTextLinks(std::string&& text, std::vector<TextLink>& links) {
  ExtractTextLinks(std::move(text), g_tipBoxTagBegin, g_tipBoxTagEnd, links);
}

// GLOBAL: gl_24591
static const std::string g_urlTagBegin = "<URL>";

// GLOBAL: gl_24592
static const std::string g_urlTagEnd = "</URL>";

// OFFSET: 0x00A7D6B0
// FUNCTION: fn_16952
void ExtractUrlTextLinks(std::string&& text, std::vector<TextLink>& links) {
  ExtractTextLinks(std::move(text), g_urlTagBegin, g_urlTagEnd, links);
}

// OFFSET: 0x00A81A70
// FUNCTION: fn_16980
bool HasTextLinks(const std::string& text) {
  return text.find("<URL>") != std::string::npos || text.find("<NAVI>") != std::string::npos || text.find("<INFO>") != std::string::npos ||
         text.find("<ITEML>") != std::string::npos || text.find("<TIPBOX>") != std::string::npos || text.find("<QUEST>") != std::string::npos;
}

// OFFSET: 0x008791E0
// FUNCTION: fn_11214
std::string ExpandTextEscapes(std::string text) {
  auto position = text.find("\\n");

  while (position != std::string::npos) {
    text.replace(position, 2, "\n");
    position = text.find("\\n");
  }

  position = text.find("\\r");

  while (position != std::string::npos) {
    text.replace(position, 2, "\r");
    position = text.find("\\r");
  }

  return text;
}

// OFFSET: 0x00A7EDC0
// FUNCTION: fn_16961
std::string RemoveTextColors(const char* text, unsigned short codePage) {
  const char* end = text + std::char_traits<char>::length(text);
  const char* current = text;
  std::string result;

  while (current < end) {
    const char* next = current;

    while (next < end && !Color(next, end)) {
      next = CharNextExA(codePage, next, 0);
    }

    result.append(current, next);

    if (next >= end) {
      break;
    }

    current = next + 7;
  }

  return result;
}

// OFFSET: 0x00A7F660
// FUNCTION: fn_16966
std::string DisplayTextTag(std::string text, const std::string& begin, const std::string& end) {
  auto position = text.find(begin);

  if (position == std::string::npos) {
    return text;
  }

  std::string result;

  do {
    if (position > 0) {
      result.append(text, 0, position);
    }

    const auto content = position + begin.size();
    std::string tail = text.substr(content);
    const auto close = tail.find(end);

    if (close == std::string::npos) {
      position = content;
    } else {
      result.append(tail, 0, close);
      text = tail.substr(close + end.size());
      position = text.find(begin);
    }
  } while (position != std::string::npos);

  result += text;
  return result;
}

// OFFSET: 0x00A7EF40
// FUNCTION: fn_16962
std::string RemoveTextLinkInfo(std::string text) {
  auto position = text.find("<INFO>");

  if (position == std::string::npos) {
    return text;
  }

  std::string result;

  do {
    if (position > 0) {
      result.append(text, 0, position);
    }

    const auto close = text.find("</INFO>");

    if (close != std::string::npos) {
      text = text.substr(close + 7);
      position = text.find("<INFO>");
    }
  } while (position != std::string::npos);

  result += text;
  return result;
}

// OFFSET: 0x00A7CAB0
// FUNCTION: fn_16950
void ExtractTextLinks(std::string text, const std::string& begin, const std::string& end, std::vector<TextLink>& links) {
  auto position = text.find(begin);

  if (position == std::string::npos) {
    return;
  }

  std::string prefix = RemoveTextLinkInfo(text.substr(0, position));
  std::uint32_t offset = static_cast<std::uint32_t>(prefix.size());

  do {
    const auto content = position + begin.size();
    std::string tail = text.substr(content);
    const auto close = tail.find(end);

    if (close == std::string::npos) {
      position = content;
      continue;
    }

    std::string display = tail.substr(0, close);
    std::string metadata;
    const auto info = display.find("<INFO>");

    if (info != std::string::npos) {
      display = tail.substr(0, info);
      metadata = tail.substr(info + 6);
      const auto infoEnd = metadata.find("</INFO>");

      if (infoEnd != std::string::npos) {
        metadata.resize(infoEnd);
      }
    }

    TextLink link;
    link.position = offset;
    link.display = display;
    link.metadata = metadata;
    links.push_back(std::move(link));
    offset += static_cast<std::uint32_t>(display.size());

    text = tail.substr(close + end.size());
    position = text.find(begin);

    if (position != std::string::npos) {
      prefix = RemoveTextLinkInfo(text.substr(0, position));
      offset += static_cast<std::uint32_t>(prefix.size());
    }
  } while (position != std::string::npos);
}

// OFFSET: 0x00877650
// FUNCTION: fn_11210
void SplitTextLinkArguments(std::string text, std::vector<std::string>& arguments) {
  if (text.empty()) {
    return;
  }

  auto comma = text.find(',');

  while (comma != std::string::npos) {
    arguments.push_back(text.substr(0, comma));
    text = text.substr(comma + 1);
    comma = text.find(',');
  }

  arguments.push_back(text);
}
