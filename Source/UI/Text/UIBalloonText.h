#ifndef RAGNAROK_SOURCE_UI_TEXT_UIBALLOONTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_UIBALLOONTEXT_H

#include "UI/Window/UIWindow.h"

#include <string>
#include <vector>

class FontCache;
struct TextLocale;

class UIBalloonText : public UIWindow {
  friend class UIBalloonOpenBtn;
  friend class UIHotKeyWnd;
  friend class CGameActor;
  friend class CGameMode;
  friend class UIBookWnd;
  friend class UISelCharForUServerWnd;

public:
  UIBalloonText();
  ~UIBalloonText() override;

  bool CanHitTest() override;
  void OnProcess() override;
  void Paint() override;

  void SetText(const char* text, int width);
  void WrapText(const char* text, int width);
  void FitText();
  void SetFontHeight(int height);

protected:
  /* 007C */ unsigned char balloonFlag;
  /* 0080 */ int m_fontHeight;
  /* 0084 */ std::vector<std::string> m_lines;
  /* 0090 */ std::uint32_t m_textColor;
  /* 0094 */ std::uint32_t m_outlineColor;
  /* 0098 */ std::uint16_t textFlags;
  /* 009A */ std::uint16_t m_fontMode;
  /* 009C */ unsigned char m_richText;
};

extern int g_nameBalloonFontHeight;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIBalloonText) == 0xA0);
#endif

#endif
