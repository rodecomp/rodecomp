#ifndef RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXTCONTROL_H
#define RAGNAROK_SOURCE_UI_TEXT_UISTATICTEXTCONTROL_H

#include "UI/Window/UIControl.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

class CUIVScrollBar;

class CUIStaticText : public CUIControl {
public:
  CUIStaticText();

  void OnMouseHover(int x, int y) override;
  void Paint() override;
  void UpdateText();
  void SetAlign(int horizontal, int vertical);
  void SetFitText(bool enabled);
  void SetBold(unsigned char enabled);
  void SetColor(std::uint32_t color);
  void SetFontHeight(int height);
  void SetFont(const std::string& name);
  void SetOutline(bool enabled, std::uint32_t color);
  void SetShadow(bool enabled, std::uint32_t color);
  __declspec(noinline) void SetText(std::string_view value);
  // INLINED
  void SetText(const char* value, std::size_t length) {
    this->SetText(std::string_view(value, length));
  }
  void SetTooltip(const char* value, std::size_t length);

  std::string& Tooltip() {
    return *reinterpret_cast<std::string*>(this->tooltipStorage);
  }


  /* 0080 */ std::string text;
  /* 0098 */ std::string displayText;
  /* 00B0 */ unsigned char tooltipStorage[0x10];
  /* 00C0 */ CUIVScrollBar* scrollBar;
  /* 00C4 */ unsigned char tooltipTailStorage[0x04];
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
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUIStaticText) == 0xFC);
static_assert(offsetof(CUIStaticText, displayText) == 0x98);
static_assert(offsetof(CUIStaticText, scrollBar) == 0xC0);
static_assert(offsetof(CUIStaticText, color) == 0xE0);
static_assert(offsetof(CUIStaticText, outlineColor) == 0xF0);
#endif

#endif
