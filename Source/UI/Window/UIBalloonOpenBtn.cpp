#include "UI/Window/UIBalloonOpenBtn.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>
#include <mmsystem.h>

// OFFSET: 0x0094D250
// FUNCTION: fn_12902
void UIBalloonOpenBtn::Paint() {
  this->ClearSurface(0xFFFFFFFF);
}

// OFFSET: 0x0094F0E0
// FUNCTION: fn_12931
void UIBalloonOpenBtn::Slot19() {
  if (this->m_balloon) {
    const std::int64_t elapsed = static_cast<std::int64_t>(timeGetTime()) - this->m_openTime;
    if (elapsed > this->m_openDelay) {
      reinterpret_cast<void(__thiscall*)(UIWindowMgr*, int)>(0x00A8CF20)(&g_windowMgr, 0x166);
    }
  }
}

// OFFSET: 0x009468D0
// FUNCTION: fn_12847
UIBalloonOpenBtn::UIBalloonOpenBtn(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_openTime(0), m_openDelay(4000), m_balloonId(0), m_balloon(nullptr), m_button(nullptr), m_controls(&controls) {
}
