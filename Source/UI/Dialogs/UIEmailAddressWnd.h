#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIEMAILADDRESSWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIEMAILADDRESSWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct UIEditContext;
class UIEditCtrl;

class UIEmailAddressWnd : public UIFrameWnd {
public:
  UIEmailAddressWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIEditContext& edits);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  UIButtonContext* m_controls;

public:
  /* 00B4 */ UIEditCtrl* m_edit;

protected:
  UIEditContext* m_editContext;
};

#endif
