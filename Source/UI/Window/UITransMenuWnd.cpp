#include "UI/Window/UITransMenuWnd.h"

// OFFSET: 0x00951440
// FUNCTION: fn_12950
int UITransMenuWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 48) {
    this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}
