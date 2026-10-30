#ifndef RAGNAROK_SOURCE_UI_UISLOTBITMAP_H
#define RAGNAROK_SOURCE_UI_UISLOTBITMAP_H

#include "UI/UIStaticBitmap.h"

class UIBitmapButton;

class UISlotBitmap : public UIStaticBitmap {
public:
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot32(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B8 */ unsigned char unknown_00B8;
  /* 00BC */ UIBitmapButton* addButton;
  /* 00C0 */ int slot;
};

#endif
