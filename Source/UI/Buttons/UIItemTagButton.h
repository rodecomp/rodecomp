#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIITEMTAGBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIITEMTAGBUTTON_H

#include "UI/Buttons/UITextButton.h"

#include <cstddef>

class UIItemTagButton : public UITextButton {
public:
  void Slot28(int x, int y) override;

  /* 00F8 */ unsigned char unknown_00F8[0xF0];
  /* 01F0 */ int column;
  /* 01F4 */ int line;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIItemTagButton, column) == 0x1F0);
static_assert(offsetof(UIItemTagButton, line) == 0x1F4);
#endif

#endif
