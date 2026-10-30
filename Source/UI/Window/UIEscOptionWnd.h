#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIESCOPTIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIESCOPTIONWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIEscOptionWnd : public UIFrameWnd {
public:
  void Paint() override;

  /* 00B4 */ unsigned char unknown_00B4[0x20];
  /* 00D4 */ int m_titleHeight;

protected:
  UIButtonContext* m_controls;
};

#endif
