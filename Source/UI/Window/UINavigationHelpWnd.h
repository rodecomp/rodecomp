#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONHELPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONHELPWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UINavigationHelpWnd : public UIFrameWnd {
public:
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  UIButtonContext* m_controls;
};

#endif
