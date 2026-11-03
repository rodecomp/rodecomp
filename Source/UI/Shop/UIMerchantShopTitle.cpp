#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/ForbiddenWords.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Shop/UIMerchantShopTitle.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Text/UIStaticText.h"

// OFFSET: 0x0085ACE0
// FUNCTION: fn_10788
void UIMerchantShopTitle::Slot15(int, int) {
  this->label = new UIStaticText();
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
    static_cast<CMode*>(g_charViewMode)->SetCursorAction(2);
  } else {
    static_cast<CMode*>(g_charViewMode)->SetCursorAction(0);
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

// OFFSET: 0x0084FE60
// FUNCTION: fn_10667
UIMerchantShopTitle::UIMerchantShopTitle(int buyingShop) : UIWindow(0), buyingShop(buyingShop) {
  this->geometry.id = 189;
}

// OFFSET: 0x00864DB0
// FUNCTION: fn_10918
int UIMerchantShopTitle::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    return 0;
  }
  if (message == 28) {
    this->ownerAid = a;
    return 0;
  }
  if (message != 34) {
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  char* text = reinterpret_cast<char*>(a);
  if (g_forbiddenWords.Contains(text) == true) {
    std::string masked;
    g_forbiddenWords.Mask(text, &masked);
  }

  std::string title(text);
  const std::size_t end = title.find('\r');
  if (end != std::string::npos) {
    title = title.substr(0, end);
  }

  this->unknown_0084 = static_cast<std::int16_t>(b);
  switch (this->unknown_0084) {
  case 1:
    this->label->fontFamily = 21;
    break;
  case 2:
    this->label->fontFamily = 20;
    break;
  case 3:
    this->label->fontFamily = 22;
    break;
  case 4:
    this->label->fontFamily = 23;
    break;
  case 5:
    this->label->fontFamily = 24;
    break;
  case 6:
    this->label->fontFamily = 25;
    break;
  case 7:
    this->label->fontFamily = 26;
    break;
  case 8:
    this->label->fontFamily = 27;
    break;
  case 9:
    this->label->fontFamily = 28;
    break;
  default:
    this->label->fontFamily = 0;
    break;
  }
  this->label->bold = 0;
  this->label->italic = 0;
  this->label->fontHeight = 12;
  this->label->SetTextFitted(title.c_str());
  this->Invalidate();
  return 0;
}
