#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIEMAILADDRESSWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIEMAILADDRESSWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIEmailAddressWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
