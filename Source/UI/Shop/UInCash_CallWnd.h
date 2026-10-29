#ifndef RAGNAROK_SOURCE_UI_SHOP_UINCASH_CALLWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UINCASH_CALLWND_H

#include "UI/Window/UIFrameWnd.h"

class UInCashWnd : public UIFrameWnd {
public:
  bool Slot03() override;
};

class UInCash_CallWnd : public UIFrameWnd {
public:
  void Paint() override;
};

#endif
