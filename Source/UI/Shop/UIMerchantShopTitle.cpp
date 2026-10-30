#include "Client/Mode/ModeCursor.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Shop/UIMerchantShopTitle.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x0085EC40

// OFFSET: 0x0085E920

// OFFSET: 0x0085F9E0
// FUNCTION: fn_10878
void UIMerchantShopTitle::ResetCursor(int x, int y) {
  if (x >= 0 && x < this->geometry.width && y >= 0 && y <= this->geometry.height) {
    this->m_context->cursor.Set(2, this->m_context->cursorTick);
  } else {
    this->m_context->cursor.Set(0, this->m_context->cursorTick);
  }
}
