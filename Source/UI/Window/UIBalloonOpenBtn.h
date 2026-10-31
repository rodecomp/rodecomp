#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIBALLOONOPENBTN_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIBALLOONOPENBTN_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIBitmapButton;
class UITransBalloonText;
struct UIButtonContext;

class UIBalloonOpenBtn : public UIFrameWnd {
public:
  UIBalloonOpenBtn(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Paint() override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00B4 */ std::uint32_t unknown_00B4;
  /* 00B8 */ std::int64_t m_openTime;
  /* 00C0 */ std::int64_t m_openDelay;
  /* 00C8 */ int m_balloonId;
  /* 00CC */ UITransBalloonText* m_balloon;
  /* 00D0 */ UIBitmapButton* m_button;
  UIButtonContext* m_controls;
};

#endif
