#ifndef RAGNAROK_SOURCE_UI_UISLOTBITMAP_H
#define RAGNAROK_SOURCE_UI_UISLOTBITMAP_H

#include "UI/UIStaticBitmap.h"

class UISlotBitmap : public UIStaticBitmap {
public:
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot32(int x, int y) override;

  /* 00B8 */ unsigned char unknown_00B8;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int slot;
};

#endif
