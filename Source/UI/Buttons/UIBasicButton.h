#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIBASICBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIBASICBUTTON_H

#include "UI/Buttons/UIButton.h"

class UIBasicButton : public UIButton {
public:
  UIBasicButton(UIWindowContext& windows, UIButtonContext& controls);
  ~UIBasicButton() override;
  void Paint() override;
  bool Configure(int width, const char* value);

  /* 00B4 */ std::string text;
  /* 00CC */ unsigned char invalidateParent;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIBasicButton) == 216);
#endif

#endif
