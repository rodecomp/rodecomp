#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIOPENMAILBOXWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIOPENMAILBOXWND_H

#include "UI/Window/UIFrameWnd.h"

class UIOpenMailBoxWnd : public UIFrameWnd {
public:
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

#endif
