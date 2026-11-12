#ifndef RAGNAROK_SOURCE_UI_ITEMS_UISELECTCARTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UISELECTCARTWND_H

#include "UI/Window/UIFrameWnd.h"

#include <vector>

class UISelectCartWnd : public UIFrameWnd {
public:
  UISelectCartWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void AddCart(int index, int type);

protected:

public:
  /* 00B4 */ int selectionContext;
  /* 00B8 */ std::vector<int> cartTypes;
};

#endif
