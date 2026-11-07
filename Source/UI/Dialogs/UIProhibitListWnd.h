#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIPROHIBITLISTWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIPROHIBITLISTWND_H

#include "UI/Window/UIFrameWnd.h"

class UIProhibitListWnd : public UIFrameWnd {
public:
  UIProhibitListWnd();
  /* 00B4 */ int m_count;
  /* 00B8 */ std::uint32_t m_aid;

  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnProcess() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

#endif
