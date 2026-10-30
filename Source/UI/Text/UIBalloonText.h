#ifndef RAGNAROK_SOURCE_UI_TEXT_UIBALLOONTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_UIBALLOONTEXT_H

#include "UI/Window/UIWindow.h"

#include <string>
#include <vector>

class FontCache;
struct TextLocale;
struct ItemDescriptions;

struct BalloonTextContext {
  FontCache& fonts;
  const TextLocale& locale;
  const unsigned short& codePage;
  ItemDescriptions& items;
};

class UIBalloonText : public UIWindow {
public:
  UIBalloonText(UIWindowContext& windows, BalloonTextContext& text);
  ~UIBalloonText() override;

  bool CanHitTest() override;
  void Slot19() override;
  void Paint() override;

  void SetText(const char* text, int width);
  void WrapText(const char* text, int width);
  void FitText();
  void SetFontHeight(int height);

protected:
  /* 007C */ unsigned char unknown_007C;
  /* 0080 */ int m_fontHeight;
  /* 0084 */ std::vector<std::string> m_lines;
  /* 0090 */ std::uint32_t m_textColor;
  /* 0094 */ std::uint32_t m_outlineColor;
  /* 0098 */ std::uint16_t unknown_0098;
  /* 009A */ std::uint16_t m_fontMode;
  /* 009C */ unsigned char m_richText;
  BalloonTextContext* m_textContext;
};

extern int g_nameBalloonFontHeight;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIBalloonText) == 168);
#endif

#endif
