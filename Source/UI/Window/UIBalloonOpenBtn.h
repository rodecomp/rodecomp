#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIBALLOONOPENBTN_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIBALLOONOPENBTN_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIBitmapButton;
struct UIButtonContext;

class UIBalloonOpenBtn : public UIFrameWnd {
public:
  UIBalloonOpenBtn(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Paint() override;
  void Slot15(int width, int height) override;
  void Slot19() override;

protected:
  /* 00B4 */ std::uint32_t unknown_00B4;
  /* 00B8 */ std::int64_t m_openTime;
  /* 00C0 */ std::int64_t m_openDelay;
  /* 00C8 */ int m_balloonId;
  /* 00CC */ UIWindow* m_balloon;
  /* 00D0 */ UIBitmapButton* m_button;
  UIButtonContext* m_controls;
};

#endif
