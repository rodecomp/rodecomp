#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIBMPTEXTBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIBMPTEXTBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

struct BitmapButtonTextStyle {
  int state;
  int x;
  int y;
  int height;
  unsigned char bold;
  std::uint32_t color;
  unsigned char shadow;
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
static_assert(sizeof(UIBmpTextButton) == 332);
#endif

#endif
