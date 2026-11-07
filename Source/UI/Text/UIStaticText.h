#ifndef RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXT_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIStaticText : public UIWindow {
public:
  UIStaticText();
  ~UIStaticText() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  void OnWheel(int value) override;
  void SetText(const char* value, int outline);
  void SetColor(int redValue, int greenValue, int blueValue);
  void SetTextFitted(const char* value);
  void SetTextFittedCodePage(const char* value);
  void SetTextAutoWidth(const char* value);
  void SetNumber(int value);
  void SetTextWithinWidth(const char* value, int outlineValue);
  void SetFont(int family, int height, unsigned char bold, unsigned char italic);
  std::string GetText() const;

  /* 007C */ int red;
  /* 0080 */ int green;
  /* 0084 */ int blue;
  /* 0088 */ int shadow;
  /* 008C */ int outline;
  /* 0090 */ std::uint32_t background;
  /* 0094 */ std::uint32_t outlineColor;
  /* 0098 */ int fontHeight;
  /* 009C */ int fontFamily;
  /* 00A0 */ unsigned char bold;
  /* 00A1 */ unsigned char italic;
  /* 00A4 */ int tooltipEnabled;
  /* 00A8 */ std::string text;
  /* 00C0 */ std::string tooltip;
};

class UIStaticText2 : public UIStaticText {
public:
  void Paint() override;
};

class UIStaticText3 : public UIStaticText {
public:
  UIStaticText3();
  void Paint() override;

  /* 00D8 */ int textX;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIStaticText) == 0xD8);
#endif

#endif
