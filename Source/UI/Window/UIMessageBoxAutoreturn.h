#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMESSAGEBOXAUTORETURN_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMESSAGEBOXAUTORETURN_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UITextViewer;
class UIBarGraph;

class UIMessageBoxAutoreturn : public UIFrameWnd {
public:
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UITextViewer* viewer;
  /* 00B8 */ UIBarGraph* gauge;
  /* 00BC */ int counting;
  /* 00C0 */ std::uint32_t started;
  /* 00C4 */ std::uint32_t deadline;
  /* 00C8 */ int seconds;
  /* 00CC */ std::uint32_t remaining;
  /* 00D0 */ std::string format;
  /* 00E8 */ std::string messageText;
};

#endif
