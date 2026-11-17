#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UINAVILINKBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UINAVILINKBUTTON_H

#include "UI/Buttons/UITextButton.h"

class UINaviLinkButton : public UITextButton {
public:
  UINaviLinkButton();
  ~UINaviLinkButton() override;
  void OnLBtnDown(int x, int y) override;
  void OpenNavigation();
  std::string Link();
  /* 00F8 */ std::string link;
  /* 0110 */ int column;
  /* 0114 */ int line;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UINaviLinkButton) == 0x118);
#endif

// OFFSET: 0x00858B80
// FUNCTION: fn_10764
inline std::string UINaviLinkButton::Link() {
  return this->link;
}

#endif
