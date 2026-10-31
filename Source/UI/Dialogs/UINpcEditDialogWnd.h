#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UINPCEDITDIALOGWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UINPCEDITDIALOGWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
class UIEditCtrl;

class UINpcEditDialogWnd : public UIFrameWnd {
public:
  UINpcEditDialogWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Paint() override;

protected:
  UIButtonContext* m_controls;

public:
  /* 00B4 */ UIEditCtrl* edit;
};

#endif
