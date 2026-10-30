#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINEWSKILLLISTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINEWSKILLLISTWND_H

#include "UI/Window/UIFrameWnd.h"

struct SkillListWndInfo;
class UIScrollBar;

class UINewSkillListWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Slot35(int delta) override;

protected:
  /* 00B4 */ unsigned char unknown_00B4[4];
  /* 00B8 */ UIScrollBar* m_scrollBar;
  /* 00BC */ unsigned char unknown_00BC[0x190];
  /* 024C */ SkillListWndInfo* m_saved;
  /* 0250 */ unsigned char unknown_0250[0x10];
  /* 0260 */ int m_visibleRows;
  /* 0264 */ unsigned char unknown_0264[0xC];
  /* 0270 */ unsigned char m_scrollable;
};

#endif
