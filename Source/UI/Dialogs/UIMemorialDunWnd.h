#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIMEMORIALDUNWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIMEMORIALDUNWND_H

#include "UI/Window/UIFrameWnd.h"

extern int g_memorialDunWndX;
extern int g_memorialDunWndY;

class UIMemorialDunWnd : public UIFrameWnd {
public:
  void Move(int x, int y) override;
};

#endif
