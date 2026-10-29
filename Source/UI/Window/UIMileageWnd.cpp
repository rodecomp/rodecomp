#include "UI/Window/UIMileageWnd.h"
#include "Contents/Configuration/ServiceOptions.h"

#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24859
int g_mileageWndActive;

// GLOBAL: gl_24860
int g_mileageWndPoint;

// GLOBAL: gl_24861
int g_mileageWndBonus;

// OFFSET: 0x00950690
// FUNCTION: fn_12942
int UIMileageWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 34) {
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  g_mileageWndActive = static_cast<int>(a);
  this->Show(static_cast<int>(a));

  if (a != 0) {
    this->m_point = static_cast<int>(b);
    g_mileageWndPoint = static_cast<int>(b);
    this->m_bonus = static_cast<int>(d);
    g_mileageWndBonus = static_cast<int>(d);

    int remaining = 3600 - static_cast<int>(c);
    if (remaining < 0) {
      remaining = 0;
    }

    this->m_remaining = remaining;
    this->m_startTime = timeGetTime();
    this->Show(g_serviceOptions.Get(219));
  }

  this->Invalidate();
  return 0;
}
