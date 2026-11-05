#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "UI/Item/UIParaItemShopWnd.h"

#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "Client/Mode/CLoginMode.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Dialogs/UIPasswordWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/GroupedInteger.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>



namespace {
#pragma pack(push, 1)
struct ItemListChunkHeader {
  /* 0000 */ std::int16_t type;
  /* 0002 */ std::uint32_t size;
};
#pragma pack(pop)

static_assert(sizeof(ItemListChunkHeader) == 6);
}

// OFFSET: 0x009A7D30
// FUNCTION: fn_13739
UIParaItemPurchaseWnd::UIParaItemPurchaseWnd() {
}

// OFFSET: 0x009A7D90
// FUNCTION: fn_13740
UIParaItemShopWnd::UIParaItemShopWnd() {
}

// OFFSET: 0x009A7DF0
// FUNCTION: fn_13741
UIParaResultWnd::UIParaResultWnd() : UIItemBaseWnd() {
  this->title = nullptr;
  this->total = nullptr;
  this->unknown_00F8 = -1;
  this->showQuantity = 0;
  this->unknown_00B8 = 1;
  this->unknown_00FC = 29;
  this->unknown_0100 = 0;
}

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
  if (drag->source == 3) {
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
      g_windowMgr.ShowWindowById(15, 1);
      dynamic_cast<UIItemDropCntWnd*>(g_windowMgr.LookupWindow(15))->SendMsg(nullptr, 13, 21, 0, 0, 0);
      return;
    }
  }

  mode->SendMsg(18, 0, 0, 0, 0);
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

  g_windowMgr.ShowWindowById(15, 1);
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
    if (item.unknown_0014[0] == item.unknown_0014[1]) {
      const char* amount = FormatGroupedInteger(static_cast<int>(item.unknown_0014[1]), price, sizeof(price));
      text = FormatOwnedString("%s %s Zeny", item.DisplayName(0).c_str(), amount);
    } else {
      const char* amount = FormatGroupedInteger(static_cast<int>(item.unknown_0014[1]), price, sizeof(price));
      text = FormatOwnedString("%s %d -> %s Zeny", item.DisplayName(0).c_str(), item.unknown_0014[0], amount);
    }

    g_modeMgr.gameMode->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
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
  this->total = new UIStaticText();
  this->total->SetSize(160, 15);
  this->total->Move(12, this->geometry.height - 21);
  this->AddChild(this->total);

  this->title = new UIStaticText();
  this->title->SetSize(70, 15);
  this->title->SetText(LookupMessage(2530), 0);
  this->title->Move(16, 2);
  this->title->shadow = 1;
  this->AddChild(this->title);

  UIResizer* resizer = new UIResizer();
  this->resizer = resizer;
  resizer->SetSize(16, 16);
  resizer->SetDirection(7);
  resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(resizer);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - this->unknown_0100 - this->unknown_00FC - 17);
  this->AddChild(scrollBar);

  {
    const std::string prefix = "유저인터페이스\\";
    const int x = this->geometry.width - 61;
    const int y = this->geometry.height - 24;
    UIBitmapButton* button = new UIBitmapButton();
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

// OFFSET: 0x009A9AB0
// FUNCTION: fn_13757
int UIParaItemPurchaseWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 36:
    this->title->SetTextAutoWidth("Total : 0 Zeny");
    return 0;
  case 34:
    this->saved = reinterpret_cast<ItemPurchaseWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->SendMsg(nullptr, 14, 3, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case 38: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    ModeDragState& drag = mode->drag;
    switch (drag.type) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15: {
      const int count = drag.unknown_03B6 == 1 ? g_session.GetItem1720Quantity(drag.id) : g_session.GetItem1720QuantityByItemId(drag.name.c_str());
      if (count > 0) {
        mode->SendMsg(18, 0, 0, 0, 0);
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(119)), 255, 0, 0, 0);
        return 0;
      }
      break;
    }
    }

    ITEM_INFO item;
    item.Assign(reinterpret_cast<const RawItemData&>(drag));
    const int total = g_session.GetItem1720Total();
    const int price = drag.unknown_0314;
    const bool affordable = g_session.m_zeny >= static_cast<long long>(price) * drag.quantity + total;

    int existing = 0;
    for (const ITEM_INFO& value : this->items) {
      if (value.itemId == item.itemId) {
        existing = value.quantity;
        break;
      }
    }

    const int combined = existing + item.quantity;
    if (combined > 30000) {
      item.quantity += 30000 - combined;
      char text[200];
      std::memset(text, 0, sizeof(text));
      sprintf_s(text, sizeof(text), LookupMessage(1704), 30000);
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 255, 0, 0, 0);
    }

    const int available = item.unknown_0074[3] == 1 ? g_session.GetItem1718QuantityOf(item) : g_session.GetItem1718QuantityByItemId(item);
    if (available != -1 && available < a) {
      item.quantity = available;
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2531)), 255, 0, 0, 0);
    }

    if (item.quantity > 0) {
      std::uint16_t weight;
      if (item.unknown_0074[3] == 1) {
        const std::uint16_t added = g_session.GetItem1718Unknown006COf(item) * static_cast<std::uint16_t>(item.quantity);
        weight = added + g_session.SumItem1720Unknown006C();
      } else {
        const std::uint16_t added = g_session.GetItem1718Unknown006CByItemId(item) * static_cast<std::uint16_t>(item.quantity);
        weight = added + g_session.SumItem1720Unknown006CByItemId();
      }

      if (!affordable) {
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(55)), 255, 0, 0, 0);
      } else if (weight + g_session.m_weight > g_session.m_maxWeight) {
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2536)), 255, 0, 0, 0);
      } else {
        if (item.unknown_0074[3] == 1) {
          g_session.RemoveItem1718(item);
          g_session.MergeItem1720(item);
        } else {
          g_session.RemoveItem1718ByItemId(item);
          g_session.MergeItem1720ByName(item);
        }

        this->SendMsg(nullptr, 23, 0, 0, 0, 0);
        dynamic_cast<UIParaItemShopWnd*>(g_windowMgr.LookupWindow(254))->SendMsg(nullptr, 23, 0, 0, 0, 0);
      }
    }

    mode->SendMsg(18, 0, 0, 0, 0);
    return 0;
  }
  case 6: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    CMode* mode = g_modeMgr.gameMode;
    switch (a) {
    case 184: {
      ITEM_INFO item = g_session.GetItem1720At(0);
      if (item.unknown_0074[3] == 1) {
        mode->SendMsg(263, 0, 0, 0, 0);
      } else {
        mode->SendMsg(262, 0, 0, 0, 0);
      }
      g_session.ClearTradeItems();
      return 0;
    }
    case 185:
      mode->SendMsg(292, 0, 0, 0, 0);
      mode->SendMsg(40, 0, 0, 0, 0);
      g_session.ClearTradeItems();
      return 0;
    }
    break;
  }
  case 14:
    if (a == 3) {
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
        this->visibleRows = (this->geometry.height - 38) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 38);
        this->UpdateScrollBar();
      }

      this->title->Move(12, this->geometry.height - 20);
      const int positions[2][2] = {{this->geometry.width - 106, this->geometry.height - 24}, {this->geometry.width - 61, this->geometry.height - 24}};
      for (int i = 0; i < 2; ++i) {
        this->buttons[i]->Move(positions[i][0], positions[i][1]);
      }

      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case 23: {
    this->items.erase(this->items.begin(), this->items.end());
    int sum = 0;
    const int size = g_session.GetList2Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1720At(i);
      if (item.unknown_0000[1] != 0) {
        sum += item.quantity * item.unknown_0014[1];
        this->items.push_back(item);
      }
    }

    char text[80];
    char amount[64];
    std::memset(text, 0, sizeof(text));
    std::memset(amount, 0, sizeof(amount));
    sprintf_s(text, sizeof(text), "Total : %s Zeny", FormatGroupedInteger(sum, amount, sizeof(amount)));
    this->title->SetTextAutoWidth(text);
    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }
  case 123: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    ItemListChunkHeader header = *reinterpret_cast<const ItemListChunkHeader*>(bytes);
    unsigned offset = 6;
    if (header.type == 20700) {
      while (static_cast<unsigned>(b) > offset) {
        header = *reinterpret_cast<const ItemListChunkHeader*>(bytes + offset);
        offset += 6;
        if (header.type == 20701) {
          UIWindow* window = g_windowMgr.EnsureWindow(22);
          if (window) {
            window->SendMsg(nullptr, 36, 0, 0, 0, 0);
            window->SendMsg(nullptr, 23, 0, 0, 0, 0);
          }

          this->SendMsg(nullptr, 36, 0, 0, 0, 0);
          this->SendMsg(nullptr, 23, 0, 0, 0, 0);
          break;
        }

        offset += header.size;
      }
    }
    break;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x009AA4F0
// FUNCTION: fn_13758
int UIParaItemShopWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 34:
    this->saved = reinterpret_cast<ItemShopWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->SendMsg(nullptr, 14, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case 6:
    this->RecordResponse(static_cast<int>(a), b, c, d);
    if (a == 213) {
      if (g_replayAnimationScaling != 0) {
        this->checkBox->geometry.unknown_0030 = b;
        this->checkBox->Invalidate();
      }

      g_windowMgr.unknown_0064 = b;
      return 0;
    }
    break;
  case 38:
    if (a == 0) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      ITEM_INFO item;
      item.Assign(reinterpret_cast<const RawItemData&>(mode->drag));
      CSession* lists = &g_session;
      if (item.unknown_0074[3] == 1) {
        lists->RemoveItem1720(item);
        lists->MergeItem1718(item);
      } else {
        lists->RemoveItem1720ByName(item);
        lists->MergeItem1718ByName(item);
      }

      dynamic_cast<UIParaItemPurchaseWnd*>(g_windowMgr.LookupWindow(255))->SendMsg(nullptr, 23, 0, 0, 0, 0);
      this->SendMsg(nullptr, 23, 0, 0, 0, 0);
      mode->SendMsg(18, 0, 0, 0, 0);
      return 0;
    }
    break;
  case 36:
    if (a == 1) {
      this->unknown_00F8 = 1;
      this->showQuantity = 1;
      this->title->SetTextAutoWidth(LookupMessage(185));
      this->checkBox->m_disabled = 0;
    } else {
      this->unknown_00F8 = 0;
      this->showQuantity = 0;
      this->title->SetTextAutoWidth(LookupMessage(186));
      this->checkBox->m_disabled = 1;
    }
    return 0;
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
        this->visibleRows = (newHeight - this->unknown_0108 - this->unknown_0104 - 17) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - this->unknown_0108 - this->unknown_0104 - 17);
        this->checkBox->Move(12, this->geometry.height - 20);
        this->UpdateScrollBar();
      }

      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case 23: {
    this->items.erase(this->items.begin(), this->items.end());
    const int size = g_session.GetList1Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1718At(i);
      if (item.unknown_0000[1] != 0) {
        this->items.push_back(item);
      }
    }

    this->Invalidate();
    this->UpdateScrollBar();
    break;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x009AA990
// FUNCTION: fn_13759
int UIParaResultWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    CMode* mode = g_modeMgr.gameMode;
    if (a == 184) {
      mode->SendMsg(292, 0, 0, 0, 0);
      mode->SendMsg(40, 0, 0, 0, 0);
      g_session.ClearTradeItems();
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
      g_windowMgr.StoreSnapRectangle(this);
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

// OFFSET: 0x009A8820
// FUNCTION: fn_13751
void UIParaItemShopWnd::Paint() {
  const int titleHeight = this->DrawTitleBar(false, "", 0);
  if (titleHeight == this->geometry.height) {
    return;
  }

  const int barHeight = this->DrawButtonBar(this->unknown_00B8 == 1 ? 2 : 0, 0, 1);
  unsigned red = 0xFF, green = 0xFF, blue = 0xFF;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  const int height = this->geometry.height - barHeight - titleHeight;
  this->FillRegion(0, titleHeight, this->geometry.width, height, RGB(red, green, blue));

  const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
  if (left) {
    for (int i = 0; i < height / left->image.height; ++i) {
      this->DrawBitmap(0, left->image.height * i + titleHeight, &left->image, false);
    }
    if (height % left->image.height > 0) {
      this->DrawBitmap(0, this->geometry.height - barHeight - left->image.height, &left->image, false);
    }
  }

  const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
  if (right) {
    for (int i = 0; i < height / right->image.height; ++i) {
      this->DrawBitmap(this->geometry.width - right->image.width, right->image.height * i + titleHeight, &right->image, false);
    }
    if (height % right->image.height > 0) {
      this->DrawBitmap(this->geometry.width - right->image.width, this->geometry.height - barHeight - right->image.height, &right->image, false);
    }
  }

  const int footer = this->unknown_010C == 1 ? this->unknown_0108 : 0;
  int slots = (height - footer) / 32;
  for (int y = titleHeight; slots > 0; --slots, y += 32) {
    DrawItemSlotBackground(*this, 30, y);
  }

  int index = 0;
  int visible = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) == 1 && it->unknown_005D == 1) {
      continue;
    }

    const int top = visible * 32 + titleHeight;
    if (this->hovered == index) {
      unsigned hoverRed = 0xFF, hoverGreen = 0xFF, hoverBlue = 0xFF;
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(72, top + 2, 191, 28, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    if (this->unknown_00F8 == 1 || it->quantity != 0) {
      DrawItemWithQuantity(*this, ResourceMgr::Instance(), 34, top + 4, *it, 0, 0);
    } else {
      DrawItemWithQuantityAuxiliary(*this, 1, 34, top + 4, *it, 0, 0);
    }

    this->DrawWrappedItemName(70, top + 12, 120, &*it, 0, 0, 12);

    if (it->unknown_0088 > 0) {
      this->DrawBitmap(
        34, top + 16,
        &static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->unknown_0088).c_str()))
        )->image,
        true
      );
    }

    std::string currency = "Z";
    if (this->unknown_010C == 1) {
      if (g_serviceType == 6) {
        currency = "K";
      } else if (g_serviceType == 0) {
        if (g_session.m_channelCopId == 1) {
          currency = "H";
        } else {
          currency = "C";
        }
      } else {
        currency = "P";
      }
    }

    char price[64];
    std::memset(price, 0, sizeof(price));
    FormatGroupedInteger(static_cast<int>(it->unknown_0014[1]), price, sizeof(price));
    std::string text = FormatOwnedString(" %s %s", price, currency.c_str());
    if (it->unknown_0014[0] != it->unknown_0014[1]) {
      text = FormatOwnedString(" %d -> %s %s", it->unknown_0014[0], price, currency.c_str());
    }

    this->DrawRightAlignedText(this->geometry.width - 18, top + 12, text.c_str(), text.size(), 0, 12, 0, 0);

    if (++visible == this->visibleRows) {
      break;
    }
  }

  if (this->unknown_010C == 1) {
    std::string text;
    if (g_serviceType == 0 && g_session.m_channelCopId == 1) {
      text = FormatOwnedString(LookupMessage(1290), this->ClipTextToWidth(g_session.DecodePlayerName(), 100, 0, 12, 0), g_pendingAid);
    } else {
      text = FormatOwnedString(LookupMessage(1210), this->ClipTextToWidth(g_session.DecodePlayerName(), 100, 0, 12, 0), g_pendingAid);
    }
    this->DrawTextAt(12, this->geometry.height - 21, text.c_str(), text.size(), 0, 12, 0, 0, 0);

    text = FormatOwnedString(LookupMessage(1305), this->ClipTextToWidth(g_session.DecodePlayerName(), 100, 0, 12, 0), g_pendingGid);
    this->DrawTextAt(12, this->geometry.height - barHeight - 21, text.c_str(), text.size(), 0, 12, 0, 0, 0);
  }
}

// OFFSET: 0x009A90E0
// FUNCTION: fn_13752
void UIParaResultWnd::Paint() {
  this->DrawTitleBar(false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  int rows = (this->geometry.height - this->unknown_00FC - 17) / 8 + ((this->geometry.height - this->unknown_00FC - 17) % 8 != 0);
  for (int y = 17; rows > 0; --rows, y += 8) {
    const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, &left->image, false);
  }

  rows = (this->geometry.height - this->unknown_00FC - 17) / 8 + ((this->geometry.height - this->unknown_00FC - 17) % 8 != 0);
  for (int y = 17; rows > 0; --rows, y += 8) {
    const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, &right->image, false);
  }

  {
    unsigned red = 0, green = 0, blue = 0;
    SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
    const int listHeight = this->geometry.height - this->unknown_00FC - 17;
    this->FillRegion(20, 17, (this->geometry.width - 60) % 32, listHeight, RGB(red, green, blue));
    this->FillRegion(20, listHeight / 32 * 32 + 17, this->geometry.width - 24, listHeight % 32, RGB(red, green, blue));
  }
  this->DrawButtonBar(this->unknown_00B8 == 1 ? 2 : 0, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const int slots = (this->geometry.height - this->unknown_0100 - this->unknown_00FC - 17) / 32;
  for (int slot = slots, y = 17; slot > 0; --slot, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  {
    unsigned red = 0, green = 0, blue = 0;
    SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
    this->FillRegion(72, 17, columns * 32 - 32, slots * 32, RGB(red, green, blue));
  }

  int index = 0;
  int visible = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) == 1 && it->unknown_005D) {
      continue;
    }

    const int column = visible % this->columns * 32;
    const int top = visible / this->columns * 32;
    if (this->unknown_00F8 == index) {
      unsigned hoverRed = 0, hoverGreen = 0, hoverBlue = 0;
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(column + 72, top + 19, 191, 28, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    const int x = column + 44;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);
    this->DrawWrappedItemName(column + 76, top + 29, 110, &*it, 0, 0, 12);

    if (it->unknown_0088 > 0) {
      this->DrawBitmap(
        x, top + 33,
        &static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->unknown_0088).c_str()))
        )->image,
        true
      );
    }

    char text[64];
    char price[64];
    std::memset(text, 0, sizeof(text));
    std::memset(price, 0, sizeof(price));
    int length;
    if (it->unknown_0014[0] != it->unknown_0014[1]) {
      length = sprintf(text, " %d -> %s Z", it->unknown_0014[0], FormatGroupedInteger(static_cast<int>(it->unknown_0014[1]), price, 64));
    } else {
      length = sprintf(text, " %s Z", FormatGroupedInteger(static_cast<int>(it->unknown_0014[0]), price, 64));
    }
    this->DrawRightAlignedText(this->geometry.width - 18, top + 29, text, length, 0, 12, 0, 0);

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }
}
