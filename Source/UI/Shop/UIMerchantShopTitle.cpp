#include "Client/Mode/CMode.h"
#include "UI/GameSettings/TalkType.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Shop/UIMerchantShopTitle.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/ColoredText.h"

// OFFSET: 0x008519B0
// FUNCTION: fn_45642
UIMerchantShopTitle::~UIMerchantShopTitle() = default;

// OFFSET: 0x0085ACE0
// FUNCTION: fn_10788
void UIMerchantShopTitle::OnCreate(int, int) {
  this->label = new UIStaticText();
  this->label->SetSize(this->geometry.width - 34, 15);
  this->label->Move(31, 8);
  this->label->SetTextFitted("");
  auto* const label = this->label;
  label->fontFamily = 0;
  label->fontHeight = 12;
  label->bold = 0;
  label->italic = 0;
  this->AddChild(this->label);
}

// OFFSET: 0x0085D150
// FUNCTION: fn_10811
void UIMerchantShopTitle::Paint() {
  this->ClearSurface(0xFFFF00FF);
  const std::uint32_t color = this->ownerAid == g_session.storeSearchOwnerAid ? 0x7DFAFF : 0xFFFFFF;
  this->DrawFilledRoundedOutline(0, 0, this->geometry.width, this->geometry.height - 6, color);
  this->DrawRoundedOutline(2, 2, this->geometry.width - 4, this->geometry.height - 10, 0xC3C3C3);
  const int buying = this->buyingShop;
  CBitmapRes* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(buying == 0 ? "유저인터페이스\\shop.bmp" : "유저인터페이스\\buyingshop.bmp")));
  this->DrawBitmap(3, 3, icon, true);
}

// OFFSET: 0x0085EC40
// FUNCTION: fn_10843
void UIMerchantShopTitle::OnLBtnDown(int, int) {
  if (!g_serviceOptions.Get(TT_SHOPPING_ON_OFF)) {
    return;
  }

  CGameMode* mode = g_modeMgr.GetGameMode();
  if (this->buyingShop != 0) {
    if (g_windowMgr.merchantItemPurchaseWnd == 0) {
      mode->SendMsg(MM_REQ_CLICK_TO_BUYING_STORE, this->ownerAid, 0, 0, 0);
    }
  } else if (g_windowMgr.buyingShopItemsWnd == 0) {
    mode->SendMsg(MM_REQ_BUY_FROMMC, this->ownerAid, 0, 0, 0);
  }
}

// OFFSET: 0x0085E920
// FUNCTION: fn_10832
void UIMerchantShopTitle::OnLBtnDblClk(int, int) {
  CGameMode* mode = g_modeMgr.GetGameMode();
  if (this->buyingShop != 0) {
    if (g_windowMgr.merchantItemPurchaseWnd == 0) {
      mode->SendMsg(MM_REQ_CLICK_TO_BUYING_STORE, this->ownerAid, 0, 0, 0);
    }
  } else if (g_windowMgr.buyingShopItemsWnd == 0) {
    mode->SendMsg(MM_REQ_BUY_FROMMC, this->ownerAid, 0, 0, 0);
  }
}

// OFFSET: 0x0085F9E0
// FUNCTION: fn_10878
void UIMerchantShopTitle::ResetCursor(int x, int y) {
  if (x >= 0 && x < this->geometry.width && y >= 0 && y <= this->geometry.height) {
    g_modeMgr.currentMode->SetCursorAction(2);
  } else {
    g_modeMgr.currentMode->SetCursorAction(0);
  }
}

// OFFSET: 0x0085FE20
// FUNCTION: fn_10886
void UIMerchantShopTitle::OnRBtnDown(int, int) {
  if (g_serviceOptions.Get(TT_SHOPPING_ON_OFF)) {
    g_modeMgr.currentMode->SendMsg(MM_RELEASE_QUEST_SH, 0, 0, 0, 0);
    g_session.ClearItems1760And1748();
  }
}

// OFFSET: 0x0084FE60
// FUNCTION: fn_10667
UIMerchantShopTitle::UIMerchantShopTitle(int buyingShop) : UIWindow(0), buyingShop(buyingShop) {
  this->geometry.id = 189;
}

// INLINED
static void SetLabelFont(UIStaticText* label, int family, int height) {
  label->fontFamily = family;
  label->fontHeight = height;
  label->bold = 0;
  label->italic = 0;
}

// OFFSET: 0x00864DB0
// FUNCTION: fn_10918
int UIMerchantShopTitle::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    break;

  case UM_SETINFO: {
    char* text = reinterpret_cast<char*>(a);
    if (g_forbiddenWords.Contains(text) == true) {
      std::string masked = "";
      g_forbiddenWords.Mask(text, &masked);
    }

    std::string title(text);
    const std::size_t end = title.find('\r');
    if (end != std::string::npos) {
      title = title.substr(0, end);
    }

    const char* fitted = title.c_str();
    this->fontStyle = static_cast<std::int16_t>(b);
    switch (this->fontStyle) {
    case TEXT_FONT_STYLE_RIX_LOVEANGEL:
      SetLabelFont(this->label, 21, 12);
      break;
    case TEXT_FONT_STYLE_RIX_SQUIRREL:
      SetLabelFont(this->label, 20, 12);
      break;
    case TEXT_FONT_STYLE_RIX_NHCGOGO:
      SetLabelFont(this->label, 22, 12);
      break;
    case TEXT_FONT_STYLE_RIX_DIARY:
      SetLabelFont(this->label, 23, 12);
      break;
    case TEXT_FONT_STYLE_RIX_MINI_HEART:
      SetLabelFont(this->label, 24, 12);
      break;
    case TEXT_FONT_STYLE_RIX_FRESHMAN:
      SetLabelFont(this->label, 25, 12);
      break;
    case TEXT_FONT_STYLE_RIX_KID:
      SetLabelFont(this->label, 26, 12);
      break;
    case TEXT_FONT_STYLE_RIX_MAGIC:
      SetLabelFont(this->label, 27, 12);
      break;
    case TEXT_FONT_STYLE_RIX_JJANGU:
      SetLabelFont(this->label, 28, 12);
      break;
    default:
      SetLabelFont(this->label, 0, 12);
      break;
    }
    this->label->SetTextFitted(fitted);
    this->Invalidate();
    break;
  }

  case UM_SETNAID:
    this->ownerAid = a;
    break;

  default:
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}
