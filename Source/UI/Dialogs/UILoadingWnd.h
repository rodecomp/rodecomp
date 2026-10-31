#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UILOADINGWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UILOADINGWND_H

#include "UI/UIBarGraph.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UILoadingWnd : public UIFrameWnd {
public:
  UILoadingWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B4 */ unsigned char unknown_00B4[0x4];
  /* 00B8 */ UIBarGraph* m_progressBar;
};

#endif
