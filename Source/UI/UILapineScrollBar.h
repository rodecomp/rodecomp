#ifndef RAGNAROK_SOURCE_UI_UILAPINESCROLLBAR_H
#define RAGNAROK_SOURCE_UI_UILAPINESCROLLBAR_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/UIScrollWnd.h"

#include <cstddef>
#include <vector>

class UILapineUpgradeBox_IngredientInfoWnd;

class UILapineScrollBar : public UIScrollBar {
public:
  void Paint() override;
};

class UILapineUpgradeBox_IngredientListWnd : public UIScrollWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Paint() override;
  void Slot35(int argument) override;
  UILapineUpgradeBox_IngredientInfoWnd* FindSelected();

  /* 00B0 */ std::vector<ITEM_INFO> materials;
  /* 00BC */ std::vector<UILapineUpgradeBox_IngredientInfoWnd*> items;
  /* 00C8 */ int minOptionCount;
  /* 00CC */ int minRefine;
  /* 00D0 */ int maxRefine;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIScrollWnd, scrollBar) == 0x90);
static_assert(offsetof(UIScrollWnd, wheelScroll) == 0xA5);
static_assert(offsetof(UILapineUpgradeBox_IngredientListWnd, materials) == 0xB0);
static_assert(offsetof(UILapineUpgradeBox_IngredientListWnd, items) == 0xBC);
static_assert(offsetof(UILapineUpgradeBox_IngredientListWnd, maxRefine) == 0xD0);
#endif

class UILapineBox_IngredientListWnd : public UIScrollWnd {
public:
  void Paint() override;
};

#endif
