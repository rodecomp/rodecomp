#ifndef RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXT_LF_H
#define RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXT_LF_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIStaticText_LF : public UIWindow {
public:
  // INLINED
  UIStaticText_LF(UIWindowContext& windows, unsigned char cached)
      : UIWindow(windows, cached), m_lineSpacing(0), m_align(0), m_fontFamily(0), m_fontHeight(12), m_textColor(0), m_bold(0) {}

  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;
  void SetText(std::string text);

protected:
  /* 007C */ std::string m_text;
  /* 0094 */ int m_lineSpacing;
  /* 0098 */ int m_align;
  /* 009C */ int m_fontFamily;
  /* 00A0 */ int m_fontHeight;
  /* 00A4 */ std::uint32_t m_textColor;
  /* 00A8 */ unsigned char m_bold;
};

#endif
