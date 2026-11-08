#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UISELECTCARTWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UISELECTCARTWND_H

#include "UI/Window/UIFrameWnd.h"

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
  /* 00B8 */ int cartTypesBegin;
  /* 00BC */ int cartTypesEnd;
  /* 00C0 */ int cartTypesCapacity;
};

#endif
