#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIGRAFFISTRBOXWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIGRAFFISTRBOXWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIGraffiStrboxWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
