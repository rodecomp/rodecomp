#include "Client/Mode/ModeCursor.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Shop/UIMerchantShopTitle.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"

// OFFSET: 0x0085D150

// OFFSET: 0x0085EC40
// FUNCTION: fn_10843
void UIMerchantShopTitle::Slot25(int, int) {
  if (!g_serviceOptions.Get(167)) {
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  if (this->buyingShop != 0) {
    if (g_windowMgr.unknown_0308 == 0) {
      mode->SendMsg(272, this->ownerAid, 0, 0, 0);
    }
  } else if (g_windowMgr.unknown_0418 == 0) {
    mode->SendMsg(84, this->ownerAid, 0, 0, 0);
  }
}

// OFFSET: 0x0085E920
// FUNCTION: fn_10832
void UIMerchantShopTitle::Slot26(int, int) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (this->buyingShop != 0) {
    if (g_windowMgr.unknown_0308 == 0) {
      mode->SendMsg(272, this->ownerAid, 0, 0, 0);
    }
  } else if (g_windowMgr.unknown_0418 == 0) {
    mode->SendMsg(84, this->ownerAid, 0, 0, 0);
  }
}

// OFFSET: 0x0085F9E0
// FUNCTION: fn_10878
void UIMerchantShopTitle::ResetCursor(int x, int y) {
  if (x >= 0 && x < this->geometry.width && y >= 0 && y <= this->geometry.height) {
    this->m_context->cursor.Set(2, this->m_context->cursorTick);
  } else {
    this->m_context->cursor.Set(0, this->m_context->cursorTick);
  }
}
