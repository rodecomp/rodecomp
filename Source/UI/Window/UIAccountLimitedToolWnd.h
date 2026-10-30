#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIACCOUNTLIMITEDTOOLWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIACCOUNTLIMITEDTOOLWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIAccountLimitedToolWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
