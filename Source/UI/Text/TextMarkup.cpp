#include "UI/Text/ColoredText.h"
#include "UI/Text/TextMarkup.h"

#include <windows.h>
#include <utility>

namespace {
  // INLINED
  bool Hex(unsigned char value) {
    return (value >= '0' && value <= '9') || (value >= 'a' && value <= 'f') || (value >= 'A' && value <= 'F');
  }

  // INLINED
}

// GLOBAL: gl_24593
static const std::string g_naviTagBegin = "<NAVI>";

// GLOBAL: gl_24594
static const std::string g_naviTagEnd = "</NAVI>";

// GLOBAL: gl_25478
static const std::string g_infoTagBegin = "<INFO>";

// GLOBAL: gl_25479
static const std::string g_infoTagEnd = "</INFO>";

// GLOBAL: gl_25480
static const std::string g_itemLinkTagOpen = "<ITEML>";

// GLOBAL: gl_24595
static const std::string g_itemTagBegin = "<ITEM>";

// GLOBAL: gl_24596
static const std::string g_itemTagEnd = "</ITEM>";

// GLOBAL: gl_24599
static const std::string g_questTagBegin = "<QUEST>";

// GLOBAL: gl_24600
static const std::string g_questTagEnd = "</QUEST>";

// OFFSET: 0x00A7CA90
// FUNCTION: fn_16949
void ExtractNavigationTextLinks(const std::string& text, std::vector<TextLink>& links) {
  ExtractTextLinks(text, g_naviTagBegin, g_naviTagEnd, links);
}

// GLOBAL: gl_24597
static const std::string g_tipBoxTagBegin = "<TIPBOX>";

// GLOBAL: gl_24598
static const std::string g_tipBoxTagEnd = "</TIPBOX>";

// OFFSET: 0x00A7D690
// FUNCTION: fn_16951
void ExtractTipBoxTextLinks(const std::string& text, std::vector<TextLink>& links) {
  ExtractTextLinks(text, g_tipBoxTagBegin, g_tipBoxTagEnd, links);
}

// GLOBAL: gl_24591
static const std::string g_urlTagBegin = "<URL>";

// GLOBAL: gl_24592
static const std::string g_urlTagEnd = "</URL>";

// OFFSET: 0x00A7D6B0
// FUNCTION: fn_16952
void ExtractUrlTextLinks(const std::string& text, std::vector<TextLink>& links) {
  ExtractTextLinks(text, g_urlTagBegin, g_urlTagEnd, links);
}

// OFFSET: 0x00A81A70
// FUNCTION: fn_16980
bool HasTextLinks(const std::string& text) {
  return text.find(g_urlTagBegin) != std::string::npos || text.find(g_itemTagBegin) != std::string::npos || text.find(g_tipBoxTagBegin) != std::string::npos ||
         text.find(g_naviTagBegin) != std::string::npos || text.find(g_itemLinkTagOpen) != std::string::npos || text.find(g_questTagBegin) != std::string::npos;
}

// OFFSET: 0x008791E0
// FUNCTION: fn_11214
std::string ExpandTextEscapes(const std::string& text) {
  std::string result = text;
  auto position = result.find("\\n");

  while (position != std::string::npos) {
    result.replace(position, 2, "\n");
    position = result.find("\\n");
  }

  position = result.find("\\r");

  while (position != std::string::npos) {
    result.replace(position, 2, "\r");
    position = result.find("\\r");
  }

  return result;
}

// OFFSET: 0x00A7EDC0
// FUNCTION: fn_16961
std::string RemoveTextColors(const char* text) {
  const char* end = text + std::char_traits<char>::length(text);
  const char* current = text;
  std::string result;

  while (current < end) {
    const char* next = SegmentEnd(current, end);
    result += std::string(current, next);

    if (next >= end) {
      break;
    }

    current = next != nullptr ? (*next == '^' ? next + 7 : next) : nullptr;
  }

  return result;
}

// OFFSET: 0x00A7F660
// FUNCTION: fn_16966
const std::string DisplayTextTag(const std::string& source, const std::string& begin, const std::string& end) {
  std::string text = source;
  int position = static_cast<int>(text.find(begin));

  if (position == -1) {
    return source;
  }

  std::string result;

  do {
    if (position > 0) {
      result += text.substr(0, position);
    }

    const int content = position + static_cast<int>(begin.size());
    std::string tail = text.substr(content);
    const int close = static_cast<int>(tail.find(end));

    if (close == -1) {
      position = content;
    } else {
      result += tail.substr(0, close);
      text = tail.substr(close + end.size());
      position = static_cast<int>(text.find(begin));
    }
  } while (position != -1);

  return result + text;
}

// OFFSET: 0x00A7F510
// FUNCTION: fn_16963
const std::string DisplayItemTextTag(const std::string& text) {
  return DisplayTextTag(text, g_itemTagBegin, g_itemTagEnd);
}

// OFFSET: 0x00A7F580
// FUNCTION: fn_16964
const std::string DisplayNavigationTextTag(const std::string& text) {
  return DisplayTextTag(text, g_naviTagBegin, g_naviTagEnd);
}

// OFFSET: 0x00A7F5F0
// FUNCTION: fn_16965
const std::string DisplayQuestTextTag(const std::string& text) {
  return DisplayTextTag(text, g_questTagBegin, g_questTagEnd);
}

// OFFSET: 0x00A7FE50
// FUNCTION: fn_16967
const std::string DisplayTipBoxTextTag(const std::string& text) {
  return DisplayTextTag(text, g_tipBoxTagBegin, g_tipBoxTagEnd);
}

// OFFSET: 0x00A7FEC0
// FUNCTION: fn_16968
const std::string DisplayUrlTextTag(const std::string& text) {
  return DisplayTextTag(text, g_urlTagBegin, g_urlTagEnd);
}

// OFFSET: 0x00A7EF40
// FUNCTION: fn_16962
const std::string RemoveTextLinkInfo(const std::string& source) {
  std::string text = source;
  int position = static_cast<int>(text.find(g_infoTagBegin));

  if (position == -1) {
    return source;
  }

  std::string result;

  do {
    if (position > 0) {
      result += text.substr(0, position);
    }

    const int close = static_cast<int>(text.find(g_infoTagEnd));

    if (close != -1) {
      text = text.substr(close + g_infoTagEnd.size());
      position = static_cast<int>(text.find(g_infoTagBegin));
    }
  } while (position != -1);

  result += text;
  return result;
}

// OFFSET: 0x00A7CAB0
// FUNCTION: fn_16950
void ExtractTextLinks(const std::string& source, const std::string& begin, const std::string& end, std::vector<TextLink>& links) {
  std::string text = source;
  auto position = text.find(begin);

  if (position == std::string::npos) {
    return;
  }

  std::uint32_t offset;

  {
    std::string prefix = text.substr(0, position);
    prefix = RemoveTextLinkInfo(prefix);
    offset = static_cast<std::uint32_t>(prefix.size());
  }

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
    const auto info = display.find(g_infoTagBegin);

    if (info != std::string::npos) {
      display = tail.substr(0, info);
      metadata = tail.substr(info + g_infoTagBegin.size());
      const auto infoEnd = metadata.find(g_infoTagEnd);

      if (infoEnd != std::string::npos) {
        metadata = metadata.substr(0, infoEnd);
      }
    }

    TextLink link;
    link.position = offset;
    link.display = display;
    link.metadata = metadata;
    links.push_back(link);
    offset += static_cast<std::uint32_t>(display.size());

    text = tail.substr(close + end.size());
    position = text.find(begin);

    if (position != std::string::npos) {
      std::string prefix = text.substr(0, position);
      prefix = RemoveTextLinkInfo(prefix);
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
