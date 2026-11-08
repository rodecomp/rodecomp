#ifndef RAGNAROK_SOURCE_UI_UISLOTBITMAP_H
#define RAGNAROK_SOURCE_UI_UISLOTBITMAP_H

#include "UI/UIStaticBitmap.h"

class UIBitmapButton;

class UISlotBitmap : public UIStaticBitmap {
public:
  UISlotBitmap(int slot);
  UISlotBitmap(int slot, unsigned char flag);
  void OnCreate(int width, int height) override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B8 */ unsigned char showAddButton;
  /* 00BC */ UIBitmapButton* addButton;
  /* 00C0 */ int slot;
};

#endif
