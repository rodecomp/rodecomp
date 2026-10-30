#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONWND_H

#include "UI/Window/UIFrameWnd.h"

struct NavigationWndInfo {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
  /* 0010 */ bool minimized;
};

class UINavigationWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Slot31(int x, int y) override;

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x70];
  /* 0124 */ NavigationWndInfo* m_saved;
};

#endif
