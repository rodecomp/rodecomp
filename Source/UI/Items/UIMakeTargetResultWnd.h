#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETRESULTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETRESULTWND_H

#include "UI/Window/UIFrameWnd.h"

class UIMakeTargetResultWnd : public UIFrameWnd {
public:
  UIMakeTargetResultWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:

public:
  /* 00B4 */ int makeType;
  /* 00B8 */ bool succeeded;
};

#endif
