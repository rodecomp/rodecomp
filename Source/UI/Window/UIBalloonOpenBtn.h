#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIBALLOONOPENBTN_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIBALLOONOPENBTN_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIBalloonOpenBtn : public UIFrameWnd {
public:
  void Paint() override;
  void Slot19() override;

protected:
  /* 00B4 */ std::uint32_t unknown_00B4;
  /* 00B8 */ std::int64_t m_openTime;
  /* 00C0 */ std::int64_t m_openDelay;
  /* 00C8 */ int m_balloonId;
  /* 00CC */ UIWindow* m_balloon;
};

#endif
