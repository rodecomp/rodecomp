#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIEDITCTRLWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIEDITCTRLWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIEditctrlWnd : public UIFrameWnd {
public:
  void Paint() override;

  /* 00B4 */ UIWindow* edit;

protected:
  UIButtonContext* m_controls;
};

#endif
