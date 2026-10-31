#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UINPCEDITDIALOGWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UINPCEDITDIALOGWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
class UIEditCtrl;

struct UIEditContext;

class UINpcEditDialogWnd : public UIFrameWnd {
public:
  UINpcEditDialogWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Paint() override;
  void Slot15(int width, int height) override;

protected:
  UIButtonContext* m_controls;

public:
  /* 00B4 */ UIEditCtrl* edit;
  /* 00B8 */ int unknown_00B8;

protected:
  UIEditContext* m_editContext;
};

class UINpcTextEditDialogWnd : public UINpcEditDialogWnd {
public:
  void Slot15(int width, int height) override;
};

#endif
