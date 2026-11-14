#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/CashShop/UICashShopWnd.h"
#include "UI/Window/UIEnums.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Application/Registry.h"
#include "Client/Session/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Edit/UIEditCtrl_Currency.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIScrollBarVariants.h"
#include "UI/Buttons/UIStaticBitmap.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include "UI/Login/UIPasswordWnd.h"
#include <algorithm>
#include <array>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <utility>
#include <windows.h>
#include <shellapi.h>
#include <timeapi.h>

template __declspec(noinline) char* std::allocator<char>::allocate(const std::size_t);

template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

#pragma pack(push, 1)
struct CashItemPurchase {
  /* 0000 */ int itemId;
  /* 0004 */ int quantity;
  /* 0008 */ std::uint16_t tab;
};

struct PACKET_CZ_SE_PC_BUY_CASHITEM_LIST {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::uint16_t packetLength;
  /* 0004 */ std::uint16_t count;
  /* 0006 */ int kafraPoints;
  /* 000A */ CashItemPurchase items[203];
};
#pragma pack(pop)

static_assert(sizeof(CashItemPurchase) == 10);
static_assert(offsetof(PACKET_CZ_SE_PC_BUY_CASHITEM_LIST, items) == 10);

struct CashShopReplaceCartItemText {
  EncryptedName<85, 0x3B915119EFD523EFULL>& operator()() const;
};

// OFFSET: 0x007CD5F0
// FUNCTION: fn_09511
UICashShopWnd::UICashShopWnd()
    : UIFrameWnd(0), m_banner(nullptr), m_secondBanner(nullptr), m_selectedTab(0), m_itemsPerPage(9), m_columns(3), m_page(0),
      m_pageCount(0), m_searchEdit(nullptr), m_cashPoints(0), m_secondaryPoints(0), m_totalPrice(0), m_pointsEdit(nullptr), m_cart(nullptr) {
}

// OFFSET: 0x007CD750
// FUNCTION: fn_09512
UICashShopWnd_Banner::UICashShopWnd_Banner()
    : UIWindow(0), m_index(0), m_lastTick(timeGetTime()) {
}

// OFFSET: 0x007CD800
// FUNCTION: fn_09513
UICashShopWnd_Cart::UICashShopWnd_Cart()
    : UIWindow(0), m_scrollBar(nullptr), m_visibleCount(6), m_scrollIndex(0) {
}

// OFFSET: 0x007CD8C0
// FUNCTION: fn_09514
UICashShopWnd_CartItem::UICashShopWnd_CartItem() : UIWindow(0) {
}

// OFFSET: 0x007CDE30
// FUNCTION: fn_09521
__declspec(noinline) UICashShopWnd::~UICashShopWnd() {
}

// OFFSET: 0x007CE110
// FUNCTION: fn_09523
EncryptedName<85, 0x3B915119EFD523EFULL>& CashShopReplaceCartItemText::operator()() const {
  OBFUSCATE_STRING_BODY("구입 리스트에는 한 종류의 아이템만 담을 수 있습니다. [%s] 상품으로 교체하시겠습니까?", 0x3B915119EFD523EFULL);
}

// OFFSET: 0x007CE570
// FUNCTION: fn_09529
void UICashShopWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIFrameWnd::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  if (mode == nullptr) {
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 4) {
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }

  if (drag->quantity <= 1) {
    this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    return;
  }

  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
  if (g_windowMgr.itemDropCntWnd != nullptr) {
    g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 20, drag->quantity, 0, 0);
  }
}

// OFFSET: 0x007CE630
// FUNCTION: fn_09530
void UICashShopWnd_Cart::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  if (mode == nullptr) {
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 6) {
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }

  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
  if (g_windowMgr.itemDropCntWnd != nullptr) {
    g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 19, 0, 0, 0);
  }
}

// OFFSET: 0x007CE6C0
// FUNCTION: fn_09531
void UICashShopWnd_CartItem::DragAndDrop(int x, int y, std::intptr_t value) {
  if (this->parent != nullptr) {
    this->parent->DragAndDrop(x, y, value);
  }
}

// OFFSET: 0x007CE6E0
// FUNCTION: fn_09532
void UICashShopWnd_Item::DragAndDrop(int x, int y, std::intptr_t value) {
  if (this->parent != nullptr) {
    this->parent->DragAndDrop(x, y, value);
  }
}

// OFFSET: 0x007CE700
// FUNCTION: fn_09533
void FormatMessageBuffer(char* buffer, const char* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  _vsnprintf(buffer, 0x800, format, arguments);
  va_end(arguments);
}

// OFFSET: 0x007CE730
// FUNCTION: fn_09534
bool UICashShopWnd::NeedsFrameRefresh() {
  return true;
}

// OFFSET: 0x007CE740
// FUNCTION: fn_09535
void UICashShopWnd::OnCreate(int, int) {
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  this->m_banner = new UICashShopWnd_Banner();
  this->m_banner->SetSize(0x213, 0x33);
  this->m_banner->Move(3, 0x12);
  this->AddChild(this->m_banner);
  if (g_session.m_launchPassCommandLine) {
    this->m_secondBanner = new UICashShopWnd_Banner();
    this->m_secondBanner->SetSize(0xB0, 0x38);
    this->m_secondBanner->Move(0x21A, 0x1A9);
    this->AddChild(this->m_secondBanner);
  }

  int tabX = 8;
  for (int tab = 0; tab < 11; ++tab) {
    const int tooltips[11] = {0xCDA, 0xCDB, 0xCDC, 0xCDD, 0xCDE, 0xCDF, 0xCE0, 0xCE1, 0xCE2, 0xE48, 0xECE};
    UIBmpRadioBtn* button = nullptr;
    if (tab != 8 && tab != 10) {
      button = new UIBmpRadioBtn();
      button->SetBitmap(FormatOwnedString("유저인터페이스\\cashshop\\img_shop_tap%d_off.bmp", tab).c_str(), 0);
      button->SetBitmap(FormatOwnedString("유저인터페이스\\cashshop\\img_shop_tap%d_off.bmp", tab).c_str(), 1);
      button->SetBitmap(FormatOwnedString("유저인터페이스\\cashshop\\img_shop_tap%d_on.bmp", tab).c_str(), 2);
      button->SetBitmap(FormatOwnedString("유저인터페이스\\cashshop\\img_shop_tap%d_disable.bmp", tab).c_str(), 3);
      button->Move(tabX, 0x48);
      button->m_tooltip = LookupMessage(tooltips[tab]);
      button->SendMsg(this, UM_SETSTATE, tab == this->m_selectedTab, 0, 0, 0);
      tabX += 0x3A;
      this->AddChild(button);
    }

    this->m_tabButtons.insert(std::make_pair(tab, button));
  }

  UIBitmapButton* previous = new UIBitmapButton();
  previous->SetBitmap("유저인터페이스\\cashshop\\bt_arrowL_off.bmp", 0, 0);
  previous->SetBitmap("유저인터페이스\\cashshop\\bt_arrowL_off.bmp", 1, 0);
  previous->SetBitmap("유저인터페이스\\cashshop\\bt_arrowL_on.bmp", 2, 0);
  previous->SetSize(previous->BitmapWidth(), previous->BitmapHeight());
  previous->Move(0xEC, 0x1ED);
  previous->SetId(0x13C);
  this->AddChild(previous);

  UIBitmapButton* next = new UIBitmapButton();
  next->SetBitmap("유저인터페이스\\cashshop\\bt_arrowR_off.bmp", 0, 0);
  next->SetBitmap("유저인터페이스\\cashshop\\bt_arrowR_off.bmp", 1, 0);
  next->SetBitmap("유저인터페이스\\cashshop\\bt_arrowR_on.bmp", 2, 0);
  next->SetSize(next->BitmapWidth(), next->BitmapHeight());
  next->Move(0x122, 0x1ED);
  next->SetId(0x13D);
  this->AddChild(next);

  UIBitmapButton* first = new UIBitmapButton();
  first->SetBitmap("유저인터페이스\\cashshop\\bt_arrowL2_off.bmp", 0, 0);
  first->SetBitmap("유저인터페이스\\cashshop\\bt_arrowL2_off.bmp", 1, 0);
  first->SetBitmap("유저인터페이스\\cashshop\\bt_arrowL2_on.bmp", 2, 0);
  first->SetSize(first->BitmapWidth(), first->BitmapHeight());
  first->Move(0xD9, 0x1ED);
  first->SetId(0x183);
  this->AddChild(first);

  UIBitmapButton* last = new UIBitmapButton();
  last->SetBitmap("유저인터페이스\\cashshop\\bt_arrowR2_off.bmp", 0, 0);
  last->SetBitmap("유저인터페이스\\cashshop\\bt_arrowR2_off.bmp", 1, 0);
  last->SetBitmap("유저인터페이스\\cashshop\\bt_arrowR2_on.bmp", 2, 0);
  last->SetSize(last->BitmapWidth(), last->BitmapHeight());
  last->Move(0x130, 0x1ED);
  last->SetId(0x184);
  this->AddChild(last);

  UIBmpTextButton* charge = new UIBmpTextButton();
  charge->SetBitmap("유저인터페이스\\cashshop\\btn_charge_normal.bmp", 0, 0);
  charge->SetBitmap("유저인터페이스\\cashshop\\btn_charge_press.bmp", 1, 0);
  charge->SetBitmap("유저인터페이스\\cashshop\\btn_charge_press.bmp", 2, 0);
  charge->SetSize(charge->BitmapWidth(), charge->BitmapHeight());
  charge->SetText(LookupMessage(MSI_ITEM_SEARCH));
  charge->Move(0x28E, 0x2E);
  charge->SetId(0x11C);
  this->AddChild(charge);

  this->m_refreshButton = new UIBmpTextButton();
  this->m_refreshButton->SetBitmap("유저인터페이스\\cashshop\\btn_refresh_normal.bmp", 0, 0);
  this->m_refreshButton->SetBitmap("유저인터페이스\\cashshop\\btn_refresh_press.bmp", 1, 0);
  this->m_refreshButton->SetBitmap("유저인터페이스\\cashshop\\btn_refresh_press.bmp", 2, 0);
  this->m_refreshButton->SetSize(this->m_refreshButton->BitmapWidth(), this->m_refreshButton->BitmapHeight());
  this->m_refreshButton->SetText(LookupMessage(MSI_STAR_EMPEROR_M));
  this->m_refreshButton->Move(0x1C4, 0x204);
  this->m_refreshButton->SetId(0x19F);
  this->AddChild(this->m_refreshButton);

  this->m_searchEdit = new UIEditCtrl();
  this->m_searchEdit->SetSize(0x123, 0xE);
  this->m_searchEdit->Move(0x5E, 0x206);
  this->m_searchEdit->preserveInput = 1;
  this->m_searchEdit->SetText(LookupMessage(MSI_CASHSHOP_CASH_POINT));
  this->m_searchEdit->TextColor(0xB1B1B1, 0xFFFFFFFFu);
  this->AddChild(this->m_searchEdit);

  UIBitmapButton* search = new UIBitmapButton();
  search->SetBitmap("유저인터페이스\\cashshop\\btn_searchbar_normal.bmp", 0, 0);
  search->SetBitmap("유저인터페이스\\cashshop\\btn_searchbar_over.bmp", 1, 0);
  search->SetBitmap("유저인터페이스\\cashshop\\btn_searchbar_press.bmp", 2, 0);
  search->SetSize(search->BitmapWidth(), search->BitmapHeight());
  search->Move(0x188, 0x204);
  search->SetId(0x141);
  this->AddChild(search);

  UIBmpTextButton* buy = new UIBmpTextButton();
  buy->SetBitmap("유저인터페이스\\cashshop\\btn_buy_normal.bmp", 0, 0);
  buy->SetBitmap("유저인터페이스\\cashshop\\btn_buy_press.bmp", 1, 0);
  buy->SetBitmap("유저인터페이스\\cashshop\\btn_buy_press.bmp", 2, 0);
  buy->SetSize(buy->BitmapWidth(), buy->BitmapHeight());
  buy->SetText(LookupMessage(MSI_CASHSHOP_FREE_POINT));
  buy->Move(0x228, 0x1FC);
  buy->SetId(0xE0);
  this->AddChild(buy);

  this->m_pointsEdit = new UIEditCtrl_Custom();
  this->m_pointsEdit->SetSize(0x3E, 0xE);
  this->m_pointsEdit->Move(0x27C, 0x1C0);
  this->m_pointsEdit->labelAlignment = 1;
  this->m_pointsEdit->Background(0xCE, 0xD6, 0xEB);
  this->m_pointsEdit->maximum = 10;
  this->m_pointsEdit->SetText(IntToString(0, 0).c_str());
  this->AddChild(this->m_pointsEdit);

  for (int index = 0; index < this->m_itemsPerPage; ++index) {
    UICashShopWnd_Item* item = new UICashShopWnd_Item();
    item->SetSize(0xAC, 0x7E);
    item->Move(index % this->m_columns * 0xAF + 8, index / this->m_columns * 0x7F + 0x6B);
    item->Show(0);
    this->AddChild(item);
    this->m_itemWindows.push_back(item);
  }

  this->m_cart = new UICashShopWnd_Cart();
  this->m_cart->SetSize(0xB9, 0x15E);
  this->m_cart->Move(0x219, 0x48);
  this->AddChild(this->m_cart);

  this->SendMsg(this->m_tabButtons[this->m_selectedTab], UM_COMMAND, ID_RADIOBTN, 0, 0, 0);
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  g_modeMgr.GetGameMode()->SendMsg(MM_GET_ACCOUNT_LIMITED_SALE_LIST, 0, 0, 0, 0);
  if (g_session.m_launchPassCommandLine) {
    charge->Enable(0);
    this->m_pointsEdit->Show(0);
  }
}

// OFFSET: 0x007CF760
// FUNCTION: fn_09536
void UICashShopWnd_Cart::OnCreate(int, int) {
  this->m_scrollBar = new UIScrollBar_Custom();
  this->m_scrollBar->Configure(this->geometry.width - 7, 0, this->geometry.height, 1, 7, 7);
  this->m_scrollBar->SetArrowBitmaps("유저인터페이스\\cashshop\\shop_scroll_up.bmp", "유저인터페이스\\cashshop\\shop_scroll_down.bmp");
  this->m_scrollBar->SetThumbBitmaps(
    "유저인터페이스\\cashshop\\shop_bar_up.bmp", "유저인터페이스\\cashshop\\shop_bar_mid.bmp", "유저인터페이스\\cashshop\\shop_bar_down.bmp"
  );
  this->AddChild(this->m_scrollBar);

  int y = 5;
  for (int index = 0; index < this->m_visibleCount; ++index) {
    UICashShopWnd_CartItem* item = new UICashShopWnd_CartItem();
    item->SetSize(0xAC, 0x36);
    item->Move(3, y);
    item->Show(0);
    this->AddChild(item);
    this->m_slots.push_back(item);
    y += 0x39;
  }
}

// OFFSET: 0x007CFB20
// FUNCTION: fn_09537
void UICashShopWnd_CartItem::OnCreate(int, int) {
  UIBitmapButton* close = new UIBitmapButton();
  close->SetBitmap("유저인터페이스\\cashshop\\btn_close.bmp", 0, 0);
  close->SetBitmap("유저인터페이스\\cashshop\\btn_close.bmp", 1, 0);
  close->SetBitmap("유저인터페이스\\cashshop\\btn_close.bmp", 2, 0);
  close->SetSize(close->BitmapWidth(), close->BitmapHeight());
  close->Move(0xA1, 2);
  close->SetId(0xD3);
  this->AddChild(close);

  UIBitmapButton* plus = new UIBitmapButton();
  plus->SetBitmap("유저인터페이스\\cashshop\\btn_plus_normal.bmp", 0, 0);
  plus->SetBitmap("유저인터페이스\\cashshop\\btn_plus_over.bmp", 1, 0);
  plus->SetBitmap("유저인터페이스\\cashshop\\btn_plus_press.bmp", 2, 0);
  plus->SetSize(plus->BitmapWidth(), plus->BitmapHeight());
  plus->Move(0x52, 0x1D);
  plus->SetId(0x117);
  this->AddChild(plus);

  UIBitmapButton* minus = new UIBitmapButton();
  minus->SetBitmap("유저인터페이스\\cashshop\\btn_minus_normal.bmp", 0, 0);
  minus->SetBitmap("유저인터페이스\\cashshop\\btn_minus_over.bmp", 1, 0);
  minus->SetBitmap("유저인터페이스\\cashshop\\btn_minus_press.bmp", 2, 0);
  minus->SetSize(minus->BitmapWidth(), minus->BitmapHeight());
  minus->Move(0x52, 0x26);
  minus->SetId(0x118);
  this->AddChild(minus);
}

namespace {
// INLINED
inline void SetButtonText(UIBmpTextButton* button, std::string value) {
  button->text = value;
  button->Invalidate();
}
}

// OFFSET: 0x007CFD20
// FUNCTION: fn_09538
void UICashShopWnd_Item::OnCreate(int, int) {
  this->m_addButton = new UIBmpTextButton();
  this->m_addButton->SetBitmap("유저인터페이스\\cashshop\\btn_add_normal.bmp", 0, 0);
  this->m_addButton->SetBitmap("유저인터페이스\\cashshop\\btn_add_press.bmp", 1, 0);
  this->m_addButton->SetBitmap("유저인터페이스\\cashshop\\btn_add_press.bmp", 2, 0);
  this->m_addButton->SetBitmap("유저인터페이스\\cashshop\\btn_add_disable.bmp", 3, 0);
  this->m_addButton->SetSize(this->m_addButton->BitmapWidth(), this->m_addButton->BitmapHeight());
  SetButtonText(this->m_addButton, LookupMessage(MSI_BUY));
  this->m_addButton->Move(0x53, 0x55);
  this->m_addButton->SetId(0xE0);
  this->AddChild(this->m_addButton);

  this->m_soldOut = new UIStaticBitmap();
  this->m_soldOut->SetBitmap("유저인터페이스\\cashshop\\img_shop_soldout.bmp");
  this->m_soldOut->SetSize(this->m_soldOut->geometry.width, this->m_soldOut->geometry.height);
  this->m_soldOut->Move(9, 9);
  this->m_soldOut->Show(0);
  this->AddChild(this->m_soldOut);
}

// OFFSET: 0x007CFF20
// FUNCTION: fn_09539
void UICashShopWnd::Paint() {
  const char* background = this->m_selectedTab == 9 ? "유저인터페이스\\cashshop\\img_shop_bg2.bmp" : "유저인터페이스\\cashshop\\img_shop_bg.bmp";
  CBitmapRes* backgroundBitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background)));
  this->DrawBitmap(0, 0, backgroundBitmap, false);
  if (g_session.m_launchPassCommandLine) {
    this->DrawBitmap(
      0x218, 0x11, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashshop\\img_shop_bg3_cover.bmp"))), false
    );
  }

  this->DrawTitleBar(false, LookupMessage(MSI_CASHSHOP_TAB_HOT), 0);
  if (this->m_pageCount == 0) {
    this->DrawWrappedFormattedText(0xF7, 0x1EE, std::string("0 / 0"), 0x2B, 0, 6, 0, 0xC, 0x6B2921, 0);
  } else {
    this->DrawWrappedFormattedText(0xF7, 0x1EE, FormatOwnedString("^b%d^/b / %d", this->m_page + 1, this->m_pageCount), 0x2B, 0, 6, 0, 0xC, 0x6B2921, 0);
  }

  if (g_session.m_launchPassCommandLine) {
    this->DrawCenteredText(0x21A, 0x1B, []() {
      const auto source = OBFUSCATE_SOURCE("스토브 구입 리스트", 0x474DC581D5E98357ULL);
      thread_local EncryptedName<sizeof(source), 0x474DC581D5E98357ULL> title(source.data);
      return title.Text();
    }(), 0xB9, 0, 0xE, 0, 1);
    this->DrawCenteredText(0x219, 0x1A, []() {
      const char source[] = OBFUSCATE_SOURCE_CHARS(19, "스토브 구입 리스트", 0x3F557BE333F95F6DULL);
      thread_local EncryptedName<sizeof(source), 0x3F557BE333F95F6DULL> title(source);
      return title.Text();
    }(), 0xB9, 0, 0xE, 0xFFFFFF, 1);
  } else {
    this->DrawCenteredText(0x21A, 0x1B, LookupMessage(MSI_CASHSHOP_TAB_LIMITED), 0xB9, 0, 0xE, 0, 1);
    this->DrawCenteredText(0x219, 0x1A, LookupMessage(MSI_CASHSHOP_TAB_LIMITED), 0xB9, 0, 0xE, 0xFFFFFF, 1);
    this->DrawRightAlignedText(0x273, 0x33, IntToString(this->m_cashPoints, 0).c_str(), 0, 0, 0xC, 0xFFFFFF, 1);
    this->DrawTextAt(0x226, 0x1B2, LookupMessage(MSI_CASHSHOP_REFRESH), 0, 0, 0xC, 0, 0, 0);
    this->DrawRightAlignedText(0x2C4, 0x1B2, FormatOwnedString("%d C", this->m_secondaryPoints).c_str(), 0, 0, 0xC, 0, 0);
    this->DrawTextAt(0x226, 0x1C2, LookupMessage(MSI_STAR_EMPEROR_F), 0, 0, 0xC, 0, 0, 0);
    this->DrawRightAlignedText(0x2C4, 0x1C2, "C", 0, 0, 0xC, 0, 0);
    this->DrawTextAt(0x226, 0x1D2, LookupMessage(MSI_SOUL_REAPER_F), 0, 0, 0xC, 0, 0, 0);
    this->DrawRightAlignedText(0x2C4, 0x1D2, FormatOwnedString("%d C", 0).c_str(), 0, 0, 0xC, 0, 0);
  }

  this->DrawCenteredText(0x228, 0x1EC, FormatOwnedString("%d C", this->m_totalPrice).c_str(), 0x9A, 0, 0xE, 0xFFFFFF, 1);
}

// OFFSET: 0x007D04F0
// FUNCTION: fn_09540
void UICashShopWnd_Banner::Paint() {
  if (this->m_entries.empty()) {
    return;
  }

  this->DrawBitmap(
    0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\cashshop\\" + this->m_entries[this->m_index].first).c_str()))),
    false
  );
}

// OFFSET: 0x007D05F0
// FUNCTION: fn_09541
void UICashShopWnd_Cart::Paint() {
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashshop\\img_shop_cart_bg.bmp"))), false);

  unsigned int index = this->m_scrollIndex;
  for (int slot = 0; slot < this->m_visibleCount; ++slot, ++index) {
    if (index >= this->m_cartItems.size()) {
      this->m_slots[slot]->Show(0);
    } else {
      this->m_slots[slot]->SendMsg(this, UM_REFRESHITEM, reinterpret_cast<std::intptr_t>(&this->m_cartItems[index]), 0, 0, 0);
    }
  }
}

// OFFSET: 0x007D06B0
// FUNCTION: fn_09542
void UICashShopWnd_CartItem::Paint() {
  this->DrawBitmap(
    0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashshop\\img_shop_itemBg2.bmp"))), false
  );

  std::string name = this->FitDisplayText(0x70, this->m_item.displayName.c_str(), 0, 0xB, 0);
  this->DrawTextAt(0x30, 0xC, name.c_str(), 0, 0, 0xB, 0x902418, 0, 0);

  CBitmapRes* icon = static_cast<CBitmapRes*>(
    ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\collection\\%s.bmp", ItemResourcePath(this->m_item)).c_str()))
  );
  if (icon != nullptr) {
    this->DrawBitmapScaled(4, 0, static_cast<int>(icon->image.width * 0.5), static_cast<int>(icon->image.height * 0.5), icon, 1);
  }

  this->DrawRightAlignedText(0xA0, 0x22, FormatOwnedString("%d C", this->m_item.prices[0]).c_str(), 0, 0, 0xC, 0, 0);
  this->DrawCenteredText(0x32, 0x21, IntToString(this->m_item.quantity, 0).c_str(), 0x20, 0, 0xB, 0, 0);
}

// OFFSET: 0x007D08A0
// FUNCTION: fn_09543
void UICashShopWnd_Item::Paint() {
  if (this->m_largeLayout == true) {
    this->DrawBitmap(
      0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashshop\\img_shop_item_bg2.bmp"))), false
    );

    {
      std::string stock;
      if (this->m_item.prices[0] == 0) {
        stock = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_CASHSHOP_NON_TRADABLE)));
      } else {
        stock = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_MAP_PROPERTY_DISABLE_CALLED), this->m_item.quantity));
      }
      this->DrawCenteredOutlinedText(6, 0x22, stock.c_str(), 0x49, 0, 0xFFFFFF, 0, 0, 0xB, 1);
    }

    if (this->m_item.identity[1] > 0) {
      std::string name = this->FitDisplayText(0xA8, this->m_item.displayName.c_str(), 0, 0xC, 1);
      this->DrawCenteredText(0, 6, name.c_str(), this->geometry.width, 0, 0xC, 0xA44F3B, 1);
      this->DrawBitmap(
        5, 0x34,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\collection\\%s.bmp", ItemResourcePath(this->m_item)).c_str()))
        ),
        true
      );
      this->DrawRightAlignedText(0xA0, 0x32, IntToString(this->m_item.prices[0], 0).c_str(), 0, 0, 0xC, 0xFFFFFF, 1);
    }

    if (this->m_item.prices[0] == 0) {
      this->DrawTextAt(0x53, 0x4C, LookupMessage(MSI_ZEROQUEST_QUESTVIEW_ON), 0, 0, 0xB, 0xFF, 0, 0);
    } else {
      std::string count;
      if (static_cast<int>(this->m_item.saleData.purchaseCount) < 0) {
        count = static_cast<const std::string&>(FormatOwnedString("%d/%d", 0, this->m_item.quantity));
      } else {
        count = static_cast<const std::string&>(FormatOwnedString("%d/%d", this->m_item.saleData.purchaseCount, this->m_item.quantity));
      }
      this->DrawTextAt(0x53, 0x4C, LookupMessage(MSI_ACCOUNT_LIMITED_SALE_ITEM_SOLD_OUT), 0, 0, 0xB, 0x6E4B41, 0, 0);
      this->DrawCenteredOutlinedText(0x53, 0x5B, count.c_str(), 0x51, 0, 0xFFFFFF, 0xD29374, 0, 0x10, 1);
    }

    __time32_t& start = this->m_item.saleData.startTime;
    __time32_t& end = this->m_item.saleData.endTime;
    start = start < 0 ? 0 : start;
    end = end < 0 ? 0 : end;

    __time32_t now;
    _time32(&now);
    const tm startTime = *_localtime32(&start);
    const tm endTime = *_localtime32(&end);
    std::string startText = FormatOwnedString(LookupMessage(MSI_ACCOUNT_LIMITED_TOOL_RESULT_FORMAT), startTime.tm_mon + 1, startTime.tm_mday, startTime.tm_hour, startTime.tm_min);
    std::string endText =
      FormatOwnedString(LookupMessage(MSI_GUILD_EXP_TRIBUTE), endTime.tm_year + 1900, endTime.tm_mon + 1, endTime.tm_mday, endTime.tm_hour, endTime.tm_min);

    const char* text = startText.c_str();
    const int center = this->geometry.width / 2;
    const int startWidth = this->TextWidth(text, 0, 0, 0xC, 0, 0);
    this->DrawTextAt(static_cast<int>(center - startWidth * 0.5), 0xA0, text, 0, 0, 0xB, 0, 0, 0);
    text = endText.c_str();
    const int endWidth = this->TextWidth(text, 0, 0, 0xC, 0, 0);
    this->DrawTextAt(0x9E - endWidth, 0xAD, text, 0, 0, 0xB, 0, 0, 0);
  } else {
    this->DrawBitmap(
      0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashshop\\img_shop_itemBg.bmp"))), false
    );
    if (this->m_item.identity[1] > 0) {
      std::string name = this->FitDisplayText(0xA8, this->m_item.displayName.c_str(), 0, 0xC, 1);
      this->DrawCenteredText(0, 6, name.c_str(), this->geometry.width, 0, 0xC, 0xA44F3B, 1);
      this->DrawBitmap(
        5, 0x17,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\collection\\%s.bmp", ItemResourcePath(this->m_item)).c_str()))
        ),
        true
      );
      this->DrawRightAlignedText(0xA0, 0x32, IntToString(this->m_item.prices[0], 0).c_str(), 0, 0, 0xC, 0xFFFFFF, 1);
    }
  }
}

// OFFSET: 0x007D1020
// FUNCTION: fn_09544
void UICashShopWnd_CartItem::OnLBtnDblClk(int, int) {
  this->SendMsg(this, UM_COMMAND, ID_DELETE, 0, 0, 0);
}

// OFFSET: 0x007D1040
// FUNCTION: fn_09545
void UICashShopWnd_CartItem::OnLBtnDown(int, int) {
  if (this->m_item.identity[1] > 0) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())
      ->BeginDragItem(4, this->m_item, this->m_item.quantity, -1);
  }
}

// OFFSET: 0x007D1080
// FUNCTION: fn_09546
void UICashShopWnd_Item::OnLBtnDown(int, int) {
  if (this->m_item.identity[1] > 0) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->BeginDragItem(6, this->m_item, 1, -1);
  }
}

// OFFSET: 0x007D10B0
// FUNCTION: fn_09547
void UICashShopWnd_Banner::OnLBtnUp(int, int) {
  if (this->m_entries.empty()) {
    return;
  }

  std::string url = this->m_entries[this->m_index].second;
  ShellExecuteA(nullptr, "OPEN", url.c_str(), url.c_str(), nullptr, SW_SHOW);
}

// OFFSET: 0x007D1160
// FUNCTION: fn_09548
void UICashShopWnd_CartItem::OnMouseHover(int x, int y) {
  if (static_cast<unsigned int>(x - 0x30) <= 0x70 && static_cast<unsigned int>(y - 0xC) <= 0xB) {
    int screenX = 0;
    int screenY = -10;
    this->ToScreen(screenX, screenY);
    g_modeMgr.gameMode->ShowTooltip(this->m_item.displayName.c_str(), screenX, screenY, 0xFFFFFFFFu, 0, 1);
  }
}

// OFFSET: 0x007D11D0
// FUNCTION: fn_09549
void UICashShopWnd_Item::OnMouseHover(int x, int y) {
  if (y <= 0x17) {
    int screenX = 0;
    int screenY = -20;
    this->ToScreen(screenX, screenY);
    g_modeMgr.gameMode->ShowTooltip(this->m_item.displayName.c_str(), screenX, screenY, 0xFFFFFFFFu, 0, 1);
  }
}

// OFFSET: 0x007D1230
// FUNCTION: fn_09550
void UICashShopWnd_Banner::ResetCursor(int, int) {
  g_modeMgr.gameMode->SetCursorAction(2);
}

// OFFSET: 0x007D1240
// FUNCTION: fn_09551
void UICashShopWnd::OnProcess() {
  std::string search(this->m_searchEdit->Text());
  if (search.compare(LookupMessage(MSI_CASHSHOP_CASH_POINT)) != 0) {
    if (search.size() == 0) {
      this->m_searchEdit->SetText(LookupMessage(MSI_CASHSHOP_CASH_POINT));
      this->m_searchEdit->TextColor(0xB1, 0xB1, 0xB1, 0xFFFFFFFFu);
    } else {
      this->m_searchEdit->TextColor(0, 0, 0, 0xFFFFFFFFu);
    }
  }

  std::string amount(this->m_pointsEdit->Text());
  if (amount.size() == 0) {
    this->m_pointsEdit->SetText(IntToString(0, 0).c_str());
  }

  this->m_banner->OnProcess();
  if (g_session.m_launchPassCommandLine) {
    this->m_secondBanner->OnProcess();
  }
}

// OFFSET: 0x007D14C0
// FUNCTION: fn_09552
void UICashShopWnd_Banner::OnProcess() {
  if (!this->m_entries.empty() && this->m_lastTick + 5000 < timeGetTime()) {
    const unsigned int next = this->m_index + 1;
    this->SendMsg(this->m_buttons[next < this->m_entries.size() ? next : 0], UM_COMMAND, ID_RADIOBTN, 0, 0, 0);
  }
}

// OFFSET: 0x007D1540
// FUNCTION: fn_09553
void UICashShopWnd_CartItem::OnRBtnUp(int x, int) {
  if (static_cast<unsigned int>(x - 4) <= 0x25) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    if (window != nullptr) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&this->m_item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x007D1590
// FUNCTION: fn_09554
void UICashShopWnd_Item::OnRBtnUp(int x, int y) {
  if (static_cast<unsigned int>(x - 5) <= 0x4B && static_cast<unsigned int>(y - 0x17) <= 0x64) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    if (window != nullptr) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&this->m_item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x007D15E0
// FUNCTION: fn_09555
void UICashShopWnd_Cart::OnWheel(int delta) {
  if (delta < 0) {
    this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, 1, 0, 0, 0);
  } else {
    this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, -1, 0, 0, 0);
  }
}

// OFFSET: 0x007D1620
// FUNCTION: fn_09556
void UICashShopWnd_CartItem::OnWheel(int delta) {
  if (this->parent) {
    this->parent->OnWheel(delta);
  }
}

// OFFSET: 0x007D1640
// FUNCTION: fn_09557
void UICashShopWnd_Banner::CreateButtons() {
  int index = 0;
  for (std::vector<std::pair<std::string, std::string>>::iterator entry = this->m_entries.begin(); entry != this->m_entries.end(); ++entry) {
    UIBmpRadioBtn* button = new UIBmpRadioBtn();
    button->SetBitmap("유저인터페이스\\cashshop\\btn_ad_off.bmp", 0);
    button->SetBitmap("유저인터페이스\\cashshop\\btn_ad_off.bmp", 1);
    button->SetBitmap("유저인터페이스\\cashshop\\btn_ad_on.bmp", 2);
    button->Move(this->geometry.width + (index - static_cast<int>(this->m_entries.size())) * 12 - 12, 0x29);
    button->SendMsg(this, UM_SETSTATE, index == this->m_index, 0, 0, 0);
    this->AddChild(button);
    this->m_buttons.emplace_back(button);
    ++index;
  }

  this->Invalidate();
}

// OFFSET: 0x007D17A0
// FUNCTION: fn_09558
int UICashShopWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  CSession& itemLists = g_session;
  const CSession& lists = g_session;
  switch (message) {
  case UM_SETINFO:
    if (g_session.m_launchPassCommandLine) {
      if (g_session.m_hotkeyScript->Call("set_stoveshop_banner", "") == true) {
        this->m_banner->CreateButtons();
        this->m_secondBanner->CreateButtons();
        this->Invalidate();
        return 0;
      }
    } else if (g_session.m_hotkeyScript->Call("set_cashshop_banner", "") == true) {
      this->m_banner->CreateButtons();
      this->Invalidate();
      return 0;
    }
    return 0;

  case UM_SET_CASHSHOP_BALANCE:
  case UM_UPDATE_CASHSHOP_BALANCE: {
    HKEY key;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS) {
      DWORD type = REG_DWORD;
      DWORD size = 4;
      RegQueryValueExA(key, "CASH_CATEGORY", nullptr, &type, reinterpret_cast<LPBYTE>(&this->m_selectedTab), &size);
    }
    RegCloseKey(key);

    this->m_cashPoints = argument1;
    this->m_secondaryPoints = argument2;
    this->m_pointsEdit->SetText(IntToString(0, 0).c_str());
    this->SendMsg(this, UM_COMMAND, 0x182, 0, 0, 0);
    return 0;
  }

  case UM_SET_CASHSHOP_TOTAL_PRICE:
    this->m_totalPrice = argument1;
    this->Invalidate();
    return 0;

  case UM_REFRESHITEM:
    this->m_items.clear();
    for (int index = 0; index < lists.GetInventoryCategorySize(this->m_selectedTab); ++index) {
      ITEM_INFO item = g_session.GetItem1784At(this->m_selectedTab, index);
      if (item.identity[1] != 0) {
        this->m_items.push_back(item);
      }
    }

    this->m_itemsPerPage = this->m_selectedTab == 9 ? 6 : 9;
    if (argument1 == 0) {
      this->m_page = 0;
    }
    this->m_pageCount = this->m_items.size() / this->m_itemsPerPage + (this->m_items.size() % this->m_itemsPerPage != 0);
    this->LayoutPage();
    return 0;

  case UM_DEFPUSH:
    if (sender == this->m_searchEdit) {
      this->SendMsg(nullptr, UM_COMMAND, ID_SEARCH, 0, 0, 0);
    }
    return 0;

  case UM_ADDITEM:
    this->m_cart->SendMsg(sender, UM_ADDITEM, argument1, 1, 0, 0);
    return 0;

  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode == nullptr) {
      return 0;
    }

    ModeDragState* drag = &mode->drag;
    if (drag == nullptr) {
      return 0;
    }

    if (drag->source == 4) {
      std::vector<ITEM_INFO>& cartItems = this->m_cart->m_cartItems;
      std::vector<ITEM_INFO>::iterator entry = cartItems.begin();
      for (; entry != cartItems.end(); ++entry) {
        if (atoi(entry->itemId.c_str()) == atoi(drag->name.c_str())) {
          break;
        }
      }

      if (entry != cartItems.end()) {
        if (entry->quantity > drag->quantity) {
          this->m_cart->SendMsg(this, UM_ADDITEM, reinterpret_cast<std::intptr_t>(&*entry), -drag->quantity, 0, 0);
        } else {
          this->m_cart->SendMsg(this, UM_DECITEM, entry->GetItemId(), 0, 0, 0);
        }
      }
    } else if (drag->source == 6) {
      if (this->m_selectedTab == 9) {
        std::list<ITEM_INFO>::iterator product = this->m_items.begin();
        for (; product != this->m_items.end(); ++product) {
          if (atoi(product->itemId.c_str()) == atoi(drag->name.c_str())) {
            break;
          }
        }

        if (product != this->m_items.end()) {
          const int stock = product->saleData.purchaseCount;
          if (stock <= 0) {
            g_windowMgr.Confirm(LookupMessage(MSI_HAVE_ITEMTAG), 0, true, false, 0, "Message", 0x118, 0x78, 0);
            mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
            mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
            return 0;
          }

          std::vector<ITEM_INFO>& cartItems = this->m_cart->m_cartItems;
          std::vector<ITEM_INFO>::iterator entry = cartItems.begin();
          for (; entry != cartItems.end(); ++entry) {
            if (atoi(entry->itemId.c_str()) == atoi(drag->name.c_str())) {
              break;
            }
          }

          if (entry != cartItems.end() && stock < drag->quantity + entry->quantity) {
            drag->quantity = stock - entry->quantity;
          }
        }
      }

      std::list<ITEM_INFO>::iterator product = this->m_items.begin();
      for (; product != this->m_items.end(); ++product) {
        if (atoi(product->itemId.c_str()) == atoi(drag->name.c_str())) {
          break;
        }
      }

      if (product != this->m_items.end()) {
        this->m_cart->SendMsg(this, UM_ADDITEM, reinterpret_cast<std::intptr_t>(&*product), drag->quantity, 0, 0);
      }
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }

  case UM_COMMAND:
    switch (argument1) {
    case ID_CLOSE:
      if (g_session.m_launchPassCommandLine) {
        g_modeMgr.GetGameMode()->SendMsg(MM_STOVE_AUTHENTICATION, 5, 0, 0, 0);
      }
      g_windowMgr.CloseWindow(WID_CASHSHOP_WND);
      return 0;
    case ID_RADIOBTN: {
      int selected = this->m_selectedTab;
      for (std::map<int, UIWindow*>::iterator tab = this->m_tabButtons.begin(); tab != this->m_tabButtons.end(); ++tab) {
        if (tab->first != 10 && tab->first != 8 && sender == tab->second) {
          selected = tab->first;
        }
      }

      if (selected != this->m_selectedTab) {
        for (std::map<int, UIWindow*>::iterator tab = this->m_tabButtons.begin(); tab != this->m_tabButtons.end(); ++tab) {
          if (tab->first != 10 && tab->first != 8 && tab->second != nullptr) {
            tab->second->SendMsg(this, UM_SETSTATE, selected == tab->first, 0, 0, 0);
          }
        }
        this->m_selectedTab = selected;
        this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }

      this->m_refreshButton->Enable(this->m_selectedTab == 9);
      HKEY key;
      DWORD disposition = 0;
      if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
        RegSetValueExA(key, "NeoCash", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&this->m_selectedTab), 4);
      }
      RegCloseKey(key);
      return 0;
    }
    case ID_BUY: {
      std::vector<ITEM_INFO>& cartItems = this->m_cart->m_cartItems;
      const int count = cartItems.size();
      if (cartItems.empty()) {
        return 0;
      }

      if (g_session.m_launchPassCommandLine) {
        const int owned = lists.GetList0Size() + g_session.CountDistinctEquippedItems() + g_session.CountDistinctCostumeItems();
        if (g_session.m_weight > g_session.m_maxWeight * 0.7f || cartItems.front().quantity + owned > g_session.extraInventorySlots + 100) {
          g_windowMgr.Confirm(LookupMessage(MSI_ANOTHER_WORKING), 0, true, false, 0, "Message", 0x118, 0x78, 0);
          return 0;
        }

        g_modeMgr.GetGameMode()->SendMsg(MM_STOVE_AUTHENTICATION, 3, 0, 0, 0);
        return 0;
      }

      const int points = atoi(this->m_pointsEdit->Text());
      const int price = this->m_totalPrice;
      if (price < points) {
        g_windowMgr.Confirm(LookupMessage(MSI_BUY_CASH_FAIL2), 0, true, false, 0, "Message", 0x118, 0x78, 0);
        return 0;
      }

      const int available = this->m_cashPoints + points;
      char text[0x800];
      if (points > 0) {
        FormatMessageBuffer(text, LookupMessage(MSI_CHANGE_MATERIAL_INVSORT), price - points, points);
      } else {
        FormatMessageBuffer(text, LookupMessage(MSI_BILLING_INFO_URL), price);
      }

      if (this->m_totalPrice > available) {
        g_windowMgr.Confirm(LookupMessage(MSI_BUY_CASH_FAIL_MONEY), 0, true, false, 0, "Message", 0x118, 0x78, 0);
        return 0;
      }

      if (g_windowMgr.Confirm(text, 2, true, true, 0, "Message", 0x118, 0x78, 0) != ID_YES) {
        return 0;
      }

      const unsigned int length = offsetof(PACKET_CZ_SE_PC_BUY_CASHITEM_LIST, items) + count * sizeof(CashItemPurchase);
      if (length > 0x800) {
        return 0;
      }

      PACKET_CZ_SE_PC_BUY_CASHITEM_LIST packet;
      packet.packetType = ID_CZ_SE_PC_BUY_CASHITEM_LIST;
      packet.packetLength = static_cast<std::uint16_t>(length);
      packet.count = static_cast<std::uint16_t>(count);
      packet.kafraPoints = points;
      for (int index = 0; index < count; ++index) {
        const ITEM_INFO& item = cartItems[index];
        CashItemPurchase& purchase = packet.items[index];
        purchase.itemId = atoi(item.itemId.c_str());
        purchase.quantity = item.quantity;
        purchase.tab = item.bindOnEquip;
      }

      GetRagConnection().Send(static_cast<short>(length), &packet);
      return 0;
    }
    case ID_CHARGE:
      ShellExecuteA(nullptr, "OPEN", LookupMessage(MSI_CASHSHOP_FREE_POINT_TO_USE), LookupMessage(MSI_CASHSHOP_FREE_POINT_TO_USE), nullptr, SW_SHOW);
      return 0;
    case ID_PREV_PAGE:
      this->m_page = std::max(this->m_page - 1, 0);
      this->LayoutPage();
      return 0;
    case ID_NEXT_PAGE:
      this->m_page = std::min(this->m_page + 1, this->m_pageCount - 1);
      this->LayoutPage();
      return 0;
    case ID_SEARCH: {
      std::string query(this->m_searchEdit->Text());
      if (query.size() != 0 && strcmp(query.c_str(), LookupMessage(MSI_CASHSHOP_CASH_POINT)) != 0) {
        itemLists.ClearInventoryCategoryItems(10);
        for (int list = 0; list < 11; ++list) {
          if (list == 10 || list == 8 || list == 9) {
            continue;
          }

          for (int index = 0; index < lists.GetInventoryCategorySize(list); ++index) {
            ITEM_INFO item = g_session.GetItem1784At(list, index);
            std::string name(item.displayName);
            if (name.find(query) != std::string::npos && !g_session.HasInventorySearchItem(atoi(item.itemId.c_str()))) {
              g_session.inventoryCategories[10].push_back(item);
            }
          }
        }

        if (lists.GetInventoryCategorySize(10) > 0) {
          this->m_items.clear();
          for (int index = 0; index < lists.GetInventoryCategorySize(10); ++index) {
            this->m_items.push_back(g_session.GetItem1784At(10, index));
          }

          this->m_selectedTab = 10;
          this->m_page = 0;
          this->m_pageCount = this->m_items.size() / this->m_itemsPerPage + (this->m_items.size() % this->m_itemsPerPage != 0);
          for (std::map<int, UIWindow*>::iterator tab = this->m_tabButtons.begin(); tab != this->m_tabButtons.end(); ++tab) {
            if (tab->second != nullptr) {
              tab->second->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
            }
          }
          this->LayoutPage();
          return 0;
        }

        g_windowMgr.Confirm(LookupMessage(MSI_NOTHING_SEARCH_ITEM), 0, true, false, 0, "Message", 0x118, 0x78, 0);
      }
      return 0;
    }
    case 0x182:
      this->m_totalPrice = 0;
      this->m_cart->m_cartItems.clear();
      this->m_cart->SendMsg(this->m_cart, UM_SCROLLVERT, 0, 0, 0, 0);
      this->m_cart->Invalidate();
      this->Invalidate();
      return 0;
    case 0x183:
      this->m_page = 0;
      this->LayoutPage();
      return 0;
    case 0x184:
      this->m_page = this->m_pageCount - 1;
      this->LayoutPage();
      return 0;
    case ID_PARTY_RELOAD_FIRST_PAGE:
      g_modeMgr.GetGameMode()->SendMsg(MM_GET_ACCOUNT_LIMITED_SALE_LIST, 0, 0, 0, 0);
      return 0;
    }
    return 0;

  case UM_SELECT_CASHSHOP_TAB:
    if (argument1 != 0 && argument1 != this->m_selectedTab) {
      for (std::map<int, UIWindow*>::iterator tab = this->m_tabButtons.begin(); tab != this->m_tabButtons.end(); ++tab) {
        if (tab->first != 10 && tab->first != 8 && tab->second != nullptr) {
          tab->second->SendMsg(this, UM_SETSTATE, argument1 == tab->first, 0, 0, 0);
        }
      }
      this->m_selectedTab = argument1;
      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007D2C00
// FUNCTION: fn_09559
int UICashShopWnd_Banner::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == UM_COMMAND && argument1 == ID_RADIOBTN) {
    for (unsigned int index = 0; index < this->m_buttons.size(); ++index) {
      if (sender == this->m_buttons[index]) {
        this->m_buttons[index]->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);
        this->m_index = index;
      } else {
        this->m_buttons[index]->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
      }
    }

    this->m_lastTick = timeGetTime();
    this->Invalidate();
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007D2CB0
// FUNCTION: fn_09560
int UICashShopWnd_Cart::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_ADDITEM: {
    if (argument1 == 0) {
      return -1;
    }

    const ITEM_INFO& item = *reinterpret_cast<const ITEM_INFO*>(argument1);
    int quantity = argument2;
    if (quantity == 0) {
      g_windowMgr.Confirm(LookupMessage(MSI_HAVE_ITEMTAG), 0, true, false, 0, "Message", 0x118, 0x78, 0);
      return -1;
    }

    std::vector<ITEM_INFO>::iterator it = this->m_cartItems.begin();
    const std::vector<ITEM_INFO>::iterator last = this->m_cartItems.end();
    for (; it != last; ++it) {
      if (atoi(it->itemId.c_str()) == atoi(item.itemId.c_str())) {
        break;
      }
    }

    if (it == this->m_cartItems.end()) {
      if (g_session.m_launchPassCommandLine && !this->m_cartItems.empty()) {
        const int result =
          g_windowMgr.Confirm(FormatOwnedString(CashShopReplaceCartItemText()().Text(), item.displayName.c_str()).c_str(), 2, true, false, 0, "Message", 0x118, 0x78, 0);
        if (result != ID_YES) {
          break;
        }
        this->m_cartItems.clear();
      }

      if (this->m_cartItems.size() >= 8) {
        g_windowMgr.Confirm(FormatOwnedString(LookupMessage(MSI_USA_LOGINERRORMSG_NOT_USER), 8).c_str(), 0, true, false, 0, "Message", 0x118, 0x78, 0);
        return -1;
      }

      ITEM_INFO entry(item);
      const int limit = entry.saleData.purchaseCount;
      if (limit != 0) {
        if (quantity > limit) {
          g_windowMgr.Confirm(FormatOwnedString(LookupMessage(MSI_BARGAINSALE_TOOLWND_CALL), limit).c_str(), 0, true, false, 0, "Message", 0x118, 0x78, 0);
        }
        if (quantity > limit) {
          quantity = limit;
        }
      }
      entry.quantity = quantity;
      this->m_cartItems.push_back(entry);
      this->SendMsg(this, UM_SCROLLVERT, 1, 0, 0, 0);
    } else {
      const int limit = it->saleData.purchaseCount;
      if (limit != 0 && it->quantity + quantity > limit) {
        g_windowMgr.Confirm(FormatOwnedString(LookupMessage(MSI_BARGAINSALE_TOOLWND_CALL), limit).c_str(), 0, true, false, 0, "Message", 0x118, 0x78, 0);
        quantity = it->saleData.purchaseCount - it->quantity;
        if (quantity == 0) {
          return -1;
        }
      }

      const int total = it->quantity + quantity;
      if (total > 100) {
        g_windowMgr.Confirm(FormatOwnedString(LookupMessage(MSI_LIMIT_BUY_ITEM2), 100).c_str(), 0, true, false, 0, "Message", 0x118, 0x78, 0);
        it->quantity = 100;
      } else {
        it->quantity = total > 0 ? total : 1;
      }
      this->SendMsg(this, UM_SCROLLVERT, 0, 0, 0, 0);
    }

    if (this->parent != nullptr) {
      int price = 0;
      for (const ITEM_INFO& entry : this->m_cartItems) {
        price += entry.prices[0] * entry.quantity;
      }
      this->parent->SendMsg(this, UM_SET_CASHSHOP_TOTAL_PRICE, price, 0, 0, 0);
    }
    this->Invalidate();
    return UIWindow::SendMsg(sender, message, argument1, quantity, argument3, argument4);
  }
  case UM_DECITEM: {
    if (sender == nullptr) {
      break;
    }

    std::vector<ITEM_INFO>::iterator it = this->m_cartItems.begin();
    const std::vector<ITEM_INFO>::iterator last = this->m_cartItems.end();
    for (; it != last; ++it) {
      if (atoi(it->itemId.c_str()) == argument1) {
        break;
      }
    }
    if (it == this->m_cartItems.end()) {
      break;
    }

    this->m_cartItems.erase(it);
    if (this->parent != nullptr) {
      int price = 0;
      for (const ITEM_INFO& entry : this->m_cartItems) {
        price += entry.prices[0] * entry.quantity;
      }
      this->parent->SendMsg(this, UM_SET_CASHSHOP_TOTAL_PRICE, price, 0, 0, 0);
    }
    this->SendMsg(this, UM_SCROLLVERT, 0, 0, 0, 0);
    this->Invalidate();
    break;
  }
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int count = this->m_cartItems.size();
    if (count == 0) {
      break;
    }

    int maximum = count - this->m_visibleCount;
    if (maximum < 0) {
      maximum = 0;
    }

    UIScrollBar* scrollBar = static_cast<UIScrollBar*>(this->m_scrollBar);
    if (maximum == 0) {
      if (static_cast<int>(this->m_scrollIndex) > 0) {
        this->m_scrollIndex = 0;
        scrollBar->SetRange(maximum, this->m_visibleCount, this->m_cartItems.size());
        scrollBar->SetPosition(this->m_scrollIndex);
      }
      break;
    }

    const int index = argument1 + this->m_scrollIndex;
    if (index < 0) {
      if (index <= maximum) {
        this->Invalidate();
        break;
      }
      this->m_scrollIndex = maximum;
    } else if (index > maximum) {
      this->m_scrollIndex = maximum;
    } else {
      this->m_scrollIndex = index;
    }

    scrollBar->SetRange(maximum, this->m_visibleCount, this->m_cartItems.size());
    scrollBar->SetPosition(this->m_scrollIndex);
    this->Invalidate();
    break;
  }
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007D3430
// FUNCTION: fn_09561
int UICashShopWnd_CartItem::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_COMMAND:
    switch (argument1) {
    case ID_DELETE:
      if (this->parent != nullptr) {
        this->parent->SendMsg(this, UM_DECITEM, atoi(this->m_item.itemId.c_str()), 0, 0, 0);
        this->Invalidate();
      }
      break;
    case ID_UP:
      if (this->parent != nullptr) {
        this->parent->SendMsg(this, UM_ADDITEM, reinterpret_cast<std::intptr_t>(&this->m_item), 1, 0, 0);
      }
      this->Invalidate();
      break;
    case ID_DOWN:
      if (this->parent != nullptr) {
        this->parent->SendMsg(this, UM_ADDITEM, reinterpret_cast<std::intptr_t>(&this->m_item), -1, 0, 0);
      }
      this->Invalidate();
      break;
    }
    break;
  case UM_REFRESHITEM:
    this->m_item = *reinterpret_cast<const ITEM_INFO*>(argument1);
    this->Show(1);
    this->Invalidate();
    break;
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007D3540
// FUNCTION: fn_09562
int UICashShopWnd_Item::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_COMMAND:
    if (argument1 == 0xE0 && this->parent != nullptr) {
      this->parent->SendMsg(this, UM_ADDITEM, reinterpret_cast<std::intptr_t>(&this->m_item), 0, 0, 0);
    }
    break;
  case UM_REFRESHITEM:
    this->m_item = *reinterpret_cast<const ITEM_INFO*>(argument1);
    this->Show(1);
    {
      auto* addButton = this->m_addButton;
      addButton->SetDisabled(0);
      addButton->Invalidate();
    }
    this->m_soldOut->Show(0);
    this->m_largeLayout = argument2 == 9;
    if (argument2 == 9) {
      if (static_cast<int>(this->m_item.saleData.purchaseCount) <= 0) {
        this->m_addButton->Move(0x53, 0x78);
        {
          auto* addButton = this->m_addButton;
          addButton->SetDisabled(1);
          addButton->Invalidate();
        }
        this->m_addButton->SetText(LookupMessage(MSI_ACCOUNT_LIMITED_SALE_ITEM_DONT_MOVE_CART));
        this->m_addButton->AddTextStyle(0, 0xC, 0x313131, 1, 1, -1000, -1000);
        this->m_addButton->AddTextStyle(1, 0xC, 0x313131, 1, 1, -1000, -1000);
        this->m_addButton->AddTextStyle(2, 0xC, 0x313131, 1, 1, -1000, -1000);
        this->m_soldOut->Show(1);
      } else {
        this->m_addButton->Move(0x53, 0x78);
        this->m_addButton->SetText(LookupMessage(MSI_BUY));
        this->m_addButton->AddTextStyle(0, 0xC, 0, 0, 1, -1000, -1000);
        this->m_addButton->AddTextStyle(1, 0xC, 0, 0, 1, -1000, -1000);
        this->m_addButton->AddTextStyle(2, 0xC, 0, 0, 1, -1000, -1000);
      }
    } else {
      this->m_addButton->Move(0x53, 0x55);
      this->m_addButton->SetText(LookupMessage(MSI_BUY));
      this->m_addButton->AddTextStyle(0, 0xC, 0, 0, 1, -1000, -1000);
      this->m_addButton->AddTextStyle(1, 0xC, 0, 0, 1, -1000, -1000);
      this->m_addButton->AddTextStyle(2, 0xC, 0, 0, 1, -1000, -1000);
    }

    this->Invalidate();
    break;
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007D37C0
// FUNCTION: fn_09563
void UICashShopWnd::LayoutPage() {
  for (unsigned int window = 0; window < this->m_itemWindows.size(); ++window) {
    this->m_itemWindows[window]->Show(0);
  }

  const int first = this->m_page * this->m_itemsPerPage;
  std::list<ITEM_INFO>::iterator item = this->m_items.begin();
  int index = 0;
  for (; index < first; ++index) {
    ++item;
  }

  for (int slot = 0; slot < this->m_itemsPerPage; ++slot, ++item, ++index) {
    if (index >= this->m_items.size() || slot >= this->m_itemWindows.size()) {
      break;
    }

    if (this->m_selectedTab == 9) {
      this->m_itemWindows[slot]->Resize(0xAC, 0xBD);
      this->m_itemWindows[slot]->Move(slot % this->m_columns * 0xAF + 8, slot / this->m_columns * 0xC0 + 0x6B);
    } else {
      this->m_itemWindows[slot]->Resize(0xAC, 0x7E);
      this->m_itemWindows[slot]->Move(slot % this->m_columns * 0xAF + 8, slot / this->m_columns * 0x7F + 0x6B);
    }

    this->m_itemWindows[slot]->SendMsg(this, UM_REFRESHITEM, reinterpret_cast<std::intptr_t>(&*item), this->m_selectedTab, 0, 0);
  }

  this->Invalidate();
}
