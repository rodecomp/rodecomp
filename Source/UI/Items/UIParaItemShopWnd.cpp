#include "Render/Text/FontCache.h"
#include "Contents/Skin/SkinMgr.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/Items/ItemType.h"
#include "UI/Window/UIEnums.h"
#include "Render/Text/TextContext.h"
#include "UI/Items/UIParaItemShopWnd.h"

#include "Client/Input/InputCharset.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Client/Mode/CLoginMode.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Login/UIPasswordWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/GroupedInteger.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;



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
  this->hovered = -1;
  this->showQuantity = 0;
  this->alternateButtonBar = 1;
  this->footerHeight = 29;
  this->headerHeight = 0;
}

// OFFSET: 0x009A80F0
// FUNCTION: fn_13745
void UIParaResultWnd::AddItem(const RawItemBasic& item, int quantity) {
  ITEM_INFO info;
  info.AssignBasic(item);
  info.quantity = quantity;
  info.refiningLevel = 0;
  info.damaged = 0;
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
void UIParaItemPurchaseWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 3) {
    switch (drag->type) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      return;
    default:
      g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
      dynamic_cast<UIItemDropCntWnd*>(g_windowMgr.LookupWindow(WID_ITEMDROPCNTWND))->SendMsg(nullptr, UM_SETSTATE, 21, 0, 0, 0);
      return;
    }
  }

  mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x009A82E0
// FUNCTION: fn_13748
void UIParaItemShopWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 4) {
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    return;
  }

  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
  dynamic_cast<UIItemDropCntWnd*>(g_windowMgr.LookupWindow(WID_ITEMDROPCNTWND))->SendMsg(nullptr, UM_SETSTATE, 22, 0, 0, 0);
}

// OFFSET: 0x009A83B0
// FUNCTION: fn_13749
void UIParaResultWnd::DragAndDrop(int x, int y, std::intptr_t value) {
}

// OFFSET: 0x009A96C0
// FUNCTION: fn_13754
void UIParaResultWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  int previousHovered = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);
  if (previousHovered != this->hovered) {
    this->Invalidate();
  }
}

// OFFSET: 0x009A9710
// FUNCTION: fn_13755
void UIParaResultWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] != 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    std::string text;
    char price[64];
    std::memset(price, 0, sizeof(price));
    if (item.prices[0] == item.prices[1]) {
      const char* amount = FormatGroupedInteger(static_cast<int>(item.prices[1]), price, sizeof(price));
      text = FormatOwnedString("%s %s Zeny", item.DisplayName(0).c_str(), amount);
    } else {
      const char* amount = FormatGroupedInteger(static_cast<int>(item.prices[1]), price, sizeof(price));
      text = FormatOwnedString("%s %d -> %s Zeny", item.DisplayName(0).c_str(), item.prices[0], amount);
    }

    g_modeMgr.gameMode->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
  }
}

// OFFSET: 0x009A99C0
// FUNCTION: fn_13756
void UIParaResultWnd::OnRBtnDown(int x, int y) {
  const int previousHovered = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);
  if (previousHovered != this->hovered) {
    this->Invalidate();
  }

  ITEM_INFO item = this->GetItemByIndex(this->hovered);
  if (item.identity[1] > 0) {
    g_modeMgr.GetGameMode();
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x009A96B0
// FUNCTION: fn_13753
void __stdcall Nullsub344(int, int) {
}

// OFFSET: 0x009A83C0
// FUNCTION: fn_13750
void UIParaResultWnd::OnCreate(int width, int height) {
  this->total = new UIStaticText();
  this->total->SetSize(160, 15);
  this->total->Move(12, this->geometry.height - 21);
  this->AddChild(this->total);

  this->title = new UIStaticText();
  this->title->SetSize(70, 15);
  this->title->SetText(LookupMessage(MSI_GUILD_INVITE), 0);
  this->title->Move(16, 2);
  this->title->shadow = 1;
  this->AddChild(this->title);

  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  this->resizer->SetDirection(7);
  this->resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  this->resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  this->scrollBar = new UIScrollBar();
  this->scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - this->headerHeight - this->footerHeight - 17);
  this->AddChild(this->scrollBar);

  {
    const std::string prefix = "유저인터페이스\\";
    const int x = this->geometry.width - 61;
    const int y = this->geometry.height - 24;
    this->buttons[0] = new UIBitmapButton();
    this->buttons[0]->SetBitmap((prefix + "btn_ok.bmp").c_str(), 0, 0);
    this->buttons[0]->SetBitmap((prefix + "btn_ok_a.bmp").c_str(), 1, 0);
    this->buttons[0]->SetBitmap((prefix + "btn_ok_b.bmp").c_str(), 2, 0);
    this->buttons[0]->SetSize(this->buttons[0]->BitmapWidth(), this->buttons[0]->BitmapHeight());
    this->buttons[0]->Move(x, y);
    this->buttons[0]->SetId(184);
    this->AddChild(this->buttons[0]);
  }

  this->visibleRows = (height - this->headerHeight - this->footerHeight - 17) / 32;
  this->columns = 1;
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x009A9AB0
// FUNCTION: fn_13757
int UIParaItemPurchaseWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETTITLE:
    this->title->SetTextAutoWidth("Total : 0 Zeny");
    return 0;
  case UM_SETINFO:
    this->saved = reinterpret_cast<ItemPurchaseWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->SendMsg(nullptr, UM_RESIZE, 3, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    ModeDragState& drag = mode->drag;
    switch (drag.type) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN: {
      const int count = drag.uniqueItem == 1 ? g_session.GetItem1720Quantity(drag.id) : g_session.GetItem1720QuantityByItemId(drag.name.c_str());
      if (count > 0) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EQUIPITEM_OLNY_ONE)), 255, 0, 0, 0);
        return 0;
      }
      break;
    }
    }

    ITEM_INFO item;
    item.Assign(drag);
    const int total = g_session.GetItem1720Total();
    const int price = drag.price;
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
      sprintf_s(text, sizeof(text), LookupMessage(MSI_LIMIT_BUY_ITEM2), 30000);
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 255, 0, 0, 0);
    }

    const int available = item.flags[3] == 1 ? g_session.GetItem1718QuantityOf(item) : g_session.GetItem1718QuantityByItemId(item);
    if (available != -1 && available < a) {
      item.quantity = available;
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PARA_PURCHASE_ITEM_FAIL)), 255, 0, 0, 0);
    }

    if (item.quantity > 0) {
      std::uint16_t weight;
      if (item.flags[3] == 1) {
        const std::uint16_t added = g_session.GetShopItemPriceFactor(item) * static_cast<std::uint16_t>(item.quantity);
        weight = added + g_session.SumPurchasePriceFactor();
      } else {
        const std::uint16_t added = g_session.GetShopItemPriceFactorByItemId(item) * static_cast<std::uint16_t>(item.quantity);
        weight = added + g_session.SumPurchasePriceFactorByItemId();
      }

      if (!affordable) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INSUFFICIENT_MONEY)), 255, 0, 0, 0);
      } else if (weight + g_session.m_weight > g_session.m_maxWeight) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_GUILD_STORAGE_ITEM_COUNT)), 255, 0, 0, 0);
      } else {
        if (item.flags[3] == 1) {
          g_session.RemoveShopItem(item);
          g_session.MergePurchaseItem(item);
        } else {
          g_session.RemoveItem1718ByItemId(item);
          g_session.MergeItem1720ByName(item);
        }

        this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        dynamic_cast<UIParaItemShopWnd*>(g_windowMgr.LookupWindow(WID_PARAITEMSHOPWND))->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    CMode* mode = g_modeMgr.gameMode;
    switch (a) {
    case ID_OK: {
      ITEM_INFO item = g_session.GetItem1720At(0);
      if (item.flags[3] == 1) {
        mode->SendMsg(MM_NPC_MARKET_PURCHASE_V2, 0, 0, 0, 0);
      } else {
        mode->SendMsg(MM_NPC_MARKET_PURCHASE, 0, 0, 0, 0);
      }
      g_session.ClearTradeItems();
      return 0;
    }
    case ID_CANCEL:
      mode->SendMsg(MM_NPC_MARKET_CLOSE, 0, 0, 0, 0);
      mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
      g_session.ClearTradeItems();
      return 0;
    }
    break;
  }
  case UM_RESIZE:
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
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    int sum = 0;
    const int size = g_session.GetList2Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1720At(i);
      if (item.identity[1] != 0) {
        sum += item.quantity * item.prices[1];
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
  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    ItemListChunkHeader header = *reinterpret_cast<const ItemListChunkHeader*>(bytes);
    unsigned offset = 6;
    if (header.type == 20700) {
      while (static_cast<unsigned>(b) > offset) {
        header = *reinterpret_cast<const ItemListChunkHeader*>(bytes + offset);
        offset += 6;
        if (header.type == 20701) {
          UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMSHOPWND);
          if (window) {
            window->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
            window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          }

          this->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
          this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
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
  case UM_SETINFO:
    this->saved = reinterpret_cast<ItemShopWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->SendMsg(nullptr, UM_RESIZE, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_COMMAND:
    this->RecordResponse(static_cast<int>(a), b, c, d);
    if (a == 213) {
      if (g_replayAnimationScaling != 0) {
        UICheckBox* checkBox = this->checkBox;
        checkBox->geometry.state = b;
        checkBox->Invalidate();
      }

      g_windowMgr.dragAllItems = b;
      return 0;
    }
    break;
  case UM_DROP:
    if (a == 0) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      const ModeDragState& drag = mode->drag;
      ITEM_INFO item;
      item.Assign(drag);
      CSession* lists = &g_session;
      if (item.flags[3] == 1) {
        lists->RemovePurchaseItem(item);
        lists->MergeShopItem(item);
      } else {
        lists->RemoveItem1720ByName(item);
        lists->MergeItem1718ByName(item);
      }

      dynamic_cast<UIParaItemPurchaseWnd*>(g_windowMgr.LookupWindow(WID_PARAITEMPURCHASEWND))->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    }
    break;
  case UM_SETTITLE:
    if (a == 1) {
      this->sellMode = 1;
      this->showQuantity = 1;
      this->title->SetTextAutoWidth(LookupMessage(MSI_ITEMSYOUCANSELL));
      this->checkBox->m_disabled = 0;
    } else {
      this->sellMode = 0;
      this->showQuantity = 0;
      this->title->SetTextAutoWidth(LookupMessage(MSI_SHOPITEMS));
      this->checkBox->m_disabled = 1;
    }
    break;
  case UM_RESIZE:
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
        this->visibleRows = (newHeight - this->footerHeight - this->bottomPadding - 17) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - this->footerHeight - this->bottomPadding - 17);
        this->checkBox->Move(12, this->geometry.height - 20);
        this->UpdateScrollBar();
      }

      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    const int size = g_session.GetList1Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1718At(i);
      if (item.identity[1] > 0) {
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
  case UM_SETINFO:
    this->total->SetTextAutoWidth("Total : 0 Zeny");
    return 0;
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    CMode* mode = g_modeMgr.gameMode;
    if (a == 184) {
      mode->SendMsg(MM_NPC_MARKET_CLOSE, 0, 0, 0, 0);
      mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
      g_session.ClearTradeItems();
      return 0;
    }
    break;
  }
  case UM_SETTITLE:
    this->showQuantity = 0;
    this->title->SetTextAutoWidth(LookupMessage(MSI_GUILD_INVITE));
    this->SendMsg(nullptr, UM_RESIZE, 7, this->geometry.width, a, 0);
    return 0;
  case UM_RESIZE:
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
        this->visibleRows = (newHeight - this->headerHeight - this->footerHeight - 17) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - this->headerHeight - this->footerHeight - 17);
        this->UpdateScrollBar();
      }

      this->total->Move(12, this->geometry.height - 20);
      this->buttons[0]->Move(this->geometry.width - 61, this->geometry.height - 24);
      this->Move(g_renderer->viewport.width / 2 - this->geometry.width / 2, g_renderer->viewport.height / 2 - this->geometry.height / 2);
      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_REFRESHITEM: {
    int sum = 0;
    for (const ITEM_INFO& item : this->items) {
      sum += item.prices[1] * item.quantity;
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

  const int barHeight = this->DrawButtonBar(this->alternateButtonBar != 1 ? 0 : 2, 0, 1);
  unsigned red = 0xFF, green = 0xFF, blue = 0xFF;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  const int height = this->geometry.height - barHeight - titleHeight;
  this->FillRegion(0, titleHeight, this->geometry.width, height, RGB(red, green, blue));

  auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
  if (left) {
    for (int i = 0; i < height / left->image.height; ++i) {
      this->DrawBitmap(0, left->image.height * i + titleHeight, left, false);
    }
    if (height % left->image.height > 0) {
      this->DrawBitmap(0, this->geometry.height - barHeight - left->image.height, left, false);
    }
  }

  auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
  if (right) {
    for (int i = 0; i < height / right->image.height; ++i) {
      this->DrawBitmap(this->geometry.width - right->image.width, right->image.height * i + titleHeight, right, false);
    }
    if (height % right->image.height > 0) {
      this->DrawBitmap(this->geometry.width - right->image.width, this->geometry.height - barHeight - right->image.height, right, false);
    }
  }

  const int footer = this->showFooter == 1 ? this->footerHeight : 0;
  const int rows = (height - footer) / 32;
  for (int row = 0; row < rows; ++row) {
    DrawItemSlotBackground(*this, 30, titleHeight + row * 32);
  }

  int index = 0;
  int visible = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) == 1 && it->damaged == 1) {
      continue;
    }

    const int top = visible * 32 + titleHeight;
    if (this->hovered == index) {
      unsigned hoverRed = 0xFF, hoverGreen = 0xFF, hoverBlue = 0xFF;
      g_skinInfo.PaletteChannels(6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(72, top + 2, 191, 28, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    if (this->sellMode == 1 || it->quantity != 0) {
      DrawItemWithQuantity(*this, ResourceMgr::Instance(), 34, top + 4, *it, 0, 0);
    } else {
      DrawItemWithQuantityAuxiliary(*this, 1, 34, top + 4, *it, 0, 0);
    }

    this->DrawWrappedItemName(70, top + 12, 120, &*it, 0, 0, 12);

    if (0 < it->grade) {
      this->DrawBitmap(
        34, top + 16,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    std::string currency = "Z";
    if (this->showFooter == 1) {
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
    FormatGroupedInteger(static_cast<int>(it->prices[1]), price, sizeof(price));
    std::string text = FormatOwnedString(" %s %s", price, currency.c_str());
    if (it->prices[0] != it->prices[1]) {
      text = static_cast<const std::string&>(FormatOwnedString(" %d -> %s %s", it->prices[0], price, currency.c_str()));
    }

    this->DrawRightAlignedText(this->geometry.width - 18, top + 12, text.c_str(), text.size(), 0, 12, 0, 0);

    if (++visible == this->visibleRows) {
      break;
    }
  }

  if (this->showFooter == 1) {
    std::string text;
    if (g_serviceType == 0 && g_session.m_channelCopId == 1) {
      text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_POINT_SHOP_NHN), this->ClipTextToWidth(g_session.DecodePlayerName(), 100, 0, 12, 0), g_session.m_pendingAid));
    } else {
      text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_POINT_SHOP), this->ClipTextToWidth(g_session.DecodePlayerName(), 100, 0, 12, 0), g_session.m_pendingAid));
    }
    this->DrawTextAt(12, this->geometry.height - 21, text.c_str(), text.size(), 0, 12, 0, 0, 0);

    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_POINT_SHOP2), this->ClipTextToWidth(g_session.DecodePlayerName(), 100, 0, 12, 0), g_session.m_pendingGid));
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

  const int rows = (this->geometry.height - this->footerHeight - 17) / 8 + ((this->geometry.height - this->footerHeight - 17) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, left, false);
  }

  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
  }

  {
    unsigned red = 0, green = 0, blue = 0;
    g_skinInfo.PaletteChannels(2, 2, red, green, blue);
    const int listHeight = this->geometry.height - this->footerHeight - 17;
    this->FillRegion(20, 17, (this->geometry.width - 60) % 32, listHeight, RGB(red, green, blue));
    this->FillRegion(20, listHeight / 32 * 32 + 17, this->geometry.width - 24, listHeight % 32, RGB(red, green, blue));
  }
  this->DrawButtonBar(this->alternateButtonBar != 1 ? 0 : 2, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const int slots = (this->geometry.height - this->headerHeight - this->footerHeight - 17) / 32;
  for (int slot = slots, y = 17; slot > 0; --slot, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  {
    unsigned red = 0, green = 0, blue = 0;
    g_skinInfo.PaletteChannels(2, 2, red, green, blue);
    this->FillRegion(72, 17, columns * 32 - 32, slots * 32, RGB(red, green, blue));
  }

  int index = 0;
  int visible = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) == 1 && it->damaged) {
      continue;
    }

    const int column = visible % this->columns * 32;
    const int top = visible / this->columns * 32;
    if (this->hovered == index) {
      unsigned hoverRed = 0, hoverGreen = 0, hoverBlue = 0;
      g_skinInfo.PaletteChannels(6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(column + 72, top + 19, 191, 28, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    const int x = column + 44;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);
    this->DrawWrappedItemName(column + 76, top + 29, 110, &*it, 0, 0, 12);

    if (0 < it->grade) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    char text[64];
    char price[64];
    std::memset(text, 0, sizeof(text));
    std::memset(price, 0, sizeof(price));
    int length;
    if (it->prices[0] != it->prices[1]) {
      length = sprintf(text, " %d -> %s Z", it->prices[0], FormatGroupedInteger(static_cast<int>(it->prices[1]), price, 64));
    } else {
      length = sprintf(text, " %s Z", FormatGroupedInteger(static_cast<int>(it->prices[0]), price, 64));
    }
    this->DrawRightAlignedText(this->geometry.width - 18, top + 29, text, length, 0, 12, 0, 0);

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }
}
