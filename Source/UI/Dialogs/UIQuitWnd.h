#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIQUITWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIQUITWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIQuitWnd : public UIFrameWnd {
public:
  UIQuitWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Slot15(int width, int height) override;

protected:
  UIButtonContext* m_controls;
};

#endif
