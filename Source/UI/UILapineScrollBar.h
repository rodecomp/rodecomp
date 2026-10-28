#ifndef RAGNAROK_SOURCE_UI_UILAPINESCROLLBAR_H
#define RAGNAROK_SOURCE_UI_UILAPINESCROLLBAR_H

#include "UI/Lists/UIScrollBar.h"

class UILapineScrollBar : public UIScrollBar {
public:
  void Paint() override;
};

class UIScrollWnd : public UIWindow {
};

class UILapineBox_IngredientListWnd : public UIScrollWnd {
public:
  void Paint() override;
};

#endif
