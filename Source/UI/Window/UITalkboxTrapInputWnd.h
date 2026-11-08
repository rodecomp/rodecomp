#ifndef RAGNAROK_SOURCE_UI_WINDOW_UITALKBOXTRAPINPUTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UITALKBOXTRAPINPUTWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;

struct TalkboxTrapTarget {
  /* 0000 */ int skillId;
  /* 0004 */ int cellX;
  /* 0008 */ int cellY;
  /* 000C */ int skillLevel;
};

struct TalkboxTrapRequest {
  /* 0000 */ TalkboxTrapTarget target;
  /* 0010 */ const char* text;
};

class UITalkboxTrapInputWnd : public UIFrameWnd {
public:
  UITalkboxTrapInputWnd();

  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ TalkboxTrapTarget target;
  /* 00C4 */ UIEditCtrl* edit;
};

#endif
