#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UINAVILINKBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UINAVILINKBUTTON_H

#include "UI/Buttons/UITextButton.h"

class UINaviLinkButton : public UITextButton {
public:
  UINaviLinkButton();
  ~UINaviLinkButton() override;
  void OnLBtnDown(int x, int y) override;
  std::string Link();
  /* 00F8 */ std::string link;
  /* 0110 */ int column;
  /* 0114 */ int line;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UINaviLinkButton) == 0x118);
#endif

#endif
