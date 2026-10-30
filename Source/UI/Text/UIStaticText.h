#ifndef RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXT_H

#include "UI/Window/UIWindow.h"

#include <string>

struct UIButtonContext;

class UIStaticText : public UIWindow {
public:
  UIStaticText();
  UIStaticText(UIWindowContext& windows, UIButtonContext& controls);
  ~UIStaticText() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int value) override;
  void SetText(const char* value, int outline);
  void SetColor(int redValue, int greenValue, int blueValue);
  void SetTextFitted(const char* value);
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

protected:
  UIButtonContext* m_controls;
};

class UIStaticText2 : public UIStaticText {
public:
  void Paint() override;
};

class UIStaticText3 : public UIStaticText {
public:
  void Paint() override;

  /* 00D8 */ int textX;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIStaticText) == 224);
#endif

#endif
