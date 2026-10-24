#ifndef RAGNAROK_SOURCE_UI_UIJOBSLOTBUTTON_H
#define RAGNAROK_SOURCE_UI_UIJOBSLOTBUTTON_H

#include "UI/UIBitmapButton.h"

// RTTI UIJobSlotButton. Original x86 size 340;
// inherited source dependency pointers account for the eight-byte increase.
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
