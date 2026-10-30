#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICOLLECTIONSYSTEMWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICOLLECTIONSYSTEMWND_H

#include "UI/Window/UIFrameWnd.h"

class CUICollectionSystemWnd : public UIFrameWnd {
public:
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

private:
  /* 00B4 */ int m_frameDelay;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int m_frame;
  /* 00C4 */ int m_lastFrame;
};

#endif
