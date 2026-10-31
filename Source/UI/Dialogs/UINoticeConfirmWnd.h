#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UINOTICECONFIRMWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UINOTICECONFIRMWND_H

#include "UI/Window/UIFrameWnd.h"

class UINoticeConfirmWnd : public UIFrameWnd {
public:
  /* 00B4 */ std::uint32_t unknown_00B4;
  /* 00B8 */ int m_event;

  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

#endif
