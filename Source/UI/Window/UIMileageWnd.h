#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMILEAGEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMILEAGEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>

struct UIButtonContext;

class UIMileageWnd : public UIFrameWnd {
public:
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  /* 00B4 */ std::uint32_t m_startTime;
  /* 00B8 */ int m_point;
  /* 00BC */ int m_remaining;
  /* 00C0 */ int m_bonus;
  /* 00C4 */ std::string m_tooltip;
  UIButtonContext* m_controls;
};

extern int g_mileageWndActive;
extern int g_mileageWndPoint;
extern int g_mileageWndElapsed;
extern int g_mileageWndBonus;

#endif
