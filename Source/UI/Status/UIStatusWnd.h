#ifndef RAGNAROK_SOURCE_UI_STATUS_UISTATUSWND_H
#define RAGNAROK_SOURCE_UI_STATUS_UISTATUSWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIStatusWnd : public UIFrameWnd {
public:
  /* 00B4 */ unsigned char unknown_00B4[0x20];
  /* 00D4 */ int m_titleHeight;
  /* 00D8 */ int m_offsetY;
  /* 00DC */ int m_offsetX;
  /* 00E0 */ unsigned char unknown_00E0[0x1C];
  /* 00FC */ unsigned char m_extended;

  bool Slot03() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;

protected:
  UIButtonContext* m_controls;
};

#endif
