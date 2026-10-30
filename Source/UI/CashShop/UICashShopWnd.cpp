#include "UI/CashShop/UICashShopWnd.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeCursor.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/UIStaticBitmap.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <shellapi.h>
#include <timeapi.h>

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  /* 0000 */ std::uint8_t m_text[Length];
  bool m_encrypted;
};

struct CashShopReplaceCartItemText {
  EncryptedName<85, 0x3B915119EFD523EFULL>& operator()() const;
};

// OFFSET: 0x007CD5F0
// FUNCTION: fn_09511
UICashShopWnd::UICashShopWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_banner(nullptr), m_secondBanner(nullptr), m_selectedTab(0), m_itemsPerPage(9), m_columns(3), m_page(0),
      m_pageCount(0), unknown_00E0(nullptr), unknown_00E8(0), unknown_00EC(0), unknown_00F0(0), unknown_00F4(nullptr), m_cart(nullptr) {
}

// OFFSET: 0x007CE110
// FUNCTION: fn_09523
EncryptedName<85, 0x3B915119EFD523EFULL>& CashShopReplaceCartItemText::operator()() const {
  const std::uint8_t source[] = {0x5E, 0x9B, 0x15, 0x3B, 0x39, 0xE9, 0x3F, 0x86, 0x55, 0xE5, 0x7B, 0x50, 0xB8, 0xE5, 0x53, 0x1B, 0x28, 0xF2, 0xF5, 0x2E,
                                 0xA7, 0xE6, 0x68, 0xFB, 0x28, 0x03, 0x6B, 0x29, 0xD9, 0x9D, 0x54, 0xE0, 0x57, 0x9B, 0xF5, 0x5B, 0xFA, 0x91, 0x2A, 0x1B,
                                 0x53, 0xD5, 0xF5, 0x2F, 0xCF, 0xEC, 0x51, 0x8F, 0x20, 0x97, 0x0C, 0xC1, 0x39, 0x0A, 0xB4, 0x48, 0xB2, 0x03, 0x6E, 0x1C,
                                 0xDE, 0xE1, 0x51, 0x83, 0x58, 0xED, 0xF5, 0x5E, 0xAA, 0x92, 0x2D, 0xFC, 0x20, 0x9E, 0x16, 0x5F, 0xC3, 0xEC, 0x51, 0x8F,
                                 0x20, 0x92, 0x3B, 0xD0, 0x19};
  thread_local EncryptedName<sizeof(source), 0x3B915119EFD523EFULL> name(source);
  return name;
}

// OFFSET: 0x007CE570
// FUNCTION: fn_09529
void UICashShopWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIFrameWnd::Slot10(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  if (mode == nullptr) {
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 4) {
    mode->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  if (drag->quantity <= 1) {
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  }

  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  if (g_windowMgr.window_01EC != nullptr) {
    g_windowMgr.window_01EC->SendMsg(nullptr, 13, 20, drag->quantity, 0, 0);
  }
}

// OFFSET: 0x007CE630
// FUNCTION: fn_09530
void UICashShopWnd_Cart::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  if (mode == nullptr) {
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 6) {
    mode->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  if (g_windowMgr.window_01EC != nullptr) {
    g_windowMgr.window_01EC->SendMsg(nullptr, 13, 19, 0, 0, 0);
  }
}

// OFFSET: 0x007CE6C0
// FUNCTION: fn_09531
void UICashShopWnd_CartItem::Slot10(int x, int y, std::intptr_t value) {
  if (this->parent != nullptr) {
    this->parent->Slot10(x, y, value);
  }
}

// OFFSET: 0x007CE6E0
// FUNCTION: fn_09532
void UICashShopWnd_Item::Slot10(int x, int y, std::intptr_t value) {
  if (this->parent != nullptr) {
    this->parent->Slot10(x, y, value);
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
bool UICashShopWnd::Slot03() {
  return true;
}

// OFFSET: 0x007CFB20
// FUNCTION: fn_09537
void UICashShopWnd_CartItem::Slot15(int, int) {
  UIBitmapButton* close = new UIBitmapButton(*this->m_context, *this->m_controls);
  close->SetBitmap("유저인터페이스\\cashshop\\btn_close.bmp", 0, 0);
  close->SetBitmap("유저인터페이스\\cashshop\\btn_close.bmp", 1, 0);
  close->SetBitmap("유저인터페이스\\cashshop\\btn_close.bmp", 2, 0);
  close->SetSize(close->BitmapWidth(), close->BitmapHeight());
  close->Move(0xA1, 2);
  close->SetId(0xD3);
  this->AddChild(close);

  UIBitmapButton* plus = new UIBitmapButton(*this->m_context, *this->m_controls);
  plus->SetBitmap("유저인터페이스\\cashshop\\btn_plus_normal.bmp", 0, 0);
  plus->SetBitmap("유저인터페이스\\cashshop\\btn_plus_over.bmp", 1, 0);
  plus->SetBitmap("유저인터페이스\\cashshop\\btn_plus_press.bmp", 2, 0);
  plus->SetSize(plus->BitmapWidth(), plus->BitmapHeight());
  plus->Move(0x52, 0x1D);
  plus->SetId(0x117);
  this->AddChild(plus);

  UIBitmapButton* minus = new UIBitmapButton(*this->m_context, *this->m_controls);
  minus->SetBitmap("유저인터페이스\\cashshop\\btn_minus_normal.bmp", 0, 0);
  minus->SetBitmap("유저인터페이스\\cashshop\\btn_minus_over.bmp", 1, 0);
  minus->SetBitmap("유저인터페이스\\cashshop\\btn_minus_press.bmp", 2, 0);
  minus->SetSize(minus->BitmapWidth(), minus->BitmapHeight());
  minus->Move(0x52, 0x26);
  minus->SetId(0x118);
  this->AddChild(minus);
}

// OFFSET: 0x007CFD20
// FUNCTION: fn_09538
void UICashShopWnd_Item::Slot15(int, int) {
  this->m_addButton = new UIBmpTextButton(*this->m_context, *this->m_controls);
  this->m_addButton->SetBitmap("유저인터페이스\\cashshop\\btn_add_normal.bmp", 0, 0);
  this->m_addButton->SetBitmap("유저인터페이스\\cashshop\\btn_add_press.bmp", 1, 0);
  this->m_addButton->SetBitmap("유저인터페이스\\cashshop\\btn_add_press.bmp", 2, 0);
  this->m_addButton->SetBitmap("유저인터페이스\\cashshop\\btn_add_disable.bmp", 3, 0);
  this->m_addButton->SetSize(this->m_addButton->BitmapWidth(), this->m_addButton->BitmapHeight());
  this->m_addButton->SetText(LookupMessage(0x6F3));
  this->m_addButton->Move(0x53, 0x55);
  this->m_addButton->SetId(0xE0);
  this->AddChild(this->m_addButton);

  this->m_soldOut = new UIStaticBitmap(*this->m_context, *this->m_controls);
  this->m_soldOut->SetBitmap("유저인터페이스\\cashshop\\img_shop_soldout.bmp");
  this->m_soldOut->SetSize(this->m_soldOut->geometry.width, this->m_soldOut->geometry.height);
  this->m_soldOut->Move(9, 9);
  this->m_soldOut->Show(0);
  this->AddChild(this->m_soldOut);
}

// OFFSET: 0x007D04F0
// FUNCTION: fn_09540
void UICashShopWnd_Banner::Paint() {
  if (this->m_entries.empty()) {
    return;
  }

  ResourceMgr& resources = ResourceMgr::Instance();
  this->DrawBitmap(
    0, 0, reinterpret_cast<const BitmapImage*>(resources.Load(Resolve(("유저인터페이스\\cashshop\\" + this->m_entries[this->m_index].image).c_str()))),
    false
  );
}

// OFFSET: 0x007D05F0
// FUNCTION: fn_09541
void UICashShopWnd_Cart::Paint() {
  ResourceMgr& resources = ResourceMgr::Instance();
  this->DrawBitmap(0, 0, reinterpret_cast<const BitmapImage*>(resources.Load(Resolve("유저인터페이스\\cashshop\\img_shop_cart_bg.bmp"))), false);

  unsigned int index = this->m_scrollIndex;
  for (int slot = 0; slot < this->m_visibleCount; ++slot, ++index) {
    UICashShopWnd_CartItem* item = this->m_slots[slot];
    if (index < this->m_cartItems.size()) {
      item->SendMsg(this, 0x17, reinterpret_cast<std::intptr_t>(&this->m_cartItems[index]), 0, 0, 0);
    } else {
      item->Show(0);
    }
  }
}

// OFFSET: 0x007D1020
// FUNCTION: fn_09544
void UICashShopWnd_CartItem::Slot26(int, int) {
  this->SendMsg(this, 6, 0xD3, 0, 0, 0);
}

// OFFSET: 0x007D10B0
// FUNCTION: fn_09547
void UICashShopWnd_Banner::Slot31(int, int) {
  if (this->m_entries.empty()) {
    return;
  }

  std::string url = this->m_entries[this->m_index].url;
  ShellExecuteA(nullptr, "OPEN", url.c_str(), url.c_str(), nullptr, SW_SHOW);
}

// OFFSET: 0x007D1230
// FUNCTION: fn_09550
void UICashShopWnd_Banner::ResetCursor(int, int) {
  this->m_context->cursor.Set(2, this->m_context->cursorTick);
}

// OFFSET: 0x007D1240
// FUNCTION: fn_09551
void UICashShopWnd::Slot19() {
  std::string search(this->unknown_00E0->Text());
  if (search.compare(LookupMessage(3302)) != 0) {
    if (search.size() == 0) {
      this->unknown_00E0->SetText(LookupMessage(3302));
      this->unknown_00E0->TextColor(0xB1B1B1, 0xFFFFFFFFu);
    } else {
      this->unknown_00E0->TextColor(0, 0xFFFFFFFFu);
    }
  }

  std::string amount(this->unknown_00F4->Text());
  if (amount.size() == 0) {
    this->unknown_00F4->SetText(IntToString(0, 0).c_str());
  }

  this->m_banner->Slot19();
  if (g_session.unknown_5912) {
    this->m_secondBanner->Slot19();
  }
}

// OFFSET: 0x007D14C0
// FUNCTION: fn_09552
void UICashShopWnd_Banner::Slot19() {
  if (!this->m_entries.empty() && this->m_lastTick + 5000 < timeGetTime()) {
    const unsigned int next = this->m_index + 1;
    this->SendMsg(this->m_buttons[next < this->m_entries.size() ? next : 0], 6, 0xD7, 0, 0, 0);
  }
}

// OFFSET: 0x007D15E0
// FUNCTION: fn_09555
void UICashShopWnd_Cart::Slot35(int delta) {
  if (delta < 0) {
    this->SendMsg(this->m_scrollBar, 7, 1, 0, 0, 0);
  } else {
    this->SendMsg(this->m_scrollBar, 7, -1, 0, 0, 0);
  }
}

// OFFSET: 0x007D1620
// FUNCTION: fn_09556
void UICashShopWnd_CartItem::Slot35(int delta) {
  if (this->parent) {
    this->parent->Slot35(delta);
  }
}

// OFFSET: 0x007D2C00
// FUNCTION: fn_09559
int UICashShopWnd_Banner::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6 && argument1 == 0xD7) {
    for (unsigned int index = 0; index < this->m_buttons.size(); ++index) {
      UIWindow* button = this->m_buttons[index];
      if (sender == button) {
        button->SendMsg(this, 13, 1, 0, 0, 0);
        this->m_index = index;
      } else {
        button->SendMsg(this, 13, 0, 0, 0, 0);
      }
    }

    this->m_lastTick = timeGetTime();
    this->Invalidate();
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007D3430
// FUNCTION: fn_09561
int UICashShopWnd_CartItem::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    switch (argument1) {
    case 0xD3:
      if (this->parent != nullptr) {
        this->parent->SendMsg(this, 0x68, atoi(this->m_item.itemId.c_str()), 0, 0, 0);
        this->Invalidate();
      }
      break;
    case 0x117:
      if (this->parent != nullptr) {
        this->parent->SendMsg(this, 0x1F, reinterpret_cast<std::intptr_t>(&this->m_item), 1, 0, 0);
      }
      this->Invalidate();
      break;
    case 0x118:
      if (this->parent != nullptr) {
        this->parent->SendMsg(this, 0x1F, reinterpret_cast<std::intptr_t>(&this->m_item), -1, 0, 0);
      }
      this->Invalidate();
      break;
    }
  } else if (message == 0x17) {
    this->m_item = *reinterpret_cast<const ITEM_INFO*>(argument1);
    this->Show(1);
    this->Invalidate();
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

    this->m_itemWindows[slot]->SendMsg(this, 0x17, reinterpret_cast<std::intptr_t>(&*item), this->m_selectedTab, 0, 0);
  }

  this->Invalidate();
}
