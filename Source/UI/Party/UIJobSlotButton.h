#ifndef RAGNAROK_SOURCE_UI_PARTY_UIJOBSLOTBUTTON_H
#define RAGNAROK_SOURCE_UI_PARTY_UIJOBSLOTBUTTON_H

#include "UI/Buttons/UIBitmapButton.h"

class UIJobSlotButton : public UIBitmapButton {
public:
  UIJobSlotButton();
  ~UIJobSlotButton() override;
  void OnLBtnUp(int x, int y) override;
  __declspec(noinline) void SetSelected(bool value);

  /* 0120 */ std::string selectedImage;
  /* 0138 */ std::string unselectedImage;
  /* 0150 */ bool selected;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIJobSlotButton) == 0x154);
#endif

#endif
