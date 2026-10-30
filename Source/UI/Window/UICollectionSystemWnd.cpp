#include "UI/Window/UICollectionSystemWnd.h"

// OFFSET: 0x00934740
// FUNCTION: fn_12741
int CUICollectionSystemWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t b, std::intptr_t, std::intptr_t) {
  if (message == 80) {
    if (a == 0) {
      this->unknown_00B8 = b;
      return 0;
    }
    this->m_lastFrame = b;
    this->m_frame = 0;
    this->m_frameDelay = 200;
  }
  return 0;
}
