#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Navigation/NavigationLink.h"
#include "Client/Mode/CMode.h"

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
#include <shlwapi.h>
#include <timeapi.h>

#include "Client/Application/ApplicationWindow.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemLink.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Navigation/NavigationManager.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Contents/Tables/TipBoxTitles.h"
#include "Network/TextEncoding.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Dialogs/UITipboxWnd.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/CreateLineInfo.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/StringUtil.h"
#include "Utility/Text/TextEscapes.h"
#include "Utility/Text/TextTokens.h"

#include "Resource/MSGSTRINGID.h"
#include <string>

namespace LineCutter {
  class UIText : public UIWindow {
  public:
    // INLINED
    explicit UIText()
        : UIWindow(0), m_style{0, 12, false, false}, m_outlined(0), m_outlineColor(0xFFFFFF), m_shadowed(0), m_shadowColor(0xFFFFFF) {
    }

    // INLINED
    void SetText(std::string text, LineTextStyle style, unsigned char outlined, std::uint32_t outlineColor, unsigned char shadowed, std::uint32_t shadowColor) {
      this->m_text = text;
      this->m_outlineColor = outlineColor;
      this->m_shadowed = shadowed;
      this->m_style = style;
      this->m_outlined = outlined;
      this->m_shadowColor = shadowColor;
      if (outlined == 1) {
        const int textWidth = this->TextWidth(text.c_str(), 0, 0, style.fontHeight, style.bold, style.italic);
        this->Resize(textWidth + 2, style.fontHeight + 2);
      }

      this->Invalidate();
    }

    void Paint() override;
    void OnLBtnDown(int x, int y) override;
    void OnLBtnDblClk(int x, int y) override;
    void OnMouseMove(int x, int y) override;
    void ResetCursor(int x, int y) override;
    void OnLBtnUp(int x, int y) override;
    void OnRBtnDown(int x, int y) override;
    void OnRBtnUp(int x, int y) override;
    void OnWheel(int value) override;

    /* 007C */ LineTextStyle m_style;
    /* 0088 */ unsigned char m_outlined;
    /* 008C */ std::uint32_t m_outlineColor;
    /* 0090 */ unsigned char m_shadowed;
    /* 0094 */ std::uint32_t m_shadowColor;
    /* 0098 */ std::string m_text;
  };
}

class UIEmotion : public UIWindow {
public:
  explicit UIEmotion(const char* sprite, const char* action);
  explicit UIEmotion();

  void OnProcess() override;
  void Paint() override;

  /* 007C */ unsigned m_animation;
  /* 0080 */ unsigned m_frame;
  /* 0084 */ unsigned m_maxLoops;
  /* 0088 */ CSprRes* m_sprite;
  /* 008C */ CActRes* m_action;
  /* 0090 */ std::uint32_t m_startTime;
  /* 0094 */ unsigned m_loops;
};

class UIItemImage : public UIWindow {
public:
  explicit UIItemImage();

  // INLINED
  void SetItemId(std::string itemId) {
    this->m_itemId = itemId;
    this->Invalidate();
  }

  void Paint() override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;

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

// GLOBAL: gl_25500
static const std::string g_parserItemLinkTag = "<ITEML>";
// GLOBAL: gl_25501
static const std::string g_parserItemLinkCloseTag = "</ITEML>";
// GLOBAL: gl_25506
static const std::string g_parserMessageTag = "<MSG>";
// GLOBAL: gl_25507
static const std::string g_parserMessageCloseTag = "</MSG>";
// STRING: st_13792
static const std::string g_parserMessageIdPrefix = "MSI_";
// STRING: st_13793
static const std::string g_parserLineBreakTag = "<NR>";
// GLOBAL: gl_25498
static const std::string g_parserNaviLinkTag = "<NAVIL>";
// GLOBAL: gl_25499
static const std::string g_parserNaviLinkCloseTag = "</NAVIL>";

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
  return text.find(g_parserNaviLinkTag) != std::string::npos ||
         text.find(g_parserNaviLinkCloseTag) != std::string::npos ||
         text.find(g_parserItemLinkTag) != std::string::npos ||
         text.find(g_parserItemLinkCloseTag) != std::string::npos;
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

    const std::size_t begin = work.find(g_parserMessageTag);
    if (begin == std::string::npos) {
      break;
    }

    work = work.substr(begin + std::strlen(g_parserMessageTag.c_str()));
    const std::size_t end = work.find(g_parserMessageCloseTag);
    if (end == std::string::npos) {
      break;
    }

    work = work.substr(0, end);
    int messageId;
    if (work.find(g_parserMessageIdPrefix) != std::string::npos) {
      messageId = FindMessageId(work.c_str());
    } else {
      messageId = std::atoi(work.c_str());
    }

    const std::string message = this->FormatMessageArguments(work, messageId);
    work.clear();
    if (expanded.empty()) {
      work = text;
    } else {
      work = expanded;
    }
    const std::string head = work.substr(0, begin);
    const std::size_t close = work.find(g_parserMessageCloseTag);
    const std::string tail = work.substr(close + std::strlen(g_parserMessageCloseTag.c_str()));
    expanded.clear();
    expanded += head;
    expanded += message;
    expanded += tail;

    if (expanded.find(g_parserMessageTag) == std::string::npos) {
      return expanded;
    }
  }

  return text;
}

// OFFSET: 0x00833160
// FUNCTION: fn_10409
int DecodeNavigationNumber(std::string value) {
  const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const int size = static_cast<int>(value.size()) + 1;
  int result = 0;
  char* buffer = new char[size];
  strcpy_s(buffer, size, value.c_str());
  const int length = size - 1;

  for (int index = 0; index < length; ++index) {
    for (int digit = 0; digit < 62; ++digit) {
      if (digits[digit] == buffer[index]) {
        int place = 1;
        for (int exponent = 1; exponent < length - index; ++exponent) {
          place *= 62;
        }
        result += digit * place;
        break;
      }
    }
  }

  if (buffer) {
    delete[] buffer;
  }

  return result;
}

// OFFSET: 0x00833270
// FUNCTION: fn_10410
std::string NavigationLinkParser::EncodeNavigationNumber(int value) {
  const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::vector<char> characters;
  int radix = 62;

  do {
    characters.push_back(digits[value % radix]);
    value /= radix;
  } while (value >= radix);
  characters.push_back(digits[value]);

  std::string result("");
  for (std::size_t i = 0; i < characters.size(); ++i) {
    result += characters[characters.size() - 1 - i];
  }
  return result;
}

// OFFSET: 0x00835890
// FUNCTION: fn_10420
bool NavigationLinkParser::ParseNavigationLink(std::string value, std::string& name, int& x, int& y) {
  const auto end = value.find(g_parserNaviLinkCloseTag);

  if (end == std::string::npos) {
    return true;
  }

  const auto coordinates = end - 4;
  const std::string tail = value.substr(coordinates);
  x = DecodeNavigationNumber(tail.substr(0, 2));
  y = DecodeNavigationNumber(tail.substr(2, 2));
  const std::string head = value.substr(0, coordinates);
  const auto begin = head.find(g_parserNaviLinkTag);

  if (begin != std::string::npos) {
    name = head.substr(begin + std::strlen(g_parserNaviLinkTag.c_str()));
  }

  if (name.empty()) {
    return false;
  }

  return true;
}

// OFFSET: 0x00835600
// FUNCTION: fn_10419
std::string FormatNavigationLink(std::string value, bool brackets) {
  std::string name;
  int x = 0;
  int y = 0;
  GetNavigationLinkParser()->ParseNavigationLink(std::move(value), name, x, y);
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
std::pair<bool, std::string> NavigationLinkParser::RemoveLineBreakTag(std::string value) {
  std::string text = value;
  const std::size_t position = text.find(g_parserLineBreakTag);

  if (position != std::string::npos) {
    std::string head = text.substr(0, position);
    const std::string tail = text.substr(position + std::strlen(g_parserLineBreakTag.c_str()));
    head.append(tail.c_str(), tail.size());
    return std::make_pair(true, head);
  }

  return std::make_pair(false, value);
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

    if (closePos == std::string::npos || openPos != std::string::npos) {
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
    } else {
      offset += g_itemLinkCloseTag.size();
      text = source;
      text = text.substr(offset);
      return true;
    }
  } while (closePos != std::string::npos);

  return true;
}

// OFFSET: 0x00834980
// FUNCTION: fn_10417
const std::string NavigationLinkParser::ExtractNavigationLink(std::string value) {
  std::string output = value;
  const auto begin = value.find(g_parserNaviLinkTag);

  if (begin != std::string::npos) {
    output.clear();
    output = value.substr(begin);
    const auto end = output.find(g_parserNaviLinkCloseTag);

    if (end != std::string::npos) {
      output = output.substr(0, end + std::strlen(g_parserNaviLinkCloseTag.c_str()));
    }
  }

  return output;
}

// OFFSET: 0x00834160
// FUNCTION: fn_10415
void NavigationLinkParser::SplitMessageArguments(const char* text, std::vector<std::string>& arguments) {
  std::string remaining = text;
  std::size_t separator = remaining.find(',');

  while (separator != std::string::npos) {
    std::string argument = remaining.substr(0, separator);
    arguments.push_back(argument);
    remaining = remaining.substr(separator + 1);
    separator = remaining.find(',');
  }

  if (!remaining.empty()) {
    arguments.push_back(remaining);
  }
}

// OFFSET: 0x00834C30
// FUNCTION: fn_10418
std::string NavigationLinkParser::FormatMessageArguments(std::string value, int messageId) {
  const std::string text = value;
  std::string result = LookupMessage(messageId);
  const std::size_t separator = text.find(',');

  if (separator != std::string::npos) {
    const std::string tail = text.substr(separator + 1);
    std::vector<std::string> arguments;
    arguments.reserve(8);
    this->SplitMessageArguments(tail.c_str(), arguments);

    for (std::size_t index = 0; index < arguments.size(); ++index) {
      const std::size_t percent = result.find('%');
      if (percent != std::string::npos) {
        int length = 1;
        char conversion[8] = {};
        std::size_t position = percent + 1;

        for (;;) {
          strcpy_s(conversion, sizeof(conversion), std::string(result, position, 1).c_str());
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

        if (_mbsstr(reinterpret_cast<const unsigned char*>(conversion), reinterpret_cast<const unsigned char*>("s"))) {
          if (arguments[index].find("MsgStr(") != std::string::npos) {
            arguments[index] = arguments[index].substr(7);
            arguments[index] = arguments[index].substr(0, arguments[index].size() - 1);
            const std::string nested = LookupMessage(std::atoi(arguments[index].c_str()));
            std::sprintf(buffer, format.c_str(), nested.c_str());
          } else {
            std::sprintf(buffer, format.c_str(), arguments[index].c_str());
          }
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>(conversion), reinterpret_cast<const unsigned char*>("d"))) {
          std::sprintf(buffer, format.c_str(), std::atoi(arguments[index].c_str()));
        } else if (_mbsstr(reinterpret_cast<const unsigned char*>(conversion), reinterpret_cast<const unsigned char*>("f"))) {
          std::sprintf(buffer, format.c_str(), std::atof(arguments[index].c_str()));
        }

        result = buffer + remainder;
      }
    }
  }

  return result;
}

// OFFSET: 0x0083B250
// FUNCTION: fn_10473
void LineCutter::UIText::OnLBtnDown(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnDown(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x0083B220
// FUNCTION: fn_10472
void LineCutter::UIText::OnLBtnDblClk(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnDblClk(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x0083B400
// FUNCTION: fn_10476
void LineCutter::UIText::OnMouseMove(int x, int y) {
  if (this->parent) {
    this->parent->OnMouseMove(x + this->geometry.x, y + this->geometry.y);
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
void LineCutter::UIText::OnLBtnUp(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnUp(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x0083B630
// FUNCTION: fn_10480
void LineCutter::UIText::OnRBtnDown(int x, int y) {
  if (this->parent) {
    this->parent->OnRBtnDown(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x0083B660
// FUNCTION: fn_10481
void LineCutter::UIText::OnRBtnUp(int x, int y) {
  if (this->parent) {
    this->parent->OnRBtnUp(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x0083B690
// FUNCTION: fn_10482
void LineCutter::UIText::OnWheel(int value) {
  if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x0083B150
// FUNCTION: fn_10471
void LineCutter::UIText::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (this->m_shadowed) {
    this->DrawTextAt(1, 1, this->m_text.c_str(), this->m_text.size(), 0, this->m_style.fontHeight, this->m_shadowColor, this->m_style.bold,
      this->m_style.italic
    );
  }

  if (this->m_outlined == 1) {
    this->DrawOutlinedText(
      1, 1, this->m_text.c_str(), this->m_text.size(), this->m_style.color, this->m_outlineColor, 0, this->m_style.fontHeight, this->m_style.bold, this->m_style.italic
    );
  } else {
    this->DrawTextAt(0, 0, this->m_text.c_str(), this->m_text.size(), 0, this->m_style.fontHeight, this->m_style.color, this->m_style.bold, this->m_style.italic
    );
  }
}

// OFFSET: 0x0083B460
// FUNCTION: fn_10478
void UIEmotion::OnProcess() {
  if (!this->m_action || 0.0f == this->m_action->Speed(this->m_animation)) {
    return;
  }

  const unsigned frames = static_cast<unsigned>(static_cast<float>(timeGetTime() - this->m_startTime) / (this->m_action->Speed(this->m_animation) * 24.0f));

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
  g_session.IconPath(path, this->m_itemId.c_str(), 1);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));

  if (bitmap) {
    this->DrawBitmap(0, 0, bitmap, true);
  }
}

// OFFSET: 0x00836970
// FUNCTION: fn_10433
CCreateLineInfo::CCreateLineInfo(
  UIWindow* parent, const char* text, int width, int lineSpacing, std::uint32_t color, int fontHeight, LineFontFlags flags, bool outlineEnabled,
  int outlineColorValue, bool shadow, int shadowColorValue, unsigned char align
)
    : m_align(align), m_newSegment(true), m_height(0) {
  if (parent && (this->m_parent = parent, text)) {
    this->m_text.assign(text, std::strlen(text));
    this->m_width = width;
    this->m_lineSpacing = lineSpacing;
    this->m_defaultColor = color;
    this->m_color = color;
    this->m_defaultFontHeight = fontHeight;
    this->m_fontHeight = fontHeight;
    this->m_bold = flags.bold;
    this->m_italic = flags.italic;
    this->shadow = shadow;
    this->shadowColor = shadowColorValue;
    this->outline = outlineEnabled;
    this->outlineColor = outlineColorValue;

    if (!s_fontStackCache.empty()) {
      this->m_fontStack.swap(s_fontStackCache);
    }

    this->Rebuild(0);
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

// OFFSET: 0x00837BC0
// FUNCTION: fn_10451
int CCreateLineInfo::LayoutEmotion(int x, const std::string& text) {
  int group = 0;
  int id = -1;
  std::cmatch match;

  if (std::regex_search(text.c_str(), text.c_str() + text.size(), match, std::regex("^\\^e\\[(\\d{1,3})\\](\\[\\d\\])*"))) {
    id = std::stoi(match[1].str());
  } else {
    if (!std::regex_search(text.c_str(), text.c_str() + text.size(), match, std::regex("^\\^e\\[(\\d{1,3}\\s*,\\s*\\d{1,3})\\](\\[\\d\\])*"))) {
      return x;
    }

    const std::vector<std::string> parts = Split(match[1].str(), ",");
    if (parts.size() == 2) {
      group = std::stoi(parts[0]);
      id = std::stoi(parts[1]);
    }
  }

  if (id >= 0) {
    int loops = 0;
    if (match[2].matched == true) {
      const std::string tail = match[2].str();
      std::cmatch loopMatch;
      if (std::regex_search(tail.c_str(), tail.c_str() + tail.size(), loopMatch, std::regex("^\\[(\\d)\\]"))) {
        loops = std::atoi(loopMatch[1].str().c_str());
      }
    }

    CashEmotionData* data = g_cashEmotionMgr->GetEmotionData(group, id);
    if (data) {
      const char* sprite = g_cashEmotionMgr->GetSprFileName(data->actId);
      if (sprite) {
        const char* action = g_cashEmotionMgr->GetPackName(data->sprId);
        if (action) {
          UIEmotion* emotion = new UIEmotion(sprite, action);
          emotion->SetSize(50, 50);
          emotion->m_animation = data->actionIndex;
          emotion->Invalidate();
          emotion->m_maxLoops = loops;
          emotion->m_loops = 1;
          if (this->m_width < emotion->geometry.width + x) {
            this->NewLine();
            this->AddWindow(emotion);
            x = emotion->geometry.width;
          } else {
            this->AddWindow(emotion);
            x += emotion->geometry.width;
          }
        }
      }
    }
  }

  return x;
}

// OFFSET: 0x00838130
// FUNCTION: fn_10452
int CCreateLineInfo::LayoutLink(int x, std::string text, int type, LineTextStyle style) {
  const std::tuple<bool, std::string, std::string> info = SplitLinkInfo(text, type);
  if (!std::get<0>(info)) {
    return x;
  }

  std::string display = std::get<1>(info);
  if (this->m_parent) {
    display = RemoveTextColors(display.c_str());
  }

  const std::string linkData = std::get<2>(info);
  const std::vector<std::string> parts = SplitString(linkData.c_str(), ",");
  const int width = MeasureText(this->m_parent, display, style).cx;
  if (width > this->m_width) {
    const std::vector<std::string> lines = SplitLines(this->m_parent, display, style, this->m_width);
    for (auto line = lines.begin(); line != lines.end(); ++line) {
      this->NewLine();
      x = 0;
      UITextButton* button = this->CreateLinkWindow(*line, parts, type, style);
      if (button) {
        this->AddWindow(button);
        x = button->geometry.width;
      }
    }
  } else {
    if (x + width > this->m_width) {
      this->NewLine();
      x = 0;
    }

    if (!display.empty() && !parts.empty()) {
      UITextButton* button = this->CreateLinkWindow(display, parts, type, style);
      this->AddWindow(button);
      x += button->geometry.width;
    }
  }

  return x;
}

// OFFSET: 0x00838780
// FUNCTION: fn_10453
int CCreateLineInfo::LayoutItemImage(int x, const std::string& text) {
  std::cmatch match;

  if (!std::regex_search(text.c_str(), text.c_str() + text.size(), match, std::regex("^\\^i\\[(\\d{1,7})\\]*"))) {
    return x;
  }

  UIItemImage* image;
  {
    const std::string itemId = match[1].str();
    image = new (std::nothrow) UIItemImage();
    if (!image) {
      return x;
    }
    image->SetSize(25, 25);
    image->SetItemId(itemId);
  }

  if (image->geometry.width + x > this->m_width) {
    this->NewLine();
    this->AddWindow(image);
    x = image->geometry.width;
  } else {
    this->AddWindow(image);
    x += image->geometry.width;
  }

  return x;
}

// OFFSET: 0x008389A0
// FUNCTION: fn_10454
int CCreateLineInfo::LayoutText(int x, std::string text, LineTextStyle style) {
  const int width = MeasureText(this->m_parent, text, style).cx;
  if (width > this->m_width) {
    const std::string first = FitText(this->m_parent, text, style, this->m_width - x);
    UIWindow* last = this->CreateTextWindow(first, style);
    if (last) {
      this->AddWindow(last);
    }

    const std::string rest = text.substr(first.size(), text.size() - first.size());
    const std::vector<std::string> lines = SplitLines(this->m_parent, rest, style, this->m_width);
    if (!lines.empty()) {
      x = 0;
    }

    for (auto line = lines.begin(); line != lines.end(); ++line) {
      this->NewLine();
      last = this->CreateTextWindow(*line, style);
      if (last) {
        this->AddWindow(last);
      }
    }

    if (last) {
      x += last->geometry.width;
    }
  } else {
    if (this->m_width < x + width) {
      this->NewLine();
      x = 0;
    }

    UIWindow* window = this->CreateTextWindow(text, style);
    if (window) {
      this->AddWindow(window);
      x += window->geometry.width;
    }
  }

  return x;
}

// OFFSET: 0x00838D40
// FUNCTION: fn_10455
void CCreateLineInfo::BuildWindows() {
  int x = 0;

  for (auto segment = this->m_segments.begin(); segment != this->m_segments.end(); ++segment) {
    const std::string text = segment->text;
    switch (segment->type) {
    case LINE_SEGMENT_URL:
    case LINE_SEGMENT_NAVI:
    case LINE_SEGMENT_ITEM:
    case LINE_SEGMENT_TIPBOX:
    case LINE_SEGMENT_QUEST:
      x = this->LayoutLink(x, text, segment->type, segment->style);
      break;
    case LINE_SEGMENT_MESSAGE: {
      const std::string expanded = GetNavigationLinkParser()->ExpandMessages(text);
      x = this->LayoutText(x, expanded, segment->style);
      break;
    }
    case LINE_SEGMENT_EMOTION:
      x = this->LayoutEmotion(x, text);
      break;
    case LINE_SEGMENT_ITEM_IMAGE:
      x = this->LayoutItemImage(x, text);
      break;
    case LINE_SEGMENT_TEXT:
      if (text.compare("\n") == 0) {
        this->NewLine();
        x = 0;
      } else {
        x = this->LayoutText(x, text, segment->style);
      }
      break;
    }
  }
}

// OFFSET: 0x008392C0
// FUNCTION: fn_10456
void CCreateLineInfo::Rebuild(int) {
  DecodeTextEscapes(this->m_text);
  this->ParseText();
  this->BuildWindows();
  this->ArrangeLines();
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
// FUNCTION: fn_10458
std::vector<std::string> CCreateLineInfo::SplitLines(UIWindow* window, std::string text, const LineTextStyle& style, int width) {
  std::vector<std::string> lines;
  const int length = static_cast<int>(text.size());
  int position = 0;

  while (position < length) {
    const std::string rest = text.substr(position, length - position);
    if (MeasureText(window, rest, style).cx + 1 > width) {
      const std::string line = FitText(window, rest, style, width);
      lines.push_back(line);
      position += static_cast<int>(line.size());
    } else {
      lines.push_back(text.substr(position, length - position));
      break;
    }
  }

  return lines;
}

// OFFSET: 0x00839860
// FUNCTION: fn_10459
std::string CCreateLineInfo::FitText(UIWindow* window, std::string text, const LineTextStyle& style, int width) {
  const int length = static_cast<int>(text.size());
  int position = 0;
  int total = 0;

  while (position < length) {
    const std::string rest = text.substr(position, length - position);
    const std::string character = NextCharacter(rest);
    if (character.empty()) {
      continue;
    }

    total += MeasureText(window, character, style).cx;
    if (total > width) {
      return text.substr(0, position);
    }
    position += static_cast<int>(character.size());
  }

  return text;
}

// OFFSET: 0x00839D60
// FUNCTION: fn_10460
std::pair<std::string, std::string> CCreateLineInfo::GetLinkTags(int type) {
  switch (type) {
  case LINE_SEGMENT_URL:
    return std::make_pair(g_urlTag, g_urlCloseTag);
  case LINE_SEGMENT_ITEM:
    return std::make_pair(g_itemTag, g_itemCloseTag);
  case LINE_SEGMENT_ITEM_LINK:
    return std::make_pair(g_itemLinkTag, g_itemLinkCloseTag);
  case LINE_SEGMENT_NAVI:
    return std::make_pair(g_naviTag, g_naviCloseTag);
  case LINE_SEGMENT_NAVI_LINK:
    return std::make_pair(g_naviLinkTag, g_naviLinkCloseTag);
  case LINE_SEGMENT_MESSAGE:
    return std::make_pair(g_messageTag, g_messageCloseTag);
  case LINE_SEGMENT_QUEST:
    return std::make_pair(g_questTag, g_questCloseTag);
  case LINE_SEGMENT_TIPBOX:
    return std::make_pair(g_tipBoxTag, g_tipBoxCloseTag);
  default:
    return std::make_pair("", "");
  }
}

// OFFSET: 0x00839FA0
// FUNCTION: fn_10461
tagSIZE CCreateLineInfo::MeasureText(UIWindow* window, std::string text, const LineTextStyle& style) {
  if (!window) {
    return tagSIZE();
  }

  return window->TextSize(text.c_str(), text.size(), 0, style.fontHeight, style.bold, style.italic);
}

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

// OFFSET: 0x0083A5F0
// FUNCTION: fn_10463
UITextButton* CCreateLineInfo::CreateLinkWindow(const std::string& text, const std::vector<std::string>& parts, int type, LineTextStyle style) {
  if (text.empty() || parts.empty()) {
    return nullptr;
  }

  const int width = MeasureText(this->m_parent, text, style).cx;
  const std::uint32_t color = type == LINE_SEGMENT_URL ? 0xE6A300 : 0xF4934A;
  const std::uint32_t hoverColor = type == LINE_SEGMENT_URL ? 0xEFB700 : 0xFF7864;
  UITextButton* button = new UITextButton();
  button->Configure(text.c_str(), color, 0, 12, 0, 1);
  button->m_hoverColor = hoverColor;
  button->SetId(type);
  button->ResizeToTextClipped(width + 1, button->geometry.height);

  for (auto it = parts.begin(); it != parts.end(); ++it) {
    std::string part = *it;
    button->m_lines.push_back(part);
  }

  return button;
}

// OFFSET: 0x0083ACF0
// FUNCTION: fn_10467
UIWindow* CCreateLineInfo::CreateTextWindow(std::string text, const LineTextStyle& style) {
  if (text.empty()) {
    return nullptr;
  }

  const tagSIZE size = MeasureText(this->m_parent, text, style);
  LineCutter::UIText* window = new LineCutter::UIText();
  window->SetSize(size.cx, size.cy);
  window->SetText(text, style, this->outline, this->outlineColor, this->shadow, this->shadowColor);
  return window;
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
    const int next = this->ParseTag(index);
    index = index == next ? this->ParseChar(index) : next;
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
  item.identity[0] = static_cast<std::uint32_t>(index);
  item.itemId = itemId;
  item.identified = 1;
  UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
  if (window) {
    window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
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
  UITipboxWnd* window = static_cast<UITipboxWnd*>(g_windowMgr.EnsureWindow(WID_TIPBOXWND));
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
    return index + static_cast<int>(match[0].length());
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
      item.GetIdDisplayName(name, capacity, 0);
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

  if (character.compare("\n") == 0) {
    this->AddSegment(LINE_SEGMENT_TEXT);
    this->AppendText(character);
    this->m_newSegment = true;
  } else if (character.compare("\r") != 0) {
    if (character.compare(" ") == 0) {
      if (index > 0) {
        const int previousIndex = index - 1;
        const std::string previousText = this->m_text.substr(previousIndex, this->m_text.size() - previousIndex);
        const std::string previous = NextCharacter(previousText);
        if (previous.compare("\n") == 0) {
          this->AddSegment(LINE_SEGMENT_TEXT);
        }
        this->AppendText(character);
      } else {
        this->AppendText(character);
      }
      this->m_newSegment = true;
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

// INLINED
static int ParseColorCode(CCreateLineInfo& info, int index) {
  const std::string code = info.m_text.substr(index + 1, 6);
  const char* digits = code.c_str();
  if (!digits) {
    return index;
  }

  char* end = nullptr;
  const unsigned long value = std::strtol(digits, &end, 16);
  if (end <= digits || end - digits != 6) {
    return index;
  }

  const std::uint32_t color = RGB((value >> 16) & 0xFF, (value >> 8) & 0xFF, value & 0xFF);
  info.m_newSegment = true;
  info.m_defaultColor = color;
  info.m_color = color;
  return index + 7;
}

// OFFSET: 0x0083D090
// FUNCTION: fn_10495
int CCreateLineInfo::ParseTag(int index) {
  int next = index;
  if (this->m_text.at(index) == '^') {
    next = ParseColorCode(*this, index);
  }

  if (index != next) {
    return next;
  }
  next = this->ParseItemName(next);
  if (index != next) {
    return next;
  }
  next = this->ParseEmotion(next);
  if (index != next) {
    return next;
  }
  next = this->ParseItemImage(next);
  if (index != next) {
    return next;
  }
  next = this->ParseLink(next);
  if (index != next) {
    return next;
  }
  next = this->ParseMarkup(next);
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
      const unsigned height = static_cast<unsigned>(window->geometry.height);
      if (lineHeight < height) {
        lineHeight = height;
      }
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
      int x = 0;
      for (UIWindow* window : line) {
        x += window->geometry.width;
      }
      x = static_cast<unsigned>(this->m_width - x) / 2;
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

  if (text.find(close, begin) == std::string::npos) {
    return std::tuple<bool, std::string, std::string>(false, "", "");
  }

  const std::size_t infoBegin = text.find(g_infoTag, begin);
  if (infoBegin == std::string::npos) {
    return std::tuple<bool, std::string, std::string>(false, "", "");
  }

  const std::size_t infoEnd = text.find(g_infoCloseTag, infoBegin);
  if (infoEnd == std::string::npos) {
    return std::tuple<bool, std::string, std::string>(false, "", "");
  }

  const std::string name = text.substr(begin + open.size(), infoBegin - open.size() - begin);
  const std::string info = text.substr(infoBegin + g_infoTag.size(), infoEnd - g_infoTag.size() - infoBegin);
  return std::tuple<bool, std::string, std::string>(true, name, info);
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

// OFFSET: 0x0083AF50
// FUNCTION: fn_10469
void UIEmotion::Paint() {
  if (!this->geometry.surface) {
    return;
  }

  this->ClearSurface(0xFFFF00FF);

  if (!this->m_sprite || !this->m_action) {
    return;
  }

  const ActionFrame* frame = &this->m_action->Frame(this->m_animation, this->m_frame);
  if (!frame) {
    return;
  }

  const std::uint32_t* palette = this->m_sprite->palette;
  if (!palette) {
    return;
  }

  for (unsigned index = 0; index < frame->cells.size(); ++index) {
    const SpriteFrameCell& cell = frame->Cell(index);
    if (cell.image == -1 || cell.imageType >= 2) {
      continue;
    }

    const std::vector<SpriteRleImage*>& images = this->m_sprite->images[cell.imageType];
    if (images.size() > static_cast<unsigned>(cell.image)) {
      SpriteRleImage* image = images[cell.image];
      if (image) {
        this->AddRotatedSpriteCellOverlay(
          static_cast<int>(this->geometry.width * 0.5), static_cast<int>(this->geometry.height * 0.5), reinterpret_cast<int>(this->m_action),
          reinterpret_cast<int>(this->m_sprite), image, &cell, palette
        );
      }
    }
  }
}

// OFFSET: 0x0083B2B0
// FUNCTION: fn_10475
void UIItemImage::OnMouseHover(int, int) {
  ITEM_INFO item;
  item.itemId = this->m_itemId;
  item.identified = 1;
  int x = 0;
  int y = -17;
  this->ToScreen(x, y);

  if (g_modeMgr.gameMode) {
    CMode* tooltip = g_modeMgr.gameMode;
    const std::uint32_t color = g_session.GetItemNameColor(item);
    tooltip->ShowTooltip(item.DisplayName(0).c_str(), x, y, color, 0, 1);
  }
}

// OFFSET: 0x0083B550
// FUNCTION: fn_10479
void UIItemImage::OnRBtnDown(int, int) {
  ITEM_INFO item;
  item.itemId = this->m_itemId;
  item.identified = 1;
  UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
  if (window) {
    window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x00832000
// FUNCTION: fn_10406
bool NavigationLinkParser::HasUnownedItemLink(std::string text) {
  if (!StrStrIA(text.c_str(), g_itemLinkTag.c_str())) {
    return false;
  }

  std::string work = text;
  int offset = 0;

  for (;;) {
    std::string link;
    if (!ExtractInnerItemLink(work, link, offset, text.c_str())) {
      return false;
    }

    const int itemId = ItemLinkMessageId(link);
    if (link.substr(0, 6) == "00000T" && itemId <= g_itemTagOwner->maxTipId) {
      offset += static_cast<int>(link.size() + g_itemLinkTag.size() + g_itemLinkCloseTag.size());
      continue;
    }

    const bool unowned = !(LookupClientItem(g_session.m_inventoryItems, itemId, 0).identity[1] > 0 || g_session.HasItemId(itemId) ||
                           g_session.HasItem1740Number(itemId));
    if (!unowned) {
      offset += static_cast<int>(link.size() + g_itemLinkTag.size() + g_itemLinkCloseTag.size());
      continue;
    }

    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_IQ_FIRM_FAITH_ON)), 0xFF, 0, 0, 0);
    return true;
  }
}

// OFFSET: 0x00836D40
// FUNCTION: fn_10435
UIEmotion::UIEmotion(const char* sprite, const char* action) : UIWindow(0), m_animation(0), m_frame(0), m_maxLoops(0) {
  this->m_sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(sprite));
  this->m_action = static_cast<CActRes*>(ResourceMgr::Instance().Load(action));
  this->m_startTime = timeGetTime();
}

// OFFSET: 0x00836DF0
// FUNCTION: fn_10436
UIEmotion::UIEmotion() : UIWindow(0), m_animation(0), m_frame(0), m_maxLoops(0) {
  this->m_sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("이팩트\\emotion.spr"));
  this->m_action = static_cast<CActRes*>(ResourceMgr::Instance().Load("이팩트\\emotion.act"));
  this->m_startTime = timeGetTime();
}

// OFFSET: 0x00836EA0
// FUNCTION: fn_10437
UIItemImage::UIItemImage() : UIWindow(0), m_itemId("") {
  this->m_clip = 1;
}

// OFFSET: 0x0083B7F0
// FUNCTION: fn_10484
void OpenTextButtonNavigation(UITextButton* button) {
  if (!button) {
    return;
  }

  if (button->m_lines.size() == 0) {
    return;
  }

  const std::string name = button->GetLine(0);
  const int x = std::atoi(button->GetLine(1).c_str());
  const int y = std::atoi(button->GetLine(2).c_str());
  if (button->m_lines.size() <= 3) {
    g_navigationManager.StartNavigation(name, 2, 5, false, x, y, 1002);
    return;
  }

  const int mode = std::atoi(button->GetLine(3).c_str());
  int code = std::atoi(button->GetLine(4).c_str());
  if (code == -222) {
    std::string tag = "$$DB ";
    if (mode != 3) {
      tag.clear();
    }

    tag += name;
    g_navigationManager.SearchNavigation(tag, mode, false);
    return;
  }

  const int flag = std::atoi(button->GetLine(5).c_str());
  const int id = std::atoi(button->GetLine(6).c_str());
  unsigned char hundreds = 0;
  if (code >= 100) {
    hundreds = 1;
    code -= 100;
  }

  const unsigned char group = code < 10 ? hundreds : static_cast<unsigned char>(hundreds + 2);
  const int remainder = code < 10 ? code : code - 10;
  const unsigned char kind = remainder < 1 ? group : static_cast<unsigned char>(group + 4);
  g_navigationManager.StartNavigation(name, mode, kind, flag == 0, x, y, id);
}

// OFFSET: 0x0083BAF0
// FUNCTION: fn_10485
void RequestTextButtonService(UITextButton* button) {
  if (!button) {
    return;
  }

  if (button->m_lines.size() == 0) {
    return;
  }

  const int value = std::atoi(button->GetLine(0).c_str());
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ReceiveServiceRequest(6, value);
}
