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
