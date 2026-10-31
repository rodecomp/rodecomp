#ifndef RAGNAROK_SOURCE_UI_WINDOW_UITAB_H
#define RAGNAROK_SOURCE_UI_WINDOW_UITAB_H

#include "UI/Window/UIWindow.h"

class UITab : public UIWindow {
public:
  explicit UITab(UIWindowContext& context) : UIWindow(context, 0), tabCount(3), style(0) {
    this->geometry.unknown_0030 = 0;
  }
  void Paint() override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ int tabCount;
  /* 0080 */ int style;
};

#endif
