#ifndef RAGNAROK_SOURCE_UI_STATUS_UISTATUSWND_H
#define RAGNAROK_SOURCE_UI_STATUS_UISTATUSWND_H

#include "UI/Window/UIFrameWnd.h"

class UIStatusWnd : public UIFrameWnd {
public:
  /* 00B4 */ unsigned char unknown_00B4[0x20];
  /* 00D4 */ int m_titleHeight;

  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
};

#endif
