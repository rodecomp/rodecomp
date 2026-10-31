#ifndef RAGNAROK_SOURCE_UI_TEXT_CREATELINEINFO_H
#define RAGNAROK_SOURCE_UI_TEXT_CREATELINEINFO_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

class FontCache;
class UIWindow;
class UITextButton;
struct TextLocale;
struct tagSIZE;

enum LineSegmentType {
  LINE_SEGMENT_URL = 0x1B5,
  LINE_SEGMENT_NAVI = 0x1B6,
  LINE_SEGMENT_ITEM = 0x1D0,
  LINE_SEGMENT_ITEM_LINK = 0x1F5,
  LINE_SEGMENT_TIPBOX = 0x205,
  LINE_SEGMENT_QUEST = 0x21B,
  LINE_SEGMENT_NAVI_LINK = 0x232,
  LINE_SEGMENT_MESSAGE = 0x233,
  LINE_SEGMENT_EMOTION = 0x236,
  LINE_SEGMENT_ITEM_IMAGE = 0x241,
  LINE_SEGMENT_TEXT = 0x258,
};

struct LineTextStyle {
  /* 0000 */ std::uint32_t color;
  /* 0004 */ int fontHeight;
  /* 0008 */ bool bold;
  /* 0009 */ bool italic;
};

struct LineTextSegment {
  // INLINED
  LineTextSegment(const std::string& text, const LineTextStyle& style)
      : text(text), style(style), type(LINE_SEGMENT_TEXT), unknown_0028(0), unknown_002C(0) {
  }

  /* 0000 */ std::string text;
  /* 0018 */ LineTextStyle style;
  /* 0024 */ int type;
  /* 0028 */ int unknown_0028;
  /* 002C */ int unknown_002C;
};

struct LineFontFlags {
  /* 0000 */ bool bold;
  /* 0001 */ bool italic;
};

struct LineFontState {
  /* 0000 */ int fontHeight;
  /* 0004 */ std::uint32_t color;
};

// VTABLE: vt_00879
class CCreateLineInfo {
public:
  static std::vector<LineFontState> s_fontStackCache;

  static bool CompareTag(std::string text, std::string keyword, bool exactMatch, bool caseSensitive);
  static std::size_t FindTag(std::string text, std::string keyword, bool caseSensitive);
  static std::pair<std::string, std::string> GetLinkTags(int type);
  static std::string NextCharacter(std::string text);
  static std::tuple<bool, std::string, std::string> SplitLinkInfo(std::string text, int type);
  static std::vector<std::string> SplitString(std::string text, const std::string& delimiter);
  static tagSIZE MeasureText(UIWindow* window, std::string text, const LineTextStyle& style);
  static std::string FitText(UIWindow* window, std::string text, const LineTextStyle& style, int width);
  static std::vector<std::string> SplitLines(UIWindow* window, std::string text, const LineTextStyle& style, int width);

  /* 0004 */ UIWindow* m_parent;
  /* 0008 */ int m_width;
  /* 000C */ int m_lineSpacing;
  /* 0010 */ int m_bold;
  /* 0014 */ int m_italic;
  /* 0018 */ int m_defaultFontHeight;
  /* 001C */ int m_fontHeight;
  /* 0020 */ std::uint32_t m_defaultColor;
  /* 0024 */ std::uint32_t m_color;
  /* 0028 */ bool unknown_0028;
  /* 002C */ int unknown_002C;
  /* 0030 */ bool unknown_0030;
  /* 0034 */ int unknown_0034;
  /* 0038 */ unsigned char m_align;
  /* 003C */ std::vector<LineFontState> m_fontStack;
  /* 0048 */ bool m_newSegment;
  /* 004C */ std::string m_text;
  /* 0064 */ std::vector<LineTextSegment> m_segments;
  /* 0070 */ std::vector<std::vector<UIWindow*>> m_lines;
  /* 007C */ int m_height;

  CCreateLineInfo(
    UIWindow* parent, const char* text, int width, int lineSpacing, std::uint32_t color, int fontHeight, LineFontFlags flags, bool unknown0028,
    int unknown002C, bool unknown0030, int unknown0034, unsigned char align
  );
  virtual ~CCreateLineInfo();

  void AppendText(const std::string& text);
  void AddWindow(UIWindow* window);
  void ApplyFont(std::string tag);
  int AddLink(int index, int type);
  void NewLine();
  void AddSegment(int type);
  void ParseText();
  int ParseEmotion(int index);
  int ParseMarkup(int index);
  int ParseLink(int index);
  int ParseItemImage(int index);
  int ParseItemName(int index);
  int ParseChar(int index);
  int ParseTag(int index);
  void ArrangeLines();
  void BuildWindows();
  void Rebuild(int unused);
  int LayoutEmotion(int x, const std::string& text);
  int LayoutLink(int x, std::string text, int type, LineTextStyle style);
  int LayoutItemImage(int x, const std::string& text);
  int LayoutText(int x, std::string text, LineTextStyle style);
  UITextButton* CreateLinkWindow(const std::string& text, const std::vector<std::string>& parts, int type, LineTextStyle style);
  UIWindow* CreateTextWindow(std::string text, const LineTextStyle& style);

private:
  void CloseFont(std::string tag);
  void OpenBold(std::string tag);
  void CloseBold(std::string tag);
  void OpenItalic(std::string tag);
  void CloseItalic(std::string tag);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(LineTextStyle) == 0xC);
static_assert(sizeof(LineTextSegment) == 0x30);
static_assert(sizeof(LineFontState) == 0x8);
static_assert(sizeof(CCreateLineInfo) == 0x80);
static_assert(offsetof(CCreateLineInfo, m_fontStack) == 0x3C);
static_assert(offsetof(CCreateLineInfo, m_text) == 0x4C);
static_assert(offsetof(CCreateLineInfo, m_height) == 0x7C);
#endif

#endif
