#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETRESULTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIMAKETARGETRESULTWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIMakeTargetResultWnd : public UIFrameWnd {
public:
  UIMakeTargetResultWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  UIButtonContext* m_controls;

public:
  /* 00B4 */ int makeType;
  /* 00B8 */ bool succeeded;
};

#endif
