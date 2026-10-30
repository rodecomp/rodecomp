#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICOLLECTIONSYSTEMWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICOLLECTIONSYSTEMWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class CActRes;
class CSprRes;

class CUICollectionSystemWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

private:
  /* 00B4 */ int m_frameDelay;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int m_frame;
  /* 00C4 */ int m_lastFrame;
  /* 00C8 */ bool m_paused;
  /* 00C9 */ unsigned char unknown_00C9[3];
  /* 00CC */ std::uint32_t m_lastTick;
  /* 00D0 */ CActRes* m_action;
  /* 00D4 */ CSprRes* m_sprite;
};

#endif
