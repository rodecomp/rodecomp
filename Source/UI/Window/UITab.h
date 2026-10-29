#ifndef RAGNAROK_SOURCE_UI_WINDOW_UITAB_H
#define RAGNAROK_SOURCE_UI_WINDOW_UITAB_H

#include "UI/Window/UIWindow.h"

class UITab : public UIWindow {
public:
  void Paint() override;
  void Slot25(int x, int y) override;

  /* 007C */ int tabCount;
  /* 0080 */ int style;
};

#endif
