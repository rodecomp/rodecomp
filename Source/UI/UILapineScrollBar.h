#ifndef RAGNAROK_SOURCE_UI_UILAPINESCROLLBAR_H
#define RAGNAROK_SOURCE_UI_UILAPINESCROLLBAR_H

#include "UI/Lists/UIScrollBar.h"

#include <cstddef>
#include <vector>

class UILapineUpgradeBox_IngredientInfoWnd;

class UILapineScrollBar : public UIScrollBar {
public:
  void Paint() override;
};

class UIScrollWnd : public UIWindow {
public:
  /* 0080 */ unsigned char unknown_0080[0x10];
  /* 0090 */ UIScrollBar* scrollBar;
  /* 0094 */ int scrollBarWidth;
  /* 0098 */ int scrollBarHeight;
  /* 009C */ int unknown_009C;
  /* 00A0 */ int scrollPosition;
  /* 00A4 */ unsigned char unknown_00A4;
  /* 00A5 */ unsigned char wheelScroll;
};

class UILapineUpgradeBox_IngredientListWnd : public UIScrollWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Paint() override;
  void Slot35(int argument) override;

  /* 00A8 */ unsigned char unknown_00A8[0x14];
  /* 00BC */ std::vector<UILapineUpgradeBox_IngredientInfoWnd*> items;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIScrollWnd, scrollBar) == 0x90);
static_assert(offsetof(UIScrollWnd, wheelScroll) == 0xA5);
static_assert(offsetof(UILapineUpgradeBox_IngredientListWnd, items) == 0xBC);
#endif

class UILapineBox_IngredientListWnd : public UIScrollWnd {
public:
  void Paint() override;
};

#endif
