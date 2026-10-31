#include "UI/Item/UIParaItemShopWnd.h"

#include "Client/Input/InputCharset.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/ReplayPacketRecorder.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Render/Renderer.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Dialogs/UIPasswordWnd.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/GroupedInteger.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

struct SessionItemLists {
  void ClearTradeItems();
};

// OFFSET: 0x009A7D30
// FUNCTION: fn_13739
UIParaItemPurchaseWnd::UIParaItemPurchaseWnd() {
}

// OFFSET: 0x009A7D90
// FUNCTION: fn_13740
UIParaItemShopWnd::UIParaItemShopWnd() {
}

// OFFSET: 0x009A7DF0

// OFFSET: 0x009A80F0
// FUNCTION: fn_13745
void UIParaResultWnd::AddItem(const RawItemBasic& item, int quantity) {
  ITEM_INFO info;
  info.AssignBasic(item);
  info.quantity = quantity;
  info.unknown_0060 = 0;
  info.unknown_005D = 0;
  info.SetItemId(atoi(info.itemId.c_str()));
  this->items.push_back(info);
}

// OFFSET: 0x009A81E0
// FUNCTION: fn_13746
void UIParaResultWnd::AddItem(const ITEM_INFO& item) {
  this->items.push_back(item);
}

// OFFSET: 0x009A81F0
// FUNCTION: fn_13747
void UIParaItemPurchaseWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 3) {
    mode->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  switch (drag->type) {
  case 4:
  case 5:
  case 8:
  case 9:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  default:
    reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
    dynamic_cast<UIItemDropCntWnd*>(g_windowMgr.LookupWindow(15))->SendMsg(nullptr, 13, 21, 0, 0, 0);
    return;
  }
}

// OFFSET: 0x009A82E0
// FUNCTION: fn_13748
void UIParaItemShopWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 4) {
    g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  }

  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  dynamic_cast<UIItemDropCntWnd*>(g_windowMgr.LookupWindow(15))->SendMsg(nullptr, 13, 22, 0, 0, 0);
}

// OFFSET: 0x009A83B0
// FUNCTION: fn_13749
void UIParaResultWnd::Slot10(int x, int y, std::intptr_t value) {
}

// OFFSET: 0x009A96C0
// FUNCTION: fn_13754
void UIParaItemPurchaseWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  int previousHovered = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);
  if (previousHovered != this->hovered) {
    this->Invalidate();
  }
}

// OFFSET: 0x009A9710
// FUNCTION: fn_13755
void UIParaResultWnd::Slot28(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.unknown_0000[1] != 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    std::string text;
    char price[64];
    std::memset(price, 0, sizeof(price));
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    if (item.unknown_0014[0] == item.unknown_0014[1]) {
      const char* amount = FormatGroupedInteger(static_cast<int>(item.unknown_0014[1]), price, sizeof(price));
      text = FormatOwnedString("%s %s Zeny", naming.DisplayName(item, 0).c_str(), amount);
    } else {
      const char* amount = FormatGroupedInteger(static_cast<int>(item.unknown_0014[1]), price, sizeof(price));
      text = FormatOwnedString("%s %d -> %s Zeny", naming.DisplayName(item, 0).c_str(), item.unknown_0014[0], amount);
    }

    reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)->Show(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
  }
}

// OFFSET: 0x009A99C0
// FUNCTION: fn_13756
void UIParaItemShopWnd::Slot32(int x, int y) {
  const int previousHovered = this->unknown_00F8;
  this->unknown_00F8 = this->GetItemIndexAt(x, y);
  if (previousHovered != this->unknown_00F8) {
    this->Invalidate();
  }

  ITEM_INFO item = this->GetItemByIndex(this->unknown_00F8);
  if (item.unknown_0000[1] != 0) {
    g_modeMgr.GetGameMode();
    UIWindow* window = g_windowMgr.EnsureWindow(0xC);
    window->SendMsg(nullptr, 0x18, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x009A96B0
// FUNCTION: fn_13753
void __stdcall Nullsub344(int, int) {
}

// OFFSET: 0x009A83C0
// FUNCTION: fn_13750
void UIParaResultWnd::Slot15(int width, int height) {
  this->total = new UIStaticText(*this->m_context, *this->m_controls);
  this->total->SetSize(160, 15);
  this->total->Move(12, this->geometry.height - 21);
  this->AddChild(this->total);

  this->title = new UIStaticText(*this->m_context, *this->m_controls);
  this->title->SetSize(70, 15);
  this->title->SetText(LookupMessage(2530), 0);
  this->title->Move(16, 2);
  this->title->shadow = 1;
  this->AddChild(this->title);

  UIResizer* resizer = new UIResizer(*this->m_context);
  this->resizer = resizer;
  resizer->SetSize(16, 16);
  resizer->SetDirection(7);
  resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(resizer);

  UIScrollBar* scrollBar = new UIScrollBar(*this->m_context);
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - this->unknown_0100 - this->unknown_00FC - 17);
  this->AddChild(scrollBar);

  {
    const std::string prefix = "유저인터페이스\\";
    const int x = this->geometry.width - 61;
    const int y = this->geometry.height - 24;
    UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->buttons[0] = button;
    button->SetBitmap((prefix + "btn_ok.bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + "btn_ok_a.bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + "btn_ok_b.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(x, y);
    button->SetId(184);
    this->AddChild(button);
  }

  this->visibleRows = (height - this->unknown_0100 - this->unknown_00FC - 17) / 32;
  this->columns = 1;
  this->SendMsg(nullptr, 23, 0, 0, 0, 0);
}

// OFFSET: 0x009AA990
// FUNCTION: fn_13759
int UIParaResultWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6: {
    this->m_recorder->Record(this->geometry.id, static_cast<int>(a), b, c, d);
    CMode* mode = g_modeMgr.gameMode;
    if (a == 184) {
      mode->SendMsg(292, 0, 0, 0, 0);
      mode->SendMsg(40, 0, 0, 0, 0);
      reinterpret_cast<SessionItemLists*>(&g_session)->ClearTradeItems();
      return 0;
    }
    break;
  }
  case 14:
    if (a == 7) {
      int width = b <= 320 ? b : 320;
      int height = c <= 336 ? c : 336;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }

      const int newWidth = (width - 280) / 32 * 32 + 280;
      const int newHeight = (height - 120) / 32 * 32 + 120;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->Resize(newWidth, newHeight);
        this->columns = 1;
        this->visibleRows = (newHeight - this->unknown_0100 - this->unknown_00FC - 17) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - this->unknown_0100 - this->unknown_00FC - 17);
        this->UpdateScrollBar();
      }

      this->total->Move(12, this->geometry.height - 20);
      this->buttons[0]->Move(this->geometry.width - 61, this->geometry.height - 24);
      this->Move(g_renderer->viewport.width / 2 - this->geometry.width / 2, g_renderer->viewport.height / 2 - this->geometry.height / 2);
      this->m_frameContext->snapping.Store(this);
      return 0;
    }
    break;
  case 23: {
    int sum = 0;
    for (const ITEM_INFO& item : this->items) {
      sum += item.unknown_0014[1] * item.quantity;
    }

    char text[80];
    char amount[64];
    std::memset(text, 0, sizeof(text));
    std::memset(amount, 0, sizeof(amount));
    sprintf_s(text, sizeof(text), "Total : %s Zeny", FormatGroupedInteger(sum, amount, sizeof(amount)));
    this->total->SetTextAutoWidth(text);
    this->Invalidate();
    this->UpdateScrollBar();
    break;
  }
  case 34:
    this->total->SetTextAutoWidth("Total : 0 Zeny");
    return 0;
  case 36:
    this->showQuantity = 0;
    this->title->SetTextAutoWidth(LookupMessage(2530));
    this->SendMsg(nullptr, 14, 7, this->geometry.width, a, 0);
    return 0;
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}
