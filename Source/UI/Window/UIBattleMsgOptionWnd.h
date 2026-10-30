#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIBATTLEMSGOPTIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIBATTLEMSGOPTIONWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UIScrollBar;

class UIBattleMsgOptionWnd : public UIFrameWnd {
public:
  UIBattleMsgOptionWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot35(int delta) override;

  /* 00B4 */ UIScrollBar* m_scrollBar;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int m_visibleRows;
  /* 00C0 */ int m_scrollable;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ std::vector<const char*> messages;
  /* 00D4 */ std::string text;
  /* 00EC */ unsigned char unknown_00EC[0x1C];
  /* 0108 */ int unknown_0108;
  /* 010C */ int unknown_010C;
  /* 0110 */ unsigned char unknown_0110;
  /* 0111 */ unsigned char unknown_0111[0x7];
};

#endif
