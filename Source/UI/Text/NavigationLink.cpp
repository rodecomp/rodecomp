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
#include <timeapi.h>

#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Network/TextEncoding.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/CreateLineInfo.h"
#include "UI/Window/UIWindow.h"
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
    UIButtonContext* m_controls;
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
  ItemDescriptions* m_descriptions;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(LineCutter::UIText) == 0xB4);
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

// OFFSET: 0x00833550
// FUNCTION: fn_10411
void DisplayNavigationLink(std::string& output, std::string value) {
  output = value;
  const auto begin = value.find("<NAVIL>");

  if (begin != std::string::npos) {
    output.clear();
    const std::string head = value.substr(0, begin);
    const std::string tail = value.substr(begin);
    const auto end = tail.find("</NAVIL>");

    if (end != std::string::npos) {
      const auto extent = end + 8;
      const std::string rest = tail.substr(extent);
      output = head;
      output += FormatNavigationLink(tail.substr(0, extent), true);
      output += rest;
    }
  }
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

// OFFSET: 0x00837390

// OFFSET: 0x008379D0

// OFFSET: 0x00837B40

// OFFSET: 0x00837B80

// OFFSET: 0x00839310

// OFFSET: 0x00839D60

// OFFSET: 0x0083A180

// OFFSET: 0x0083A820

// OFFSET: 0x0083AA40

// OFFSET: 0x0083AB90

// OFFSET: 0x0083AF20

// OFFSET: 0x0083BD80

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

// OFFSET: 0x0083C3D0

// OFFSET: 0x0083C6A0

// OFFSET: 0x0083C890

// OFFSET: 0x0083CDA0

// OFFSET: 0x0083CFE0

// OFFSET: 0x0083D090

// OFFSET: 0x0083D1F0

// OFFSET: 0x0083D360

// OFFSET: 0x0083D660
