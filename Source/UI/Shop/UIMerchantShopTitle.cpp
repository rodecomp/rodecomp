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
#include "UI/Text/UIStaticText.h"

// OFFSET: 0x0085ACE0
// FUNCTION: fn_10788
void UIMerchantShopTitle::Slot15(int, int) {
  this->label = new UIStaticText(*this->m_context, *this->m_controls);
  this->label->SetSize(this->geometry.width - 34, 15);
  this->label->Move(31, 8);
  this->label->SetTextFitted("");
  this->label->fontFamily = 0;
  this->label->fontHeight = 12;
  this->label->bold = 0;
  this->label->italic = 0;
  this->AddChild(this->label);
}

// OFFSET: 0x0085D150
// FUNCTION: fn_10811
void UIMerchantShopTitle::Paint() {
  this->ClearSurface(0xFFFF00FF);
  const std::uint32_t color = this->ownerAid == g_session.unknown_5BF0 ? 0x7DFAFF : 0xFFFFFF;
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height - 6, color);
  DrawRoundedBorder(*this, 2, 2, this->geometry.width - 4, this->geometry.height - 10, 0xC3C3C3);
  const char* icon = this->buyingShop != 0 ? "유저인터페이스\\buyingshop.bmp" : "유저인터페이스\\shop.bmp";
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(icon)));
  this->DrawBitmap(3, 3, bitmap ? &bitmap->image : nullptr, true);
}

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

// OFFSET: 0x0085FE20
// FUNCTION: fn_10886
void UIMerchantShopTitle::Slot32(int, int) {
  if (g_serviceOptions.Get(167)) {
    g_modeMgr.gameMode->SendMsg(245, 0, 0, 0, 0);
    g_session.unknown_1760.clear();
    g_session.unknown_1748.clear();
  }
}
