#include "UI/CashShop/UICashShopWnd.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>

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
