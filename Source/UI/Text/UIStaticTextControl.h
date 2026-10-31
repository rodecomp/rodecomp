#ifndef RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXTCONTROL_H
#define RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXTCONTROL_H

#include "UI/Window/UIControl.h"

#include <cstddef>
#include <cstdint>
#include <string>

class CUIVScrollBar;

class CUIStaticText : public CUIControl {
public:
  explicit CUIStaticText(UIWindowContext& context)
      : CUIControl(context), unknown_0084(), unknown_0098(), unknown_00B0(), scrollBar(nullptr), unknown_00C4(), fitText(0), horizontalAlign(0),
        verticalAlign(0), fontIndex(0), fontHeight(12), bold(0), color(0), shadow(0), shadowColor(0xFFFFFF), outline(0), outlineColor(0xFFFFFF), textX(0),
        textY(0), text() {
  }

  void Slot28(int x, int y) override;
  void UpdateText();
  void SetAlign(int horizontal, int vertical);
  void SetFitText(bool enabled);
  void SetBold(unsigned char enabled);
  void SetColor(std::uint32_t color);
  void SetFontHeight(int height);
  void SetFont(const std::string& name);
  void SetOutline(bool enabled, std::uint32_t color);
  void SetShadow(bool enabled, std::uint32_t color);
  __declspec(noinline) void SetText(const char* value, std::size_t length);
  void SetTooltip(const char* value, std::size_t length);

  std::string& Tooltip() {
    return *reinterpret_cast<std::string*>(this->unknown_00B0);
  }

  /* 0084 */ unsigned char unknown_0084[0x14];
  /* 0098 */ std::string unknown_0098;
  /* 00B0 */ unsigned char unknown_00B0[0x10];
  /* 00C0 */ CUIVScrollBar* scrollBar;
  /* 00C4 */ unsigned char unknown_00C4[0x04];
  /* 00C8 */ bool fitText;
  /* 00CC */ int horizontalAlign;
  /* 00D0 */ int verticalAlign;
  /* 00D4 */ int fontIndex;
  /* 00D8 */ int fontHeight;
  /* 00DC */ unsigned char bold;
  /* 00E0 */ std::uint32_t color;
  /* 00E4 */ bool shadow;
  /* 00E8 */ std::uint32_t shadowColor;
  /* 00EC */ bool outline;
  /* 00F0 */ std::uint32_t outlineColor;
  /* 00F4 */ int textX;
  /* 00F8 */ int textY;
  /* 0080 */ std::string text;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUIStaticText) == 0x114);
static_assert(offsetof(CUIStaticText, unknown_0098) == 0x98);
static_assert(offsetof(CUIStaticText, scrollBar) == 0xC0);
static_assert(offsetof(CUIStaticText, color) == 0xE0);
static_assert(offsetof(CUIStaticText, outlineColor) == 0xF0);
#endif

#endif
