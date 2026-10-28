#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIEQUIPWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIEQUIPWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEquipWnd : public UIFrameWnd {
public:
  /* 00B4 */ unsigned char unknown_00B4[0x3C];
  /* 00F0 */ int m_page;
  /* 00F4 */ unsigned char unknown_00F4[0x40];
  /* 0134 */ UIWindow* m_scrollBar;
  /* 0138 */ unsigned char unknown_0138[0x1C];
  /* 0154 */ int m_firstSlot;
  /* 0158 */ int m_hoverSlot;

  void Slot19() override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;
};

#endif
