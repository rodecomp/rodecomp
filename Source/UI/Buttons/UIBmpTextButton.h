#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIBMPTEXTBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIBMPTEXTBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

#include <cstddef>

struct BitmapButtonTextStyle {
  /* 0000 */ int state;
  /* 0004 */ int x;
  /* 0008 */ int y;
  /* 000C */ int height;
  /* 0010 */ unsigned char bold;
  /* 0014 */ std::uint32_t color;
  /* 0018 */ unsigned char shadow;
};

class UIBmpTextButton : public UIBitmapButton {
public:
  UIBmpTextButton(UIWindowContext& windows, UIButtonContext& controls);
  ~UIBmpTextButton() override;
  void Paint() override;
  void SetText(std::string value);

  /* 0120 */ std::string text;
  /* 0138 */ std::vector<BitmapButtonTextStyle> styles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BitmapButtonTextStyle) == 28);
static_assert(offsetof(BitmapButtonTextStyle, state) == 0x0000);
static_assert(offsetof(BitmapButtonTextStyle, x) == 0x0004);
static_assert(offsetof(BitmapButtonTextStyle, y) == 0x0008);
static_assert(offsetof(BitmapButtonTextStyle, height) == 0x000C);
static_assert(offsetof(BitmapButtonTextStyle, bold) == 0x0010);
static_assert(offsetof(BitmapButtonTextStyle, color) == 0x0014);
static_assert(offsetof(BitmapButtonTextStyle, shadow) == 0x0018);
static_assert(sizeof(UIBmpTextButton) == 332);
#endif

#endif
