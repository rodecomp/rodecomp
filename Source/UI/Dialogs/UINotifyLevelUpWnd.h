#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UINOTIFYLEVELUPWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UINOTIFYLEVELUPWND_H

#include "UI/Window/UIFrameWnd.h"


class UINotifyLevelUpWnd : public UIFrameWnd {
public:
  UINotifyLevelUpWnd(UIWindowContext& windows);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void Slot15(int width, int height) override;

protected:
  void* unknown_controls;
};

#endif
