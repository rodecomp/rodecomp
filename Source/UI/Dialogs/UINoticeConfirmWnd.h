#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UINOTICECONFIRMWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UINOTICECONFIRMWND_H

#include "UI/Window/UIFrameWnd.h"

class UITextViewer;

class UINoticeConfirmWnd : public UIFrameWnd {
public:
  UINoticeConfirmWnd();
  /* 00B4 */ UITextViewer* m_viewer;
  /* 00B8 */ int m_event;

  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void Slot15(int width, int height) override;

protected:
  void* unknown_textContext;
};

#endif
