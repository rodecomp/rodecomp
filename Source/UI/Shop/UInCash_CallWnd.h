#ifndef RAGNAROK_SOURCE_UI_SHOP_UINCASH_CALLWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UINCASH_CALLWND_H

#include "UI/Window/UIFrameWnd.h"

struct WindowPlacement;

class UInCashWnd : public UIFrameWnd {
public:
  bool Slot03() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot11() override;

  /* 00B4 */ unsigned char unknown_00B4[0x4C];
  /* 0100 */ WindowPlacement* m_placement;
};

class UInCash_CallWnd : public UIFrameWnd {
public:
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

#endif
