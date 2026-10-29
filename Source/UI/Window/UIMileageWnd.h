#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMILEAGEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMILEAGEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIMileageWnd : public UIFrameWnd {
public:
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  /* 00B4 */ std::uint32_t m_startTime;
  /* 00B8 */ int m_point;
  /* 00BC */ int m_remaining;
  /* 00C0 */ int m_bonus;
};

extern int g_mileageWndActive;
extern int g_mileageWndPoint;
extern int g_mileageWndBonus;

#endif
