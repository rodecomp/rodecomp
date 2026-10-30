#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIJOBLISTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIJOBLISTWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct JobListWndInfo;

class UIJobListWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Paint() override;

protected:
  UIButtonContext* m_controls;
  /* 00B8 */ unsigned char unknown_00B8[0x34];
  /* 00EC */ JobListWndInfo* m_saved;
};

#endif
