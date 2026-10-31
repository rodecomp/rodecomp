#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINOTICEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINOTICEWND_H

#include "UI/Window/UIFrameWnd.h"

class UINoticeWnd : public UIFrameWnd {
public:
  UINoticeWnd(UIWindowContext& windows, UIFrameContext& frames);

  void Slot15(int width, int height) override;
  void Slot18() override;
  void Paint() override;
  void Slot28(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void LoadNotice();

  /* 00B4 */ UIStaticText* m_texts[19];
  /* 0100 */ unsigned char unknown_0100[0x08];
  /* 0108 */ int unknown_0108;
  /* 010C */ int unknown_010C;
  /* 0110 */ const char* m_patchFile;
  /* 0114 */ int m_noticeCount;
  /* 0118 */ int m_index;
  /* 011C */ int m_scrollPositions[10];
  /* 0144 */ int m_bottomPositions[10];
  /* 016C */ int m_scroll;
  /* 0170 */ UIWindow* m_scrollBar;

protected:
  UIButtonContext* m_controls;
};

#endif
