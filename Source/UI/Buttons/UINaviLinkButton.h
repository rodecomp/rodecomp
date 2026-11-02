#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UINAVILINKBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UINAVILINKBUTTON_H

#include "UI/Buttons/UITextButton.h"

class UINaviLinkButton : public UITextButton {
public:
  UINaviLinkButton(UIWindowContext& windows);
  ~UINaviLinkButton() override;
  void Slot25(int x, int y) override;
  std::string Link();
  /* 00F8 */ std::string link;
  /* 0110 */ int column;
  /* 0114 */ int line;

  void* unknown_0118;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UINaviLinkButton) == 292);
#endif

#endif
