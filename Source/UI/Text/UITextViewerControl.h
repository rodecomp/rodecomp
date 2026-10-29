#ifndef RAGNAROK_SOURCE_UI_TEXT_UITEXTVIEWERCONTROL_H
#define RAGNAROK_SOURCE_UI_TEXT_UITEXTVIEWERCONTROL_H

#include "UI/Window/UIControl.h"

#include <cstdint>
#include <string>

class CUIPadding;
class CUIVScrollBar;

class CUITextViewer : public CUIControl {
public:
  void Slot19() override;
  void Paint() override;
  void Slot35(int value) override;

  void SetTextOffset(int x, int y);
  void SetBackgroundColor(std::uint32_t value);
  void SetBold(bool value);
  void SetTextColor(std::uint32_t value);
  void SetFontHeight(int value);
  void SetFont(const std::string& name);
  void SetLineSpacing(int value);
  void SetOutline(bool enabled, std::uint32_t value);
  void SetPadding(int left, int top, int right, int bottom);
  void SetScrollStep(int value);
  void SetScrollBarSize(int width, int height);

  /* 0080 */ int textX;
  /* 0084 */ int textY;
  /* 0088 */ int font;
  /* 008C */ int fontHeight;
  /* 0090 */ bool bold;
  /* 0094 */ std::uint32_t textColor;
  /* 0098 */ bool unknown_0098;
  /* 009C */ std::uint32_t unknown_009C;
  /* 00A0 */ bool outline;
  /* 00A4 */ std::uint32_t outlineColor;
  /* 00A8 */ int lineSpacing;
  /* 00AC */ std::uint32_t backgroundColor;
  /* 00B0 */ int paddingLeft;
  /* 00B4 */ int paddingTop;
  /* 00B8 */ int paddingRight;
  /* 00BC */ int paddingBottom;
  /* 00C0 */ CUIVScrollBar* scrollBar;
  /* 00C4 */ CUIPadding* padding;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ unsigned char unknown_00D8[8];
};

#endif
