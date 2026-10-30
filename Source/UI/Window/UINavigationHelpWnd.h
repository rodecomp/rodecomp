#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONHELPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONHELPWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UINavigationHelpWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
