#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIJOBSLOTBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIJOBSLOTBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

class UIJobSlotButton : public UIBitmapButton {
public:
  UIJobSlotButton(UIWindowContext& windows, UIButtonContext& controls);
  ~UIJobSlotButton() override;
  void Slot31(int x, int y) override;
  void SetSelected(unsigned char value);

  /* 0120 */ std::string selectedImage;
  /* 0138 */ std::string unselectedImage;
  /* 0150 */ unsigned char selected;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIJobSlotButton) == 348);
#endif

#endif
