#include "UI/Text/NavigationLink.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mbstring.h>
#include <new>
#include <regex>
#include <vector>
#include <windows.h>
#include <shellapi.h>
#include <timeapi.h>

#include "Client/Application/ApplicationWindow.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Network/TextEncoding.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Dialogs/UITipboxWnd.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/CreateLineInfo.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/TextTokens.h"

namespace LineCutter {
  class UIText : public UIWindow {
  public:
    void Paint() override;
    void Slot25(int x, int y) override;
    void Slot26(int x, int y) override;
    void Slot27(int x, int y) override;
    void ResetCursor(int x, int y) override;
    void Slot31(int x, int y) override;
    void Slot32(int x, int y) override;
    void Slot33(int x, int y) override;
    void Slot35(int value) override;

    /* 007C */ std::uint32_t m_color;
    /* 0080 */ int m_fontHeight;
    /* 0084 */ unsigned char m_bold;
    /* 0085 */ unsigned char m_italic;
    /* 0088 */ unsigned char m_outlined;
    /* 008C */ std::uint32_t m_outlineColor;
    /* 0090 */ unsigned char m_shadowed;
    /* 0094 */ std::uint32_t m_shadowColor;
    /* 0098 */ std::string m_text;
  };
}

class UIEmotion : public UIWindow {
public:
  void Slot19() override;

protected:
  /* 007C */ unsigned m_animation;
  /* 0080 */ unsigned m_frame;
  /* 0084 */ unsigned m_maxLoops;
  /* 0088 */ std::uint32_t unknown_0088;
  /* 008C */ CActRes* m_action;
  /* 0090 */ std::uint32_t m_startTime;
  /* 0094 */ unsigned m_loops;
};

class UIItemImage : public UIWindow {
public:
  void Paint() override;

protected:
  /* 007C */ std::string m_itemId;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(LineCutter::UIText) == 0xB0);
#endif

// GLOBAL: gl_24668
NavigationLinkParser* g_navigationLinkParser;

// OFFSET: 0x00831FE0
// FUNCTION: fn_10404
NavigationLinkParser::NavigationLinkParser() : m_ready(true) {
}

// OFFSET: 0x00831FF0
// FUNCTION: fn_10405
NavigationLinkParser::~NavigationLinkParser() {
}

// OFFSET: 0x00833960
// FUNCTION: fn_10412
bool CreateNavigationLinkParser() {
  if (g_navigationLinkParser) {
    return false;
  }

  g_navigationLinkParser = new (std::nothrow) NavigationLinkParser;

  if (!g_navigationLinkParser) {
    return false;
  }

  if (!g_navigationLinkParser->m_ready) {
    delete g_navigationLinkParser;
    g_navigationLinkParser = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x00833A00
// FUNCTION: fn_10413
void DestroyNavigationLinkParser() {
  if (g_navigationLinkParser) {
    delete g_navigationLinkParser;
    g_navigationLinkParser = nullptr;
  }
}

// GLOBAL: gl_24818
static const std::string g_urlTag = "<URL>";
// GLOBAL: gl_24819
static const std::string g_urlCloseTag = "</URL>";
// GLOBAL: gl_24820
static const std::string g_naviTag = "<NAVI>";
// GLOBAL: gl_24821
static const std::string g_naviCloseTag = "</NAVI>";
// GLOBAL: gl_24822
static const std::string g_infoTag = "<INFO>";
// GLOBAL: gl_24823
static const std::string g_infoCloseTag = "</INFO>";
// GLOBAL: gl_24824
static const std::string g_itemTag = "<ITEM>";
// GLOBAL: gl_24825
static const std::string g_itemCloseTag = "</ITEM>";
// GLOBAL: gl_24826
static const std::string g_itemLinkTag = "<ITEML>";
// GLOBAL: gl_24827
static const std::string g_itemLinkCloseTag = "</ITEML>";
// GLOBAL: gl_24828
static const std::string g_messageTag = "<MSG>";
// GLOBAL: gl_24829
static const std::string g_messageCloseTag = "</MSG>";
// GLOBAL: gl_24830
static const std::string g_naviLinkTag = "<NAVIL>";
// GLOBAL: gl_24831
static const std::string g_naviLinkCloseTag = "</NAVIL>";
// GLOBAL: gl_24832
static const std::string g_tipBoxTag = "<TIPBOX>";
// GLOBAL: gl_24833
static const std::string g_tipBoxCloseTag = "</TIPBOX>";
// GLOBAL: gl_24834
static const std::string g_questTag = "<QUEST>";
// GLOBAL: gl_24835
static const std::string g_questCloseTag = "</QUEST>";

// GLOBAL: gl_24836
std::vector<LineFontState> CCreateLineInfo::s_fontStackCache;

// OFFSET: 0x00832500
// FUNCTION: fn_10407
bool NavigationLinkParser::HasLink(std::string text) {
  return text.find(g_naviLinkTag) != std::string::npos || text.find(g_naviLinkCloseTag) != std::string::npos ||
         text.find(g_itemLinkTag) != std::string::npos || text.find(g_itemLinkCloseTag) != std::string::npos;
}

// OFFSET: 0x00832640
// FUNCTION: fn_10408
std::string NavigationLinkParser::ExpandMessages(std::string text) {
  std::string expanded;

  for (;;) {
    std::string work;
    if (expanded.empty()) {
      work = text;
    } else {
      work = expanded;
    }

    const std::size_t begin = work.find(g_messageTag);
    if (begin == std::string::npos) {
      return text;
    }

    work = work.substr(begin + std::strlen(g_messageTag.c_str()));
    const std::size_t end = work.find(g_messageCloseTag);
    if (end == std::string::npos) {
      return text;
    }

    work = work.substr(0, end);
    int messageId;
    if (work.find("MSI_") != std::string::npos) {
      messageId = FindMessageId(work.c_str());
    } else {
      messageId = std::atoi(work.c_str());
    }

    const std::string message = FormatMessageArguments(work, messageId);
    work.clear();
    work = expanded.empty() ? text : expanded;
    const std::string head = work.substr(0, begin);
    const std::string tail = work.substr(work.find(g_messageCloseTag) + std::strlen(g_messageCloseTag.c_str()));
    expanded.clear();
    expanded += head;
    expanded += message;
    expanded += tail;

    if (expanded.find(g_messageTag) == std::string::npos) {
      return expanded;
    }
  }
}

// OFFSET: 0x00833160
// FUNCTION: fn_10409
int DecodeNavigationNumber(std::string value) {
  constexpr char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const std::size_t length = value.size();
  std::uint32_t result = 0;

  for (std::size_t index = 0; index < length; ++index) {
    const char* digit = std::strchr(digits, value[index]);

    if (!digit || *digit == '\0') {
      continue;
    }

    std::uint32_t place = 1;

    for (std::size_t exponent = index + 1; exponent < length; ++exponent) {
      place *= 62;
    }

    result += static_cast<std::uint32_t>(digit - digits) * place;
  }

  return static_cast<int>(result);
}

// OFFSET: 0x00833270
// FUNCTION: fn_10410
std::string EncodeNavigationNumber(int value) {
  const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::vector<char> characters;

  do {
    characters.push_back(digits[value % 62]);
    value /= 62;
  } while (value >= 62);
  characters.push_back(digits[value]);

  std::string result;
  const std::size_t count = characters.size();
  for (std::size_t index = 0; index < count; ++index) {
    result += characters[count - 1 - index];
  }
  return result;
}

// OFFSET: 0x00835890
// FUNCTION: fn_10420
bool ParseNavigationLink(std::string value, std::string& name, int& x, int& y) {
  const auto end = value.find("</NAVIL>");

  if (end == std::string::npos) {
    return true;
  }

  const auto coordinates = end - 4;
  const std::string tail = value.substr(coordinates);
  x = DecodeNavigationNumber(tail.substr(0, 2));
  y = DecodeNavigationNumber(tail.substr(2, 2));
  const std::string head = value.substr(0, coordinates);
  const auto begin = head.find("<NAVIL>");

  if (begin != std::string::npos) {
    name = head.substr(begin + 7);
  }

  return !name.empty();
}

// OFFSET: 0x00835600
// FUNCTION: fn_10419
std::string FormatNavigationLink(std::string value, bool brackets) {
  std::string name;
  int x = 0;
  int y = 0;
  ParseNavigationLink(std::move(value), name, x, y);
  char text[128] = {};

  if (brackets) {
    if (x > 999 && y > 999) {
      std::sprintf(text, "<%s>", name.c_str());
    } else {
      std::sprintf(text, "<%s %d,%d>", name.c_str(), x, y);
    }
  } else {
    if (x > 999 && y > 999) {
      std::sprintf(text, "%s", name.c_str());
    } else {
      std::sprintf(text, "%s %d,%d", name.c_str(), x, y);
    }
  }

  return text;
}

// OFFSET: 0x00835B00
// FUNCTION: fn_10422
std::pair<bool, std::string> RemoveLineBreakTag(std::string value) {
  std::string text = value;
  const std::size_t position = text.find("<NR>");

  if (position == std::string::npos) {
    return std::pair<bool, std::string>(false, value);
  }

  std::string head = text.substr(0, position);
  const std::string tail = text.substr(position + std::strlen("<NR>"));
  head.append(tail.c_str(), tail.size());
  return std::pair<bool, std::string>(true, head);
}

// OFFSET: 0x00833550
// FUNCTION: fn_10411
void DisplayNavigationLink(std::string& output, std::string value) {
  std::string text = value;
  output = text;
  const auto begin = text.find(g_naviLinkTag);

  if (begin != std::string::npos) {
    output.clear();
    const std::string head = text.substr(0, begin);
    text = text.substr(begin);
    const auto end = text.find(g_naviLinkCloseTag);

    if (end != std::string::npos) {
      const std::string rest = text.substr(end + std::strlen(g_naviLinkCloseTag.c_str()));
      text = text.substr(0, end + std::strlen(g_naviLinkCloseTag.c_str()));
      const std::string link = FormatNavigationLink(text, true);
      output += head;
      output += link;
      output += rest;
    }
  }
}

// OFFSET: 0x00833A20
// FUNCTION: fn_10414
bool ExtractInnerItemLink(std::string& text, std::string& link, int& offset, const char* source) {
  std::size_t closePos = text.find(g_itemLinkCloseTag);

  if (closePos == std::string::npos) {
    return false;
  }

  do {
    text = text.substr(0, closePos);
    std::size_t openPos = text.find(g_itemLinkTag);

    if (closePos != std::string::npos && openPos == std::string::npos) {
      offset += g_itemLinkCloseTag.size();
      text = source;
      text = text.substr(offset);
      return true;
    }

    closePos = text.find(g_itemLinkCloseTag);

    if (openPos != std::string::npos) {
      for (;;) {
        offset += openPos;
        text = text.substr(openPos + g_itemLinkTag.size());
        openPos = text.find(g_itemLinkTag);
        closePos = text.find(g_itemLinkCloseTag);

        if (openPos == std::string::npos) {
          break;
        }

        offset += g_itemLinkTag.size();
      }

      link = text;
      text = source;
      text = text.substr(link.size() + offset + g_itemLinkTag.size() + g_itemLinkCloseTag.size());
    }
  } while (closePos != std::string::npos);

  return true;
}

// OFFSET: 0x00834980
// FUNCTION: fn_10417
void ExtractNavigationLink(std::string& output, std::string value) {
  output = value;
  const auto begin = value.find("<NAVIL>");

  if (begin != std::string::npos) {
    output = value.substr(begin);
    const auto end = output.find("</NAVIL>");

    if (end != std::string::npos) {
      output.resize(end + 8);
    }
  }
}

// OFFSET: 0x00834160
// FUNCTION: fn_10415
static void SplitMessageArguments(const char* text, std::vector<std::string>& arguments) {
  std::string remaining = text;
  std::size_t separator = remaining.find(',');

  while (separator != std::string::npos) {
    std::string argument = remaining.substr(0, separator);
    arguments.emplace_back(argument.data(), argument.size());
    remaining = remaining.substr(separator + 1);
    separator = remaining.find(',');
  }

  if (!remaining.empty()) {
    arguments.emplace_back(remaining.data(), remaining.size());
  }
}

// OFFSET: 0x00834C30
// FUNCTION: fn_10418
std::string FormatMessageArguments(std::string value, int messageId) {
  const std::string text(value.data(), value.size());
  std::string result = LookupMessage(messageId);
  const std::size_t separator = text.find(',');

  if (separator != std::string::npos) {
    const std::string tail = text.substr(separator + 1);
    std::vector<std::string> arguments;
    arguments.reserve(8);
    SplitMessageArguments(tail.c_str(), arguments);

    for (std::size_t index = 0; index < arguments.size(); ++index) {
      const std::size_t percent = result.find('%');
      if (percent != std::string::npos) {
        unsigned length = 1;
        char conversion[8] = {};
        std::size_t position = percent + 1;

        for (;;) {
          strcpy_s(conversion, sizeof(conversion), result.substr(position, 1).c_str());
          if (conversion[0] >= 'A') {
            break;
          }
          ++length;
          ++position;
        }

        const std::size_t end = percent + length + 1;
        const std::string format = result.substr(0, end);
        const std::string remainder = result.substr(end);
        char buffer[164] = {};
        std::string& argument = arguments[index];

        if (_mbsstr(reinterpret_cast<const unsigned char*>(conversion), reinterpret_cast<const unsigned char*>("s"))) {
          if (argument.find("MsgStr(") != std::string::npos) {
            argument = argument.substr(7);
            argument = argument.substr(0, argument.size() - 1);
            const std::string nested = LookupMessage(std::atoi(argument.c_str()));
            std::sprintf(buffer, format.c_str(), nested.c_str());
          } else {
            std::sprintf(buffer, format.c_str(), argument.c_str());
          }
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>(conversion), reinterpret_cast<const unsigned char*>("d"))) {
          std::sprintf(buffer, format.c_str(), std::atoi(argument.c_str()));
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>(conversion), reinterpret_cast<const unsigned char*>("f"))) {
          std::sprintf(buffer, format.c_str(), std::atof(argument.c_str()));
        }

        result = buffer + remainder;
      }
    }
  }

  return result;
}

// OFFSET: 0x0083B250
// FUNCTION: fn_10473
void LineCutter::UIText::Slot25(int x, int y) {
  if (this->parent) {
    this->parent->Slot25(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x0083B220
// FUNCTION: fn_10472
void LineCutter::UIText::Slot26(int x, int y) {
  if (this->parent) {
    this->parent->Slot26(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x0083B400
// FUNCTION: fn_10476
void LineCutter::UIText::Slot27(int x, int y) {
  if (this->parent) {
    this->parent->Slot27(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x0083B430
// FUNCTION: fn_10477
void LineCutter::UIText::ResetCursor(int x, int y) {
  if (this->parent) {
    this->parent->ResetCursor(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x0083B280
// FUNCTION: fn_10474
void LineCutter::UIText::Slot31(int x, int y) {
  if (this->parent) {
    this->parent->Slot31(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x0083B630
// FUNCTION: fn_10480
void LineCutter::UIText::Slot32(int x, int y) {
  if (this->parent) {
    this->parent->Slot32(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x0083B660
// FUNCTION: fn_10481
void LineCutter::UIText::Slot33(int x, int y) {
  if (this->parent) {
    this->parent->Slot33(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x0083B690
// FUNCTION: fn_10482
void LineCutter::UIText::Slot35(int value) {
  if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x0083B150
// FUNCTION: fn_10471
void LineCutter::UIText::Paint() {
  this->ClearSurface(0xFFFF00FF);
  const TextLocale locale = {};

  if (this->m_shadowed) {
    this->DrawTextAt(
      g_fontCache, locale, 1, 1, this->m_text.c_str(), this->m_text.size(), 0, this->m_fontHeight, this->m_shadowColor, this->m_bold,
      this->m_italic
    );
  }

  if (this->m_outlined == 1) {
    DrawOutlinedText(
      *this, 1, 1, this->m_text.c_str(), this->m_text.size(), this->m_color, this->m_outlineColor, 0, this->m_fontHeight, this->m_bold, this->m_italic
    );
  } else {
    this->DrawTextAt(
      g_fontCache, locale, 0, 0, this->m_text.c_str(), this->m_text.size(), 0, this->m_fontHeight, this->m_color, this->m_bold, this->m_italic
    );
  }
}

// OFFSET: 0x0083B460
// FUNCTION: fn_10478
void UIEmotion::Slot19() {
  if (!this->m_action || this->m_action->Speed(this->m_animation) == 0.0f) {
    return;
  }

  const float elapsed = static_cast<float>(timeGetTime() - this->m_startTime);
  const unsigned frames = static_cast<unsigned>(elapsed / (this->m_action->Speed(this->m_animation) * 24.0f));

  if (this->m_maxLoops != 0) {
    if (this->m_loops > this->m_maxLoops) {
      return;
    }

    if (frames >= this->m_action->FrameCount(this->m_animation) * this->m_loops) {
      this->m_loops = this->m_loops + 1;
    }
  }

  this->m_frame = frames % this->m_action->FrameCount(this->m_animation);
  this->Invalidate();
}

// OFFSET: 0x0083B0A0
// FUNCTION: fn_10470
void UIItemImage::Paint() {
  if (!this->geometry.surface || this->m_itemId.empty()) {
    return;
  }

  this->ClearSurface(0xFFFF00FF);
  char path[128] = {};
  ItemDescriptions descriptions{g_itemDescriptions};
  descriptions.IconPath(path, this->m_itemId.c_str(), 1);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));

  if (bitmap) {
    this->DrawBitmap(0, 0, &bitmap->image, true);
  }
}

// OFFSET: 0x00837390
// FUNCTION: fn_10441
CCreateLineInfo::~CCreateLineInfo() {
  if (!this->m_fontStack.empty()) {
    this->m_fontStack.swap(s_fontStackCache);
  }
}

// OFFSET: 0x008379D0
// FUNCTION: fn_10448
bool CCreateLineInfo::CompareTag(std::string text, std::string keyword, bool exactMatch, bool caseSensitive) {
  if (text.empty() || keyword.empty()) {
    return false;
  }

  if (!caseSensitive) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
    std::transform(keyword.begin(), keyword.end(), keyword.begin(), [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
  }

  if (exactMatch == true) {
    return text.compare(keyword) == 0;
  }

  return text.compare(0, keyword.size(), keyword) == 0;
}

// OFFSET: 0x00837B40
// FUNCTION: fn_10449
void CCreateLineInfo::AppendText(const std::string& text) {
  if (this->m_segments.empty()) {
    this->AddSegment(LINE_SEGMENT_TEXT);
  }
  this->m_segments.back().text += text;
}

// OFFSET: 0x00837B80
// FUNCTION: fn_10450
void CCreateLineInfo::AddWindow(UIWindow* window) {
  if (this->m_lines.empty()) {
    this->NewLine();
  }
  this->m_lines.back().push_back(window);
}

// OFFSET: 0x00839310
// FUNCTION: fn_10457
std::size_t CCreateLineInfo::FindTag(std::string text, std::string keyword, bool caseSensitive) {
  if (text.empty() || keyword.empty()) {
    return std::string::npos;
  }

  if (!caseSensitive) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
    std::transform(keyword.begin(), keyword.end(), keyword.begin(), [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
  }

  return text.find(keyword, 0);
}

// OFFSET: 0x00839420

// OFFSET: 0x00839860

// OFFSET: 0x00839D60
// FUNCTION: fn_10460
std::pair<std::string, std::string> CCreateLineInfo::GetLinkTags(int type) {
  switch (type) {
  case LINE_SEGMENT_URL:
    return std::make_pair(g_urlTag, g_urlCloseTag);
  case LINE_SEGMENT_NAVI:
    return std::make_pair(g_naviTag, g_naviCloseTag);
  case LINE_SEGMENT_ITEM:
    return std::make_pair(g_itemTag, g_itemCloseTag);
  case LINE_SEGMENT_ITEM_LINK:
    return std::make_pair(g_itemLinkTag, g_itemLinkCloseTag);
  case LINE_SEGMENT_TIPBOX:
    return std::make_pair(g_tipBoxTag, g_tipBoxCloseTag);
  case LINE_SEGMENT_QUEST:
    return std::make_pair(g_questTag, g_questCloseTag);
  case LINE_SEGMENT_NAVI_LINK:
    return std::make_pair(g_naviLinkTag, g_naviLinkCloseTag);
  case LINE_SEGMENT_MESSAGE:
    return std::make_pair(g_messageTag, g_messageCloseTag);
  default:
    return std::pair<std::string, std::string>("", "");
  }
}

// OFFSET: 0x00839FA0

// OFFSET: 0x0083A180
// FUNCTION: fn_10462
void CCreateLineInfo::ApplyFont(std::string tag) {
  bool changed = false;

  std::size_t position = FindTag(tag, "SIZE", false);
  if (position != std::string::npos) {
    const std::string value = tag.substr(position, tag.size() - position);
    int size = 0;
    std::sscanf(value.c_str(), "%*[^=]=%d", &size);
    this->m_fontHeight = size != 0 ? size : this->m_fontHeight;
    changed = true;
  }

  position = FindTag(tag, "COLOR", false);
  if (position != std::string::npos) {
    const std::string value = tag.substr(position, tag.size() - position);
    char digits[7] = {};
    std::sscanf(value.c_str(), "%*[^=]=%6s", digits);
    char* end = nullptr;
    const unsigned long color = std::strtol(digits, &end, 16);
    if (end > digits && end - digits == 6) {
      changed = true;
      this->m_color = RGB((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
    }
  }

  if (changed == true) {
    this->m_fontStack.push_back(LineFontState{this->m_fontHeight, this->m_color});
    this->m_newSegment = true;
  }
}

// OFFSET: 0x0083A820
// FUNCTION: fn_10464
int CCreateLineInfo::AddLink(int index, int type) {
  const std::pair<std::string, std::string> tags = GetLinkTags(type);
  const std::size_t end = this->m_text.find(tags.second, index);

  if (end != std::string::npos) {
    const std::string link = this->m_text.substr(index, end + tags.second.size() - index);
    this->AddSegment(type);
    this->AppendText(link);
    this->m_newSegment = true;
    index = static_cast<int>(end + tags.second.size());
  }

  return index;
}

// OFFSET: 0x0083AA40
// FUNCTION: fn_10465
void CCreateLineInfo::NewLine() {
  std::vector<UIWindow*> line;
  this->m_lines.push_back(line);
}

// OFFSET: 0x0083AB90
// FUNCTION: fn_10466
void CCreateLineInfo::AddSegment(int type) {
  LineTextSegment segment(std::string(), LineTextStyle{this->m_color, this->m_fontHeight, this->m_bold > 0, this->m_italic > 0});
  segment.type = type;
  this->m_segments.push_back(segment);
  this->m_newSegment = false;
}

// OFFSET: 0x0083AF20
// FUNCTION: fn_10468
void CCreateLineInfo::ParseText() {
  const int length = static_cast<int>(this->m_text.size());
  int index = 0;

  while (index < length) {
    int next = this->ParseTag(index);
    if (index == next) {
      next = this->ParseChar(index);
    }
    index = next;
  }
}

// OFFSET: 0x0083B6B0
// FUNCTION: fn_10483
void ShowTextButtonItem(UITextButton* button) {
  if (!button) {
    return;
  }

  if (button->m_lines.size() == 0) {
    return;
  }

  const std::string itemId = button->GetLine(0);
  const int index = std::atoi(button->GetLine(1).c_str());
  ITEM_INFO item;
  item.unknown_0000[0] = static_cast<std::uint32_t>(index);
  item.itemId = itemId;
  item.identified = 1;
  UIWindow* window = g_windowMgr.EnsureWindow(0xC);
  if (window) {
    window->SendMsg(nullptr, 0x18, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x0083BB50
// FUNCTION: fn_10486
void ShowTextButtonTip(UITextButton* button) {
  if (!button) {
    return;
  }

  if (button->m_lines.size() == 0) {
    return;
  }

  const int tip = std::atoi(button->GetLine(0).c_str());
  UITipboxWnd* window = static_cast<UITipboxWnd*>(g_windowMgr.EnsureWindow(0x13B));
  if (window) {
    window->ShowTip(tip);
  }
}

// OFFSET: 0x0083BBC0
// FUNCTION: fn_10487
void OpenTextButtonLink(UITextButton* button) {
  if (!button) {
    return;
  }

  if (button->m_lines.size() == 0) {
    return;
  }

  const std::string line = button->GetLine(0);
  if (!line.empty()) {
    ShellExecuteA(hWnd, "open", line.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
  }
}

// OFFSET: 0x0083BD80
// FUNCTION: fn_10488
int CCreateLineInfo::ParseEmotion(int index) {
  const std::string text = this->m_text.substr(index, this->m_text.size() - index);
  std::cmatch match;

  bool missing = !std::regex_search(text.c_str(), text.c_str() + text.size(), match, std::regex("^\\^e\\[\\d{1,3}\\](\\[\\d\\])*"));
  if (missing) {
    missing = !std::regex_search(text.c_str(), text.c_str() + text.size(), match, std::regex("^\\^e\\[\\d{1,3}\\s*,\\s*\\d{1,3}\\](\\[\\d\\])*"));
    if (missing) {
      return index;
    }
  }

  if (match.size() >= 2) {
    this->AddSegment(LINE_SEGMENT_EMOTION);
    this->AppendText(match[0].str());
    this->m_newSegment = true;
    index += static_cast<int>(match[0].length());
  }

  return index;
}

// INLINED
inline void CCreateLineInfo::CloseFont(std::string tag) {
  if (!this->m_fontStack.empty()) {
    this->m_fontStack.pop_back();
    if (this->m_fontStack.empty()) {
      this->m_fontHeight = this->m_defaultFontHeight;
      this->m_color = this->m_defaultColor;
    } else {
      this->m_fontHeight = this->m_fontStack.back().fontHeight;
      this->m_color = this->m_fontStack.back().color;
    }
    this->m_newSegment = true;
  }
}

// INLINED
inline void CCreateLineInfo::OpenBold(std::string tag) {
  if (this->m_bold == 0) {
    this->m_newSegment = true;
  }
  ++this->m_bold;
}

// INLINED
inline void CCreateLineInfo::CloseBold(std::string tag) {
  if (--this->m_bold == 0) {
    this->m_newSegment = true;
  }
}

// INLINED
inline void CCreateLineInfo::OpenItalic(std::string tag) {
  if (this->m_italic == 0) {
    this->m_newSegment = true;
  }
  ++this->m_italic;
}

// INLINED
inline void CCreateLineInfo::CloseItalic(std::string tag) {
  if (--this->m_italic == 0) {
    this->m_newSegment = true;
  }
}

// OFFSET: 0x0083BFF0
// FUNCTION: fn_10489
int CCreateLineInfo::ParseMarkup(int index) {
  if (this->m_text.at(index) != '<') {
    return index;
  }

  const std::size_t end = this->m_text.find('>', index);
  if (end == std::string::npos) {
    return index;
  }

  const std::string tag = this->m_text.substr(index + 1, end - index - 1);
  if (CompareTag(tag, "FONT", false, false) == true) {
    this->ApplyFont(tag);
  } else if (CompareTag(tag, "/FONT", true, false) == true) {
    this->CloseFont(tag);
  } else if (CompareTag(tag, "B", true, false) == true) {
    this->OpenBold(tag);
  } else if (CompareTag(tag, "/B", true, false) == true) {
    this->CloseBold(tag);
  } else if (CompareTag(tag, "I", true, false) == true) {
    this->OpenItalic(tag);
  } else if (CompareTag(tag, "/I", true, false) == true) {
    this->CloseItalic(tag);
  } else {
    return index;
  }

  return static_cast<int>(end + 1);
}

// OFFSET: 0x0083C3D0
// FUNCTION: fn_10490
int CCreateLineInfo::ParseLink(int index) {
  const std::string text = this->m_text.substr(index, this->m_text.size() - index);
  int type;

  if (CompareTag(text, g_urlTag, false, true) == true) {
    type = LINE_SEGMENT_URL;
  } else if (CompareTag(text, g_itemTag, false, true) == true) {
    type = LINE_SEGMENT_ITEM;
  } else if (CompareTag(text, g_itemLinkTag, false, true) == true) {
    type = LINE_SEGMENT_ITEM_LINK;
  } else if (CompareTag(text, g_naviTag, false, true) == true) {
    type = LINE_SEGMENT_NAVI;
  } else if (CompareTag(text, g_naviLinkTag, false, true) == true) {
    type = LINE_SEGMENT_NAVI_LINK;
  } else if (CompareTag(text, g_messageTag, false, true) == true) {
    type = LINE_SEGMENT_MESSAGE;
  } else if (CompareTag(text, g_questTag, false, true) == true) {
    type = LINE_SEGMENT_QUEST;
  } else if (CompareTag(text, g_tipBoxTag, false, true) == true) {
    type = LINE_SEGMENT_TIPBOX;
  } else {
    return index;
  }

  return this->AddLink(index, type);
}

// OFFSET: 0x0083C6A0
// FUNCTION: fn_10491
int CCreateLineInfo::ParseItemImage(int index) {
  const std::string text = this->m_text.substr(index, this->m_text.size() - index);
  std::cmatch match;

  const bool missing = !std::regex_search(text.c_str(), text.c_str() + text.size(), match, std::regex("^\\^i(\\[\\d{1,7}\\])*"));
  if (!missing && match.size() >= 2) {
    this->AddSegment(LINE_SEGMENT_ITEM_IMAGE);
    this->AppendText(match[0].str());
    this->m_newSegment = true;
    index += static_cast<int>(match[0].length());
  }

  return index;
}

// OFFSET: 0x0083C890
// FUNCTION: fn_10492
int CCreateLineInfo::ParseItemName(int index) {
  const std::string keyword = "^nItemID^";

  if (index == static_cast<int>(this->m_text.find(keyword, index))) {
    std::string digits;
    for (long long position = index + keyword.size(); position < this->m_text.size(); ++position) {
      const char ch = this->m_text.at(static_cast<std::size_t>(position));
      if (!g_colorHexDigit[static_cast<unsigned char>(ch)]) {
        break;
      }
      digits.push_back(ch);
    }

    if (!digits.empty()) {
      ITEM_INFO item;
      item.SetItemId(std::atoi(digits.c_str()));
      char name[260];
      std::memset(name, 0, sizeof(name));
      int capacity = sizeof(name);
      ItemDescriptions descriptions{g_itemDescriptions};
      descriptions.Name(item, name, capacity, 0);
      this->AddSegment(LINE_SEGMENT_TEXT);
      this->AppendText(std::string(name));
      index += static_cast<int>(digits.size()) + 9;
      this->m_newSegment = true;
    }
  }

  return index;
}

// OFFSET: 0x0083CDA0
// FUNCTION: fn_10493
int CCreateLineInfo::ParseChar(int index) {
  const std::string text = this->m_text.substr(index, this->m_text.size() - index);
  const std::string character = NextCharacter(text);

  if (character.empty()) {
    return index + 1;
  }

  if (character == "\n") {
    this->AddSegment(LINE_SEGMENT_TEXT);
    this->AppendText(character);
    this->m_newSegment = true;
  } else if (character != "\r") {
    if (character == " ") {
      if (index > 0) {
        const std::string previousText = this->m_text.substr(index - 1, this->m_text.size() - (index - 1));
        const std::string previous = NextCharacter(previousText);
        if (previous == "\n") {
          this->AddSegment(LINE_SEGMENT_TEXT);
        }
        this->AppendText(character);
        this->m_newSegment = true;
      } else {
        this->AppendText(character);
        this->m_newSegment = true;
      }
    } else {
      if (this->m_newSegment == true) {
        this->AddSegment(LINE_SEGMENT_TEXT);
      }
      this->AppendText(character);
    }
  }

  return index + static_cast<int>(character.size());
}

// OFFSET: 0x0083CFE0
// FUNCTION: fn_10494
std::string CCreateLineInfo::NextCharacter(std::string text) {
  const char* begin = text.c_str();
  const char* end = CharNextExA(static_cast<WORD>(g_textCodePage), begin, 0);
  if (!end) {
    return "";
  }
  return std::string(begin, end);
}

// OFFSET: 0x0083D090
// FUNCTION: fn_10495
int CCreateLineInfo::ParseTag(int index) {
  if (this->m_text.at(index) == '^') {
    int next = index;
    const std::string code = this->m_text.substr(index + 1, 6);
    const char* digits = code.c_str();
    if (digits) {
      char* end = nullptr;
      const unsigned long value = std::strtol(digits, &end, 16);
      if (end > digits && end - digits == 6) {
        next = index + 7;
        const std::uint32_t color = RGB((value >> 16) & 0xFF, (value >> 8) & 0xFF, value & 0xFF);
        this->m_newSegment = true;
        this->m_defaultColor = color;
        this->m_color = color;
      }
    }
    if (index != next) {
      return next;
    }
  }

  int next = this->ParseItemName(index);
  if (index != next) {
    return next;
  }
  next = this->ParseEmotion(index);
  if (index != next) {
    return next;
  }
  next = this->ParseItemImage(index);
  if (index != next) {
    return next;
  }
  next = this->ParseLink(index);
  if (index != next) {
    return next;
  }
  next = this->ParseMarkup(index);
  return index != next ? next : index;
}

// OFFSET: 0x0083D1F0
// FUNCTION: fn_10496
void CCreateLineInfo::ArrangeLines() {
  int y = 0;

  for (auto lineIt = this->m_lines.begin(); lineIt != this->m_lines.end(); ++lineIt) {
    std::vector<UIWindow*>& line = *lineIt;
    unsigned lineHeight = 0;
    for (UIWindow* window : line) {
      lineHeight = std::max(lineHeight, static_cast<unsigned>(window->geometry.height));
    }

    switch (this->m_align) {
    case 0: {
      int x = 0;
      for (auto it = line.begin(); it != line.end(); ++it) {
        UIWindow* window = *it;
        if (window) {
          window->Move(x, lineHeight - window->geometry.height + y);
          x += window->geometry.width;
        }
      }
      break;
    }
    case 1: {
      int x = this->m_width;
      for (auto it = line.rbegin(); it != line.rend(); ++it) {
        UIWindow* window = *it;
        if (window) {
          x -= window->geometry.width;
          window->Move(x, lineHeight - window->geometry.height + y);
        }
      }
      break;
    }
    case 2: {
      int width = 0;
      for (UIWindow* window : line) {
        width += window->geometry.width;
      }
      int x = static_cast<unsigned>(this->m_width - width) / 2;
      for (auto it = line.begin(); it != line.end(); ++it) {
        UIWindow* window = *it;
        if (window) {
          window->Move(x, lineHeight - window->geometry.height + y);
          x += window->geometry.width;
        }
      }
      break;
    }
    }

    y += this->m_lineSpacing + lineHeight;
  }

  this->m_height = y - this->m_lineSpacing;
}

// OFFSET: 0x0083D360
// FUNCTION: fn_10497
std::tuple<bool, std::string, std::string> CCreateLineInfo::SplitLinkInfo(std::string text, int type) {
  const std::pair<std::string, std::string> tags = GetLinkTags(type);
  const std::string open = tags.first;
  const std::string close = tags.second;

  if (text.empty() || open.empty() || close.empty()) {
    return std::tuple<bool, std::string, std::string>(false, "", "");
  }

  const std::size_t begin = text.find(open, 0);
  if (begin == std::string::npos) {
    return std::tuple<bool, std::string, std::string>(false, "", "");
  }

  if (text.find(close, begin) != std::string::npos) {
    const std::size_t infoBegin = text.find(g_infoTag, begin);
    if (infoBegin != std::string::npos) {
      const std::size_t infoEnd = text.find(g_infoCloseTag, infoBegin);
      if (infoEnd != std::string::npos) {
        const std::string name = text.substr(begin + open.size(), infoBegin - open.size() - begin);
        const std::string info = text.substr(infoBegin + g_infoTag.size(), infoEnd - g_infoTag.size() - infoBegin);
        return std::tuple<bool, std::string, std::string>(true, name, info);
      }
    }
  }

  return std::tuple<bool, std::string, std::string>(false, "", "");
}

// OFFSET: 0x0083D660
// FUNCTION: fn_10498
std::vector<std::string> CCreateLineInfo::SplitString(std::string text, const std::string& delimiter) {
  std::vector<std::string> parts;
  if (text.empty()) {
    return parts;
  }

  std::size_t position = text.find(delimiter, 0);
  while (position != std::string::npos) {
    parts.push_back(text.substr(0, position));
    text = text.substr(position + delimiter.size());
    position = text.find(delimiter, 0);
  }

  parts.push_back(text);
  return parts;
}
