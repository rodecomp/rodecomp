#include "UI/Shop/UIBartermarketWnd.h"
#include "Contents/Skin/SkinMgr.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Tables/MessageTable.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <format>
#include <map>
#include <new>
#include <string>

template __declspec(noinline) std::map<int, int>::map(const std::map<int, int>&);

// OFFSET: 0x00844BE0
// FUNCTION: fn_10567
UIExpandedBarterItemPurchaseWnd::UIExpandedBarterItemPurchaseWnd()
    : UIFrameWnd(0) {
  this->resizer = nullptr;
  this->scrollBar = nullptr;
  this->firstRow = 0;
  this->rowCount = 0;
  this->hoveredRow = 0;
  this->scrollEnabled = 0;
}

// OFFSET: 0x00844D00
// FUNCTION: fn_10569
UIExpandedBartermarketWnd::UIExpandedBartermarketWnd()
    : UIFrameWnd(0) {
  this->resizer = nullptr;
  this->scrollBar = nullptr;
  this->firstRow = 0;
  this->rowCount = 0;
  this->hoveredRow = 0;
  this->scrollEnabled = 0;
}

// OFFSET: 0x00840B30
// FUNCTION: fn_10545
void UIBarterItemPurchaseWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 31) {
    if (g_session.IsEquipItemType(drag->type) == true || g_session.IsShadowItemType(drag->type) == true) {
      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      return;
    }

    g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_ITEMDROPCNTWND)) {
      window->SendMsg(nullptr, UM_SETSTATE, 24, 0, 0, 0);
    }
  } else {
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00840C00
// FUNCTION: fn_10546
void UIBartermarketWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 4) {
    if (CMode* mode = g_modeMgr.GetGameMode()) {
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    }
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    return;
  }

  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_ITEMDROPCNTWND)) {
    window->SendMsg(nullptr, UM_SETSTATE, 23, 0, 0, 0);
  }
}

// OFFSET: 0x00840CC0
// FUNCTION: fn_10547
std::pair<bool, BarterMarketItem> UIBartermarketWnd::GetBarterItemAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return std::make_pair(false, BarterMarketItem());
  }

  const int row = (y - 17) / 32;
  std::list<BarterMarketItem> items = *g_barterMarketItemMgr->GetItems();
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketItem>::iterator it = items.begin(); it != items.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (visible == this->columns * row) {
        return std::make_pair(true, *it);
      }
      ++visible;
    }
  }
  return std::make_pair(false, BarterMarketItem());
}

// OFFSET: 0x00840FF0
// FUNCTION: fn_10548
std::pair<bool, BarterMarketItem> UIBarterItemPurchaseWnd::GetBarterItemOrPriceAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return std::make_pair(false, BarterMarketItem());
  }

  const int column = (x - 40) / 32;
  const int row = (y - 17) / 32;
  std::list<BarterMarketItem> items = *g_barterMarketItemMgr->GetAllItems();
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketItem>::iterator it = items.begin(); it != items.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (visible == this->columns * row) {
        if (column == 0) {
          return std::make_pair(true, *it);
        }
        BarterMarketItem price;
        char buffer[100];
        std::memset(buffer, 0, sizeof(buffer));
        _itoa(it->field_2c, buffer, 10);
        price.name = buffer;
        price.field_1c = 20;
        return std::make_pair(true, price);
      }
      ++visible;
    }
  }
  return std::make_pair(false, BarterMarketItem());
}

// OFFSET: 0x00841320
// FUNCTION: fn_10549
std::pair<bool, BarterMarketItem> UIBartermarketWnd::GetBarterItemOrPriceAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return std::make_pair(false, BarterMarketItem());
  }

  const int column = (x - 40) / 32;
  const int row = (y - 17) / 32;
  std::list<BarterMarketItem> items = *g_barterMarketItemMgr->GetItems();
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketItem>::iterator it = items.begin(); it != items.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (visible == this->columns * row) {
        if (column == 0) {
          return std::make_pair(true, *it);
        }
        BarterMarketItem price;
        char buffer[100];
        std::memset(buffer, 0, sizeof(buffer));
        _itoa(it->field_2c, buffer, 10);
        price.name = buffer;
        price.field_1c = 20;
        return std::make_pair(true, price);
      }
      ++visible;
    }
  }
  return std::make_pair(false, BarterMarketItem());
}

// OFFSET: 0x00841650
// FUNCTION: fn_10550
std::pair<bool, BarterMarketItem> UIBarterItemPurchaseWnd::GetBarterItemAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return std::make_pair(false, BarterMarketItem());
  }

  const int row = (y - 17) / 32;
  std::list<BarterMarketItem> items = *g_barterMarketItemMgr->GetAllItems();
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketItem>::iterator it = items.begin(); it != items.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (visible == this->columns * row) {
        return std::make_pair(true, *it);
      }
      ++visible;
    }
  }
  return std::make_pair(false, BarterMarketItem());
}


// OFFSET: 0x00841910
// FUNCTION: fn_10551
int UIBarterItemPurchaseWnd::GetInventoryItemCount(int itemId) {
  const int count = g_session.GetList0Size();
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item = g_session.GetInventoryItemAt(i);
    if (std::atoi(item.itemId.c_str()) == itemId) {
      return item.quantity;
    }
  }
  return 0;
}

// OFFSET: 0x00845940
// FUNCTION: fn_10576
void UIExpandedBarterItemPurchaseWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 31) {
    if (g_session.IsEquipItemType(drag->type) == true || g_session.IsShadowItemType(drag->type) == true) {
      this->SendMsg(0, UM_DROP, 0, 0, 0, 0);
      return;
    }
    g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
    g_windowMgr.LookupWindow(WID_ITEMDROPCNTWND)->SendMsg(0, UM_SETSTATE, 26, 0, 0, 0);
  } else {
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
  }
}

// OFFSET: 0x008459F0
// FUNCTION: fn_10577
void UIExpandedBartermarketWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 32) {
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }
  if (drag->quantity == 1) {
    this->SendMsg(0, UM_DROP, 0, 0, 0, 0);
    return;
  }
  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
  g_windowMgr.LookupWindow(WID_ITEMDROPCNTWND)->SendMsg(0, UM_SETSTATE, 25, 0, 0, 0);
}

// OFFSET: 0x00845F60
// FUNCTION: fn_10578
std::pair<bool, BarterMarketEntry> UIExpandedBarterItemPurchaseWnd::GetEntryAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->rowCount * 64 + 17) {
    return std::make_pair(false, BarterMarketEntry());
  }

  const int row = (y - 17) / 64;
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it, ++index) {
    if (index >= this->firstRow) {
      if (visible == row) {
        return std::pair<bool, BarterMarketEntry>(true, *it);
      }
      ++visible;
    }
  }
  return std::make_pair(false, BarterMarketEntry());
}

// OFFSET: 0x00846110
// FUNCTION: fn_10579
std::pair<bool, BarterMarketEntry> UIExpandedBartermarketWnd::GetEntryAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->rowCount * 64 + 17) {
    return std::make_pair(false, BarterMarketEntry());
  }

  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it, ++index) {
    if (index >= this->firstRow) {
      if (visible == (y - 17) / 64) {
        return std::pair<bool, BarterMarketEntry>(true, *it);
      }
      ++visible;
    }
  }
  return std::make_pair(false, BarterMarketEntry());
}

// OFFSET: 0x008462C0
// FUNCTION: fn_10580
std::pair<bool, BarterMarketEntry> UIExpandedBarterItemPurchaseWnd::GetEntryOrChunkAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->rowCount * 64 + 17) {
    return std::pair<bool, BarterMarketEntry>(false, BarterMarketEntry());
  }

  const int offsetY = y - 17;
  const int row = offsetY / 64;
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it, ++index) {
    if (index >= this->firstRow) {
      if (visible == row) {
        BarterMarketEntry entry;
        if ((offsetY / 32) & 1) {
          int right = 114;
          for (std::vector<BarterChunk16>::iterator chunk = it->chunks.begin(); chunk != it->chunks.end(); ++chunk) {
            if (x > right - 42 && x < right) {
              entry.field_0 = chunk->field_c;
              entry.field_28 = chunk->field_4;
              char buffer[16] = {};
              entry.name = _itoa(chunk->field_0, buffer, 10);
              return std::pair<bool, BarterMarketEntry>(true, entry);
            }
            right += 42;
          }
        }
        if (x > 40 && x < 72) {
          entry = *it;
          return std::pair<bool, BarterMarketEntry>(true, entry);
        }
      }
      ++visible;
    }
  }
  return std::pair<bool, BarterMarketEntry>(false, BarterMarketEntry());
}

// OFFSET: 0x00846590
// FUNCTION: fn_10581
std::pair<bool, BarterMarketEntry> UIExpandedBartermarketWnd::GetEntryOrChunkAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->rowCount * 64 + 17) {
    return std::make_pair(false, BarterMarketEntry());
  }

  const int offsetY = y - 17;
  const int row = offsetY / 64;
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it, ++index) {
    if (index >= this->firstRow) {
      if (visible == row) {
        BarterMarketEntry entry;
        if ((offsetY / 32) & 1) {
          int right = 114;
          for (std::vector<BarterChunk16>::iterator chunk = it->chunks.begin(); chunk != it->chunks.end(); ++chunk) {
            if (x > right - 42 && x < right) {
              entry.field_28 = chunk->field_4;
              char buffer[16] = {};
              entry.name = _itoa(chunk->field_0, buffer, 10);
              entry.field_0 = chunk->field_c;
              return std::pair<bool, BarterMarketEntry>(true, entry);
            }
            right += 42;
          }
        }
        if (x > 40 && x < 72) {
          entry = *it;
          return std::pair<bool, BarterMarketEntry>(true, entry);
        }
      }
      ++visible;
    }
  }
  return std::make_pair(false, BarterMarketEntry());
}

// OFFSET: 0x00849030
// FUNCTION: fn_10594
void UIExpandedBarterItemPurchaseWnd::OnWheel(int delta) {
  if (this->scrollEnabled == 1) {
    int step = this->rowCount - 1;
    if (step > 3) {
      step = 3;
    }
    this->SendMsg(this->scrollBar, UM_SCROLLVERT, delta > 0 ? -step : step, 0, 0, 0);
  }
}

// OFFSET: 0x00849080
// FUNCTION: fn_10595
void UIExpandedBartermarketWnd::OnWheel(int delta) {
  if (this->scrollEnabled == 1) {
    int step = this->rowCount - 1;
    if (step > 3) {
      step = 3;
    }
    this->SendMsg(this->scrollBar, UM_SCROLLVERT, delta > 0 ? -step : step, 0, 0, 0);
  }
}

// OFFSET: 0x008490D0
// FUNCTION: fn_10596
void UIExpandedBarterItemPurchaseWnd::UpdateScrollBar() {
  if (!this->scrollBar) {
    return;
  }

  const int rows = this->rowCount;
  const int count = static_cast<int>(this->entries.size());
  int overflow = count - rows;
  if (overflow < 0) {
    overflow = 0;
  }

  if (overflow != 0) {
    this->scrollEnabled = 1;
    this->scrollBar->SetRange(overflow, rows, count);

    int position = this->firstRow;
    if (position < 0) {
      this->firstRow = 0;
      position = 0;
    }

    const int limit = count - this->rowCount;
    if (position > limit) {
      this->firstRow = limit;
      position = limit;
    }

    this->scrollBar->SetPosition(position);
  } else {
    this->scrollEnabled = 0;
    this->firstRow = 0;
    this->scrollBar->SetRange(0, rows, count);
  }

  this->scrollBar->Invalidate();
}

// OFFSET: 0x00849170
// FUNCTION: fn_10597
void UIExpandedBartermarketWnd::UpdateScrollBar() {
  if (!this->scrollBar) {
    return;
  }

  const int rows = this->rowCount;
  const int count = static_cast<int>(this->entries.size());
  int overflow = count - rows;
  if (overflow < 0) {
    overflow = 0;
  }

  if (overflow != 0) {
    this->scrollEnabled = 1;
    this->scrollBar->SetRange(overflow, rows, count);

    int position = this->firstRow;
    if (position < 0) {
      this->firstRow = 0;
      position = 0;
    }

    const int limit = count - this->rowCount;
    if (position > limit) {
      this->firstRow = limit;
      position = limit;
    }

    this->scrollBar->SetPosition(position);
  } else {
    this->scrollEnabled = 0;
    this->firstRow = 0;
    this->scrollBar->SetRange(0, rows, count);
  }

  this->scrollBar->Invalidate();
}

// OFFSET: 0x00849210
// FUNCTION: fn_10598
int UIExpandedBarterItemPurchaseWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    std::pair<bool, BarterMarketEntry> found = g_expandedBarterMarketItemMgr->FindItem(mode->drag.barterId);
    if (!found.first) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_OUT_OF_STOCK)), 255, 0, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    }

    found.second.value = mode->drag.quantity;
    for (std::vector<BarterChunk16>::iterator chunk = found.second.chunks.begin(); chunk != found.second.chunks.end(); ++chunk) {
      ITEM_INFO owned = g_session.LookupClientItemByIdentity(chunk->field_0);
      if (owned.identity[1] > 0) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MONSTER_MONSTERINFO)), 255, 0, 0, 0);
        return 0;
      }

      if (g_session.HasItemCount(chunk->field_0, chunk->field_4) == true) {
        const int itemId = chunk->field_0;
        const int refine = chunk->field_4;
        const int count = g_session.GetList0Size();
        int available = 0;
        for (int i = 0; i < count; ++i) {
          ITEM_INFO item = g_session.GetInventoryItemAt(i);
          if (std::atoi(item.itemId.c_str()) == itemId && item.refiningLevel >= refine) {
            available = item.quantity;
            break;
          }
        }
        if (available <= 0) {
          mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
          g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MONSTER_MONSTERINFO)), 255, 0, 0, 0);
          return 0;
        }
      }
    }

    if (g_session.IsEquipItemType(mode->drag.type) == true) {
      if (g_expandedBarterMarketItemMgr->HasEntryByName(mode->drag.name) == true) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EQUIPITEM_OLNY_ONE)), 255, 0, 0, 0);
        return 0;
      }
    } else if (g_session.IsShadowItemType(mode->drag.type) == true) {
      if (g_expandedBarterMarketItemMgr->HasEntryByName(mode->drag.name) == true) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SAVE_HOTKEY_TO_SERVER)), 255, 0, 0, 0);
        return 0;
      }
    }

    if (mode->drag.quantity + g_expandedBarterMarketItemMgr->GetValueById(mode->drag.barterId) > 30000) {
      char buffer[200];
      std::memset(buffer, 0, sizeof(buffer));
      StringFormat(buffer, sizeof(buffer), static_cast<std::size_t>(-1), LookupMessage(MSI_GLOBALRODEX_MESSAGE_RECEIVE_ALL), 30000);
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 255, 0, 0, 0);
      return 0;
    }

    const int factor = static_cast<int>(g_expandedBarterMarketItemMgr->GetItemFactorById(mode->drag.barterId));
    if (factor < 0) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_OUT_OF_STOCK)), 255, 0, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    }

    if (factor > 0) {
      const int available = g_session.m_maxWeight - g_session.m_weight;
      const float remaining = static_cast<float>(available) - g_expandedBarterMarketItemMgr->GetWeightedTotal();
      if (static_cast<int>(remaining) / factor < mode->drag.quantity) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_OVER_WEIGHT)), 255, 0, 0, 0);
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        return 0;
      }
    }

    if (static_cast<__int64>(mode->drag.quantity * found.second.field_8) + g_expandedBarterMarketItemMgr->total > g_session.m_zeny) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INSUFFICIENT_MONEY)), 255, 0, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    }

    g_expandedBarterMarketItemMgr->AddEntry(found.second);
    this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    g_windowMgr.LookupWindow(WID_EXPANDEDBARTERMARKETWND)->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }

  case UM_COMMAND: {
    CMode* mode = g_modeMgr.GetGameMode();
    if (a == ID_OK) {
      if (this->Purchase()) {
        mode->SendMsg(MM_NPC_EXPANDED_BARTER_MARKET_PURCHASE, 0, 0, 0, 0);
        mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
        g_expandedBarterMarketItemMgr->Clear();
      }
    } else if (a == ID_CANCEL) {
      mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
      g_expandedBarterMarketItemMgr->Clear();
    }
    return 0;
  }

  case UM_RESIZE: {
    if (a != 3) {
      return 0;
    }

    int height = c;
    if (height < 366) {
      height = 366;
    } else if (height > 549.0f) {
      height = 549;
    }
    height = height / 64 * 64;

    if (height != this->geometry.height) {
      this->Resize(this->geometry.width, height);
      this->rowCount = (height - 46) / 64;
      this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
      this->scrollBar->Move(this->geometry.width - 15, 14);
      this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 46);
      this->UpdateScrollBar();
    }

    this->buyButton->Move(this->geometry.width - 105, this->geometry.height - 24);
    this->cancelButton->Move(this->geometry.width - 60, this->geometry.height - 24);
    g_windowMgr.StoreSnapRectangle(this);
    return 0;
  }

  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->firstRow;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->firstRow = previous - this->rowCount + 1;
    } else {
      if (message == UM_SCROLLVERTPAGEDOWN) {
        a = this->rowCount - 1;
      }
      this->firstRow = a + previous;
    }

    if (previous != this->firstRow) {
      this->UpdateScrollBar();
      this->Invalidate();
    }
    return this->firstRow - previous;
  }

  case UM_REFRESHITEM: {
    this->entries.erase(this->entries.begin(), this->entries.end());
    std::vector<BarterMarketEntry>* entries = g_expandedBarterMarketItemMgr->GetEntries();
    for (std::vector<BarterMarketEntry>::iterator it = entries->begin(); it != entries->end(); ++it) {
      this->entries.emplace_back(*it);
    }
    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }

  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x00849B30
// FUNCTION: fn_10599
int UIExpandedBartermarketWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    std::pair<bool, BarterMarketEntry> found = g_expandedBarterMarketItemMgr->FindEntry(mode->drag.barterId);
    if (found.first) {
      found.second.value = mode->drag.quantity;
      g_expandedBarterMarketItemMgr->RemoveEntry(found.second);
      g_windowMgr.LookupWindow(WID_EXPANDEDBARTERITEMPURCHASEWND)->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }

  case UM_RESIZE: {
    if (a != 7) {
      return 0;
    }

    int width = b;
    int height = c;
    if (width < 366) {
      width = 366;
    } else if (width > 439.2f) {
      width = 439;
    }
    if (height < 366) {
      height = 366;
    } else if (height > 549.0f) {
      height = 549;
    }
    height = height / 64 * 64;

    if (width != this->geometry.width || height != this->geometry.height) {
      this->Resize(width, height);
      this->rowCount = (height - 46) / 64;
      this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
      this->scrollBar->Move(this->geometry.width - 15, 14);
      this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 46);
      this->UpdateScrollBar();
    }
    g_windowMgr.StoreSnapRectangle(this);
    return 0;
  }

  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->firstRow;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->firstRow = previous - this->rowCount + 1;
    } else {
      if (message == UM_SCROLLVERTPAGEDOWN) {
        a = this->rowCount - 1;
      }
      this->firstRow = a + previous;
    }

    if (previous != this->firstRow) {
      this->UpdateScrollBar();
      this->Invalidate();
    }
    return this->firstRow - previous;
  }

  case UM_REFRESHITEM: {
    this->entries.clear();
    std::vector<BarterMarketEntry>* items = g_expandedBarterMarketItemMgr->GetItems();
    for (std::vector<BarterMarketEntry>::iterator it = items->begin(); it != items->end(); ++it) {
      this->entries.push_back(*it);
    }
    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }

  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x00AE75C0
// FUNCTION: fn_18076
__declspec(noinline) ITEM_INFO BarterMarketItem::ToItemInfo() const {
  ITEM_INFO item;
  item.identity[0] = this->field_1c;
  item.quantity = this->value;
  item.prices[0] = this->field_20;
  item.itemId = this->name;
  item.identified = 1;
  item.identity[2] = this->field_40;
  item.sprite = this->field_44;
  return item;
}

// OFFSET: 0x00AE7660
// FUNCTION: fn_18077
__declspec(noinline) ITEM_INFO BarterMarketEntry::ToItemInfo() const {
  ITEM_INFO item;
  item.identity[0] = this->field_0;
  item.quantity = this->value;
  item.prices[0] = this->field_8;
  item.itemId = this->name;
  item.refiningLevel = this->field_28;
  item.identified = 1;
  item.priceFactor = static_cast<std::uint16_t>(this->factor);
  item.identity[2] = this->field_38;
  item.sprite = this->field_3c;
  return item;
}

// OFFSET: 0x00843190
// FUNCTION: fn_10556
void UIBarterItemPurchaseWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);
  if (previous != this->hovered) {
    this->Invalidate();
  }

  std::pair<bool, BarterMarketItem> found = this->GetBarterItemAt(x, y);
  if (found.first == true) {
    if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
      mode->BeginDragBarterItem(4, found.second, found.second.value, -1);
    }
  }
}

// INLINED
__forceinline int UIBartermarketWnd::GetBarterItemIndexAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return -1;
  }

  const int column = std::min((x - 40) / 32, this->columns - 1);
  const int row = (y - 17) / 32;
  std::list<BarterMarketItem> items = *g_barterMarketItemMgr->GetItems();
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketItem>::iterator it = items.begin(); it != items.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (visible == this->columns * row + column) {
        return index;
      }
      ++visible;
    }
  }
  return -1;
}

// OFFSET: 0x00843270
// FUNCTION: fn_10557
void UIBartermarketWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  const int previous = this->hovered;
  this->hovered = this->GetBarterItemIndexAt(x, y);
  if (previous != this->hovered) {
    this->Invalidate();
  }

  std::pair<bool, BarterMarketItem> found = this->GetBarterItemAt(x, y);
  if (found.first) {
    if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
      mode->BeginDragBarterItem(31, found.second, 1, -1);
    }
  }
}

// OFFSET: 0x00843DE0
// FUNCTION: fn_10561
void UIBarterItemPurchaseWnd::OnRBtnDown(int x, int y) {
  std::pair<bool, BarterMarketItem> found = this->GetBarterItemOrPriceAt(x, y);
  if (found.first == true) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    ITEM_INFO item = found.second.ToItemInfo();
    if (window) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00843EC0
// FUNCTION: fn_10562
void UIBartermarketWnd::OnRBtnDown(int x, int y) {
  std::pair<bool, BarterMarketItem> found = this->GetBarterItemOrPriceAt(x, y);
  if (found.first) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    ITEM_INFO item = found.second.ToItemInfo();
    if (window) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// INLINED
__forceinline int UIExpandedBarterItemPurchaseWnd::GetEntryIndexAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || this->rowCount * 64 + 17 <= y) {
    return -1;
  }

  const int row = (y - 17) / 64;
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it, ++index) {
    if (index >= this->firstRow) {
      if (visible == row) {
        return index;
      }
      ++visible;
    }
  }
  return -1;
}

// OFFSET: 0x008481E0
// FUNCTION: fn_10587
void UIExpandedBarterItemPurchaseWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  const int previous = this->hoveredRow;
  this->hoveredRow = this->GetEntryIndexAt(x, y);
  if (previous != this->hoveredRow) {
    this->Invalidate();
  }

  std::pair<bool, BarterMarketEntry> found = this->GetEntryAt(x, y);
  if (found.first == true) {
    if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
      mode->BeginDragBarterEntry(32, found.second, found.second.value, -1);
    }
  }
}

// INLINED
__forceinline int UIExpandedBartermarketWnd::GetEntryIndexAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || this->rowCount * 64 + 17 <= y) {
    return -1;
  }

  const int row = (y - 17) / 64;
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it, ++index) {
    if (index >= this->firstRow) {
      if (visible == row) {
        return index;
      }
      ++visible;
    }
  }
  return -1;
}

// OFFSET: 0x00848320
// FUNCTION: fn_10588
void UIExpandedBartermarketWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  const int previous = this->hoveredRow;
  this->hoveredRow = this->GetEntryIndexAt(x, y);
  if (previous != this->hoveredRow) {
    this->Invalidate();
  }

  std::pair<bool, BarterMarketEntry> found = this->GetEntryAt(x, y);
  if (found.first == true) {
    if (found.second.value == 0) {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INVENTORY_EXTEND_OF_BODYITEM)), 255, 0, 0, 0);
    } else if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
      mode->BeginDragBarterEntry(31, found.second, 1, -1);
    }
  }
}

// OFFSET: 0x00848E70
// FUNCTION: fn_10592
void UIExpandedBarterItemPurchaseWnd::OnRBtnDown(int x, int y) {
  std::pair<bool, BarterMarketEntry> found = this->GetEntryOrChunkAt(x, y);
  if (found.first == true) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    ITEM_INFO item = found.second.ToItemInfo();
    if (window) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00848F50
// FUNCTION: fn_10593
void UIExpandedBartermarketWnd::OnRBtnDown(int x, int y) {
  std::pair<bool, BarterMarketEntry> found = this->GetEntryOrChunkAt(x, y);
  if (found.first == true) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    ITEM_INFO item = found.second.ToItemInfo();
    if (window) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00843420
// FUNCTION: fn_10558
void UIBarterItemPurchaseWnd::OnMouseHover(int x, int y) {
  std::pair<bool, BarterMarketItem> found = this->GetBarterItemAt(x, y);
  if (found.first) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);
    std::string text;
    BarterMarketItem item = found.second;
    ITEM_DESC description = FindItemDescription(item.field_2c);
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_NO_ITEM_FOR_EXCHANGE), description.identifiedName, item.field_28 * item.value));
    CMode* tooltip = g_modeMgr.gameMode;
    ITEM_INFO info = item.ToItemInfo();
    if (tooltip) {
      tooltip->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(info), 0, 1);
    }
  }
}

// OFFSET: 0x008436B0
// FUNCTION: fn_10559
void UIBartermarketWnd::OnMouseHover(int x, int y) {
  std::pair<bool, BarterMarketItem> found = this->GetBarterItemAt(x, y);
  if (found.first) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);
    std::string text;
    ITEM_DESC description = FindItemDescription(found.second.field_2c);
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_NO_ITEM_FOR_EXCHANGE), description.identifiedName, found.second.field_28));
    CMode* tooltip = g_modeMgr.gameMode;
    ITEM_INFO info = found.second.ToItemInfo();
    if (tooltip) {
      tooltip->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(info), 0, 1);
    }
  }
}

// OFFSET: 0x008484A0
// FUNCTION: fn_10589
void UIExpandedBarterItemPurchaseWnd::OnMouseHover(int x, int y) {
  std::pair<bool, BarterMarketEntry> found = this->GetEntryAt(x, y);
  if (found.first && x > 40 && x < 72) {
    int tooltipX = x;
    int tooltipY = (y - 17) / 64 * 64 + 1;
    this->ToScreen(tooltipX, tooltipY);
    std::string text;
    int index = 1;
    for (std::vector<BarterChunk16>::iterator chunk = found.second.chunks.begin(); chunk != found.second.chunks.end(); ++chunk, ++index) {
      ITEM_DESC description = FindItemDescription(chunk->field_0);
      const char* name = description.identifiedName;
      if (chunk->field_4 > 0) {
        text += "+";
        char buffer[12];
        _itoa(chunk->field_4, buffer, 10);
        text += buffer;
        text += " ";
      }
      text += FormatOwnedString(LookupMessage(MSI_NO_ITEM_FOR_EXCHANGE), name, chunk->amount * found.second.value);
      if (index != static_cast<int>(found.second.chunks.size())) {
        text += "\n";
      }
    }
    CMode* tooltip = g_modeMgr.GetGameMode();
    ITEM_INFO info = found.second.ToItemInfo();
    if (tooltip) {
      tooltip->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(info), 0, 1);
    }
  }
}

// OFFSET: 0x00848B80
// FUNCTION: fn_10591
void UIExpandedBartermarketWnd::OnMouseHover(int x, int y) {
  std::pair<bool, BarterMarketEntry> found = this->GetEntryAt(x, y);
  if (found.first && x > 40 && x < 72) {
    int tooltipX = x;
    int tooltipY = (y - 17) / 64 * 64 + 1;
    this->ToScreen(tooltipX, tooltipY);
    std::string text;
    int index = 1;
    for (std::vector<BarterChunk16>::iterator chunk = found.second.chunks.begin(); chunk != found.second.chunks.end(); ++chunk, ++index) {
      ITEM_DESC description = FindItemDescription(chunk->field_0);
      const char* name = description.identifiedName;
      if (chunk->field_4 > 0) {
        text += "+";
        char buffer[12];
        _itoa(chunk->field_4, buffer, 10);
        text += buffer;
        text += " ";
      }
      text += FormatOwnedString(LookupMessage(MSI_NO_ITEM_FOR_EXCHANGE), name, chunk->amount);
      if (index != static_cast<int>(found.second.chunks.size())) {
        text += "\n";
      }
    }
    CMode* tooltip = g_modeMgr.GetGameMode();
    ITEM_INFO info = found.second.ToItemInfo();
    if (tooltip) {
      tooltip->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(info), 0, 1);
    }
  }
}

// OFFSET: 0x00843910
// FUNCTION: fn_10560
void UIPurchaseButton::OnMouseHover(int x, int y) {
  std::string text;
  std::map<int, int> totals = *g_barterMarketItemMgr->GetTotals();
  int count = 0;
  for (std::map<int, int>::iterator it = totals.begin(); it != totals.end();) {
    const int amount = it->second;
    ITEM_DESC description = FindItemDescription(it->first);
    ++it;
    ++count;
    if (count == 30) {
      text += "...";
      break;
    }
    if (it == totals.end()) {
      text += FormatOwnedString(LookupMessage(MSI_NO_ITEM_FOR_EXCHANGE), description.identifiedName, amount);
    } else {
      text += FormatOwnedString(LookupMessage(MSI_NO_ITEM_FOR_EXCHANGE), description.identifiedName, amount) + "\n";
    }
  }
  int tooltipX = 0;
  int tooltipY = -4 - count * 16;
  this->ToScreen(tooltipX, tooltipY);
  CMode* tooltip = g_modeMgr.gameMode;
  if (tooltip) {
    tooltip->ShowTooltip(text.c_str(), tooltipX, tooltipY, 0xFFFFFFFF, 0, 1);
  }
}

// OFFSET: 0x00846940
// FUNCTION: fn_10582
std::string UIExpandedBarterPurchaseButton::FormatChunkText(int refine, std::string name, int amount) {
  std::string text("");
  if (refine > 0) {
    char buffer[5] = {};
    _itoa(refine, buffer, 10);
    text += "+" + std::string(buffer) + " ";
  }
  char line[1024];
  std::memset(line, 0, sizeof(line));
  StringFormat(line, sizeof(line), static_cast<std::size_t>(-1), LookupMessage(MSI_NO_ITEM_FOR_EXCHANGE), name.c_str(), amount);
  text += std::string(line);
  return text;
}

// OFFSET: 0x00848790
// FUNCTION: fn_10590
void UIExpandedBarterPurchaseButton::OnMouseHover(int x, int y) {
  std::string text;
  std::vector<BarterChunk16> chunks = *g_expandedBarterMarketItemMgr->GetChunks();
  int index = 0;
  for (std::vector<BarterChunk16>::iterator chunk = chunks.begin(); chunk != chunks.end(); ++chunk, ++index) {
    ITEM_DESC description = FindItemDescription(chunk->field_0);
    const char* name = description.identifiedName;
    if (index == 30) {
      text += "...";
      break;
    }
    text += this->FormatChunkText(chunk->field_4, name, chunk->amount);
    if (index != static_cast<int>(chunks.size()) - 1) {
      text += "\n";
    }
  }
  if (g_expandedBarterMarketItemMgr->total > 0) {
    std::string total = FormatOwnedString(LookupMessage(MSI_ITEM_GRADE_ENCHANT_SUCCEESS), g_expandedBarterMarketItemMgr->total);
    text += "\n" + total;
  }
  if (!text.empty()) {
    int tooltipX = 0;
    int tooltipY = -4 - index * 16;
    this->ToScreen(tooltipX, tooltipY);
    CMode* tooltip = g_modeMgr.gameMode;
    if (tooltip) {
      tooltip->ShowTooltip(text.c_str(), tooltipX, tooltipY, 0xFFFFFFFF, 0, 1);
    }
  }
}


// OFFSET: 0x00840530
// FUNCTION: fn_10544
bool UIBarterItemPurchaseWnd::Purchase() {
  std::map<std::string, int> totals;
  std::list<BarterMarketItem> items = *g_barterMarketItemMgr->GetAllItems();

  int count = g_session.GetList0Size();
  count += g_session.CountDistinctEquippedItems();
  count += g_session.CountDistinctCostumeItems();

  for (std::list<BarterMarketItem>::iterator it = items.begin(); it != items.end(); ++it) {
    std::map<std::string, int>::iterator total = totals.find(it->name);
    if (total != totals.end()) {
      total->second += it->value;
    } else {
      totals.insert(std::make_pair(it->name, it->value));
    }

    if (g_session.IsEquipItemType(it->field_1c) == true || g_session.IsShadowItemType(it->field_1c) == true) {
      ++count;
    } else if (this->GetInventoryItemCount(std::atoi(it->name.c_str())) == 0) {
      ++count;
    }
  }

  if (count > g_session.extraInventorySlots + 100) {
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SOUL_REAPER)), 255, 0, 0, 0);
    this->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    return false;
  }

  std::map<int, int>* required = g_barterMarketItemMgr->GetTotals();
  for (std::map<int, int>::iterator it = required->begin(); it != required->end(); ++it) {
    if (this->GetInventoryItemCount(it->first) < it->second) {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PVP)), 255, 0, 0, 0);
      this->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
      return false;
    }
  }

  for (std::map<std::string, int>::iterator it = totals.begin(); it != totals.end(); ++it) {
    if (it->second + this->GetInventoryItemCount(std::atoi(it->first.c_str())) > 30000) {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      const std::string text = FormatOwnedString(LookupMessage(MSI_STAR_EMPEROR), 30000);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 255, 0, 0, 0);
      this->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00841A10
// FUNCTION: fn_10552
void UIBarterItemPurchaseWnd::OnCreate(int width, int height) {
  UIResizer* resizer = new UIResizer();
  this->resizer = resizer;
  resizer->SetSize(16, 16);
  resizer->SetDirection(3);
  resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(resizer);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 38);
  this->AddChild(scrollBar);

  const std::string prefix = "유저인터페이스\\";
  const char* names[2][3] = {
    {"btn_buy", "btn_buy_a", "btn_buy_b"},
    {"btn_cancel", "btn_cancel_a", "btn_cancel_b"},
  };
  const int ids[2] = {184, 185};
  const int xs[2] = {this->geometry.width - 106, this->geometry.width - 61};
  const int y = this->geometry.height - 24;

  for (int index = 0; index < 2; ++index) {
    UIBitmapButton* button = index == 0 ? static_cast<UIBitmapButton*>(new UIPurchaseButton()) : new UIBitmapButton();
    this->buttons[index] = button;
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(xs[index], y);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  this->columns = 1;
  this->visibleRows = (height - 38) / 32;

  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x00843FA0
// FUNCTION: fn_10563
int UIBarterItemPurchaseWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO: {
    this->saved = reinterpret_cast<ItemPurchaseWndInfo*>(a);
    if (!this->saved) {
      return 0;
    }
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->SendMsg(nullptr, UM_RESIZE, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  }

  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (!mode) {
      return 0;
    }

    std::pair<bool, BarterMarketItem> found = g_barterMarketItemMgr->FindItem(mode->drag.barterId);
    if (found.first == false) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_OUT_OF_STOCK)), 255, 0, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    }

    found.second.value = mode->drag.quantity;
    if (g_session.HasItemId(found.second.field_2c)) {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MONSTER_MONSTERINFO)), 255, 0, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    }

    if (g_barterMarketItemMgr->HasItemByName(mode->drag.name)) {
      int notice = 0;
      if (g_session.IsEquipItemType(mode->drag.type)) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        notice = 119;
      } else if (g_session.IsShadowItemType(mode->drag.type)) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        notice = 3569;
      }

      if (notice != 0) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(notice)), 255, 0, 0, 0);
        return 0;
      }
    }

    if (mode->drag.quantity + g_barterMarketItemMgr->GetAllItemValueByName(mode->drag.name) > 30000) {
      char buffer[200] = {};
      StringFormat(buffer, 200, static_cast<std::size_t>(-1), LookupMessage(MSI_LIMIT_BUY_ITEM2), 30000);
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 255, 0, 0, 0);
      return 0;
    }

    const int amountById = g_barterMarketItemMgr->GetAmountById(mode->drag.barterId);
    if (amountById < 0) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_OUT_OF_STOCK)), 255, 0, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    }

    if (amountById > 0) {
      const int available = g_session.m_maxWeight - g_session.m_weight;
      const float remaining = static_cast<float>(available) - g_barterMarketItemMgr->GetTotalAmount();
      if (static_cast<int>(remaining) / amountById < mode->drag.quantity) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_OVER_WEIGHT)), 255, 0, 0, 0);
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        return 0;
      }
    }

    g_barterMarketItemMgr->AddItem(found.second);
    this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_BARTERMARKETWND)) {
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }

  case UM_COMMAND: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (!mode) {
      return 0;
    }

    if (a == ID_OK) {
      if (!this->Purchase()) {
        return 0;
      }
      mode->SendMsg(MM_NPC_BARTER_MARKET_PURCHASE, 0, 0, 0, 0);
    } else if (a != ID_CANCEL) {
      return 0;
    }

    mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
    g_barterMarketItemMgr->Clear();
    return 0;
  }

  case UM_RESIZE: {
    if (a != 3) {
      return 0;
    }

    int width = b <= 320 ? static_cast<int>(b) : 320;
    int height = c <= 336 ? static_cast<int>(c) : 336;
    if (width < 232) {
      width = 232;
    }
    if (height < 120) {
      height = 120;
    }
    width = (width - 280) / 32 * 32 + 280;
    height = (height - 120) / 32 * 32 + 120;

    if (width != this->geometry.width || height != this->geometry.height) {
      this->Resize(width, height);
      this->columns = 1;
      const int rowsHeight = this->geometry.height - 38;
      this->visibleRows = rowsHeight / 32;
      this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
      this->scrollBar->Move(this->geometry.width - 15, 14);
      this->scrollBar->Resize(this->scrollBar->geometry.width, rowsHeight);
      this->UpdateScrollBar();
    }

    const int xs[2] = {this->geometry.width - 106, this->geometry.width - 61};
    const int y = this->geometry.height - 24;
    for (int index = 0; index < 2; ++index) {
      this->buttons[index]->Move(xs[index], y);
    }

    g_windowMgr.StoreSnapRectangle(this);
    return 0;
  }

  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    std::list<BarterMarketItem>* items = g_barterMarketItemMgr->GetAllItems();
    for (std::list<BarterMarketItem>::iterator it = items->begin(); it != items->end(); ++it) {
      ITEM_INFO info = it->ToItemInfo();
      this->items.push_back(info);
    }
    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }

  default:
    break;
  }

  return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x00846FB0
// FUNCTION: fn_10583
void UIExpandedBarterItemPurchaseWnd::OnCreate(int width, int height) {
  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  this->resizer->SetDirection(3);
  this->resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  this->resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  this->scrollBar = new UIScrollBar();
  this->scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 46);
  this->AddChild(this->scrollBar);

  this->buyButton = new UIExpandedBarterPurchaseButton;
  this->buyButton->SetBitmap("유저인터페이스\\btn_buy.bmp", 0, 0);
  this->buyButton->SetBitmap("유저인터페이스\\btn_buy_a.bmp", 1, 0);
  this->buyButton->SetBitmap("유저인터페이스\\btn_buy_b.bmp", 2, 0);
  this->buyButton->SetSize(this->buyButton->BitmapWidth(), this->buyButton->BitmapHeight());
  this->buyButton->Move(this->geometry.width - 105, this->geometry.height - 24);
  this->buyButton->SetId(184);
  this->AddChild(this->buyButton);

  this->cancelButton = new UIBitmapButton();
  this->cancelButton->SetBitmap("유저인터페이스\\btn_cancel.bmp", 0, 0);
  this->cancelButton->SetBitmap("유저인터페이스\\btn_cancel_a.bmp", 1, 0);
  this->cancelButton->SetBitmap("유저인터페이스\\btn_cancel_b.bmp", 2, 0);
  this->cancelButton->SetSize(this->cancelButton->BitmapWidth(), this->cancelButton->BitmapHeight());
  this->cancelButton->Move(this->geometry.width - 60, this->geometry.height - 24);
  this->cancelButton->SetId(185);
  this->AddChild(this->cancelButton);

  this->rowCount = (height - 46) / 64;

  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x008446D0
// FUNCTION: fn_10564
int UIBartermarketWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO: {
    this->saved = reinterpret_cast<ItemShopWndInfo*>(a);
    if (a == 0) {
      return 0;
    }
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->SendMsg(nullptr, UM_RESIZE, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  }

  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (!mode) {
      return 0;
    }

    std::pair<bool, BarterMarketItem> found = g_barterMarketItemMgr->FindAllItem(mode->drag.barterId);
    if (found.first == false) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEM_OUT_OF_STOCK)), 255, 0, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    }

    found.second.value = mode->drag.quantity;
    g_barterMarketItemMgr->RemoveItem(found.second);
    if (UIWindow* window = g_windowMgr.LookupWindow(WID_BARTERITEMPURCHASEWND)) {
      window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }
    this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }

  case UM_RESIZE: {
    if (a == 7) {
      int width = b <= 320 ? static_cast<int>(b) : 320;
      int height = c <= 336 ? static_cast<int>(c) : 336;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }
      width = (width - 280) / 32 * 32 + 280;
      height = (height - 120) / 32 * 32 + 120;

      if (width != this->geometry.width || height != this->geometry.height) {
        this->Resize(width, height);
        this->columns = 1;
        const int contentHeight = this->geometry.height - this->footerHeight - this->bottomPadding - 17;
        this->visibleRows = contentHeight / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, contentHeight);
        this->UpdateScrollBar();
      }

      g_windowMgr.StoreSnapRectangle(this);
    }
    return 0;
  }

  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    std::list<BarterMarketItem>* items = g_barterMarketItemMgr->GetItems();
    for (std::list<BarterMarketItem>::iterator it = items->begin(); it != items->end(); ++it) {
      ITEM_INFO info = it->ToItemInfo();
      this->items.push_back(info);
    }
    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }

  default:
    break;
  }

  return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x00842570
// FUNCTION: fn_10553
void UIBartermarketWnd::OnCreate(int width, int height) {
  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(7);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  this->scrollBar = new UIScrollBar();
  static_cast<UIScrollBar*>(this->scrollBar)->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - this->bottomPadding - 17);
  this->AddChild(this->scrollBar);

  this->columns = 1;
  this->visibleRows = (height - this->bottomPadding - 17) / 32;

  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x00845220
// FUNCTION: fn_10575
bool UIExpandedBarterItemPurchaseWnd::Purchase() {
  std::map<std::string, int> totals;
  std::vector<BarterMarketEntry> entries = *g_expandedBarterMarketItemMgr->GetEntries();

  int count = g_session.GetList0Size();
  count += g_session.CountDistinctEquippedItems();
  count += g_session.CountDistinctCostumeItems();

  for (std::vector<BarterMarketEntry>::iterator it = entries.begin(); it != entries.end(); ++it) {
    std::map<std::string, int>::iterator total = totals.find(it->name);
    if (total != totals.end()) {
      total->second += it->value;
    } else {
      totals.insert(std::make_pair(it->name, it->value));
    }

    if (g_session.IsEquipItemType(it->field_0) == true || g_session.IsShadowItemType(it->field_0) == true) {
      ++count;
    } else {
      g_session.FindInventoryIndexByItemId(std::atoi(it->name.c_str()));
    }
  }

  if (count > g_session.extraInventorySlots + 100) {
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SOUL_REAPER)), 255, 0, 0, 0);
    this->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    return false;
  }

  if (!g_expandedBarterMarketItemMgr->HasRequiredItems()) {
    g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PVP)), 255, 0, 0, 0);
    this->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
    return false;
  }

  for (std::map<std::string, int>::iterator it = totals.begin(); it != totals.end(); ++it) {
    const int itemId = std::atoi(it->first.c_str());
    const int inventoryCount = g_session.GetList0Size();
    int owned = 0;
    for (int i = 0; i < inventoryCount; ++i) {
      ITEM_INFO item = g_session.GetInventoryItemAt(i);
      if (std::atoi(item.itemId.c_str()) == itemId) {
        owned = item.quantity;
        break;
      }
    }

    if (it->second + owned > 30000) {
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      const std::string text = FormatOwnedString(LookupMessage(MSI_GLOBALRODEX_MESSAGE_DELETE_ALL_ON_ADMIN), 30000);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 255, 0, 0, 0);
      this->SendMsg(nullptr, UM_COMMAND, ID_CANCEL, 0, 0, 0);
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00847290
// FUNCTION: fn_10584
void UIExpandedBartermarketWnd::OnCreate(int width, int height) {
  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(7);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  this->scrollBar = new UIScrollBar();
  this->scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 46);
  this->AddChild(this->scrollBar);

  this->rowCount = (height - 46) / 64;

  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x008426E0
// FUNCTION: fn_10554
void UIBarterItemPurchaseWnd::Paint() {
  this->DrawTitleBar(false, LookupMessage(MSI_EXCHANGE_ITEM_CNT), 0);
  if (this->geometry.height == 17) {
    return;
  }

  const int leftHeight = this->geometry.height - 38;
  const int leftTiles = leftHeight / 8 + (leftHeight % 8 != 0);
  for (int tile = 0, y = 17; tile < leftTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, bitmap, false);
  }
  const int rightHeight = this->geometry.height - 38;
  const int rightTiles = rightHeight / 8 + (rightHeight % 8 != 0);
  for (int tile = 0, y = 17; tile < rightTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - bitmap->image.width, y, bitmap, false);
  }

  unsigned red;
  unsigned green;
  unsigned blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  const int rightRemainder = (this->geometry.width - 60) % 32;
  this->FillRegion(20, 17, rightRemainder, this->geometry.height - 38, RGB(red, green, blue));
  const int bottomRemainder = (this->geometry.height - 38) % 32;
  this->FillRegion(20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, bottomRemainder, RGB(red, green, blue));
  this->DrawButtonBar(2, 0, 1);

  const int rows = (this->geometry.height - 38) / 32;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(40, 17, this->geometry.width - 50, rows * 32, RGB(red, green, blue));

  for (int row = 0; row < rows; ++row) {
    DrawItemSlotBackground(*this, 40, row * 32 + 17);
  }

  int visible = 0;
  int index = 0;
  std::list<BarterMarketItem>* items = g_barterMarketItemMgr->GetAllItems();
  for (std::list<BarterMarketItem>::iterator it = items->begin(); it != items->end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    const int row = visible / this->columns;
    const int column = visible % this->columns;
    const int x = column * 32;
    const int y = row * 32;
    if (this->hovered == index) {
      g_skinInfo.PaletteChannels(6, 2, red, green, blue);
      this->FillRegion(x + 72, y + 19, 0xBF, 0x1C, RGB(red, green, blue));
    }

    const ITEM_INFO item = it->ToItemInfo();
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x + 44, y + 21, item, 0, 0);
    this->DrawWrappedItemName(x + 76, y + 29, 100, &item, 0, 0, 12);
    char iconId[64] = {};
    StringFormat(iconId, sizeof(iconId), static_cast<std::size_t>(-1), "%d", it->field_2c);
    DrawItemIconById(*this, this->geometry.width - 70, y + 21, iconId, 1);
    char amount[64] = {};
    if (it->field_28 > 0) {
      const int bytes = StringFormat(amount, sizeof(amount), static_cast<std::size_t>(-1), "%d", it->value * it->field_28);
      this->DrawRightAlignedText(this->geometry.width - 25, y + 29, amount, bytes, 0, 12, 0, 0);
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }
}

// OFFSET: 0x00842BF0
// FUNCTION: fn_10555
void UIBartermarketWnd::Paint() {
  this->DrawTitleBar(false, LookupMessage(MSI_SHOPITEMS), 0);
  if (this->geometry.height == 17) {
    return;
  }

  const int leftHeight = this->geometry.height - this->bottomPadding - 17;
  const int leftTiles = leftHeight / 8 + (leftHeight % 8 != 0);
  for (int tile = 0, y = 17; tile < leftTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, bitmap, false);
  }
  const int rightHeight = this->geometry.height - this->bottomPadding - 17;
  const int rightTiles = rightHeight / 8 + (rightHeight % 8 != 0);
  for (int tile = 0, y = 17; tile < rightTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - bitmap->image.width, y, bitmap, false);
  }

  unsigned red;
  unsigned green;
  unsigned blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  const int rightRemainder = (this->geometry.width - 60) % 32;
  this->FillRegion(20, 17, rightRemainder, this->geometry.height - this->bottomPadding - 17, RGB(red, green, blue));
  const int bottomRemainder = (this->geometry.height - this->bottomPadding - 17) % 32;
  this->FillRegion(20, (this->geometry.height - this->bottomPadding - 17) / 32 * 32 + 17, this->geometry.width - 24, bottomRemainder, RGB(red, green, blue));
  this->DrawButtonBar(2, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const int rows = (this->geometry.height - this->footerHeight - this->bottomPadding - 17) / 32;
  for (int row = 0; row < rows; ++row) {
    DrawItemSlotBackground(*this, 40, row * 32 + 17);
  }
  unsigned slotRed;
  unsigned slotGreen;
  unsigned slotBlue;
  g_skinInfo.PaletteChannels(2, 2, slotRed, slotGreen, slotBlue);
  this->FillRegion(72, 17, columns * 32 - 32, rows * 32, RGB(slotRed, slotGreen, slotBlue));

  int visible = 0;
  int index = 0;
  std::list<BarterMarketItem> items = *g_barterMarketItemMgr->GetItems();
  for (std::list<BarterMarketItem>::iterator it = items.begin(); it != items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    const int row = visible / this->columns;
    const int column = visible % this->columns;
    const int x = column * 32;
    const int y = row * 32;
    if (this->hovered == index) {
      unsigned hoverRed;
      unsigned hoverGreen;
      unsigned hoverBlue;
      g_skinInfo.PaletteChannels(6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(x + 72, y + 19, this->geometry.width - 0x58, 0x1C, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    if (it->flag_38 == true) {
      DrawItemIcon(*this, ResourceMgr::Instance(), x + 44, y + 21, it->ToItemInfo(), 0);
    } else {
      DrawItemWithQuantity(*this, ResourceMgr::Instance(), x + 44, y + 21, it->ToItemInfo(), 0, 0);
    }
    {
      const ITEM_INFO item = it->ToItemInfo();
      this->DrawWrappedItemName(x + 76, y + 29, 120, &item, 0, 0, 12);
    }
    char iconId[64];
    StringFormat(iconId, sizeof(iconId), static_cast<std::size_t>(-1), "%d", it->field_2c);
    DrawItemIconById(*this, this->geometry.width - 70, y + 21, iconId, 1);
    if (it->field_28 > 0) {
      char amount[64];
      const int bytes = StringFormat(amount, sizeof(amount), static_cast<std::size_t>(-1), "%d", it->field_28);
      this->DrawRightAlignedText(this->geometry.width - 25, y + 29, amount, bytes, 0, 12, 0, 0);
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }
}

// OFFSET: 0x008473F0
// FUNCTION: fn_10585
void UIExpandedBarterItemPurchaseWnd::Paint() {
  this->DrawTitleBar(false, LookupMessage(MSI_EXCHANGE_ITEM_CNT), 0);

  const int leftHeight = this->geometry.height - 46;
  const int leftTiles = leftHeight / 8 + (leftHeight % 8 != 0);
  for (int tile = 0, y = 17; tile < leftTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, bitmap, false);
  }
  const int rightHeight = this->geometry.height - 46;
  const int rightTiles = rightHeight / 8 + (rightHeight % 8 != 0);
  for (int tile = 0, y = 17; tile < rightTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - bitmap->image.width, y, bitmap, false);
  }

  this->DrawButtonBar(2, 0, 1);
  unsigned red;
  unsigned green;
  unsigned blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, this->geometry.width - 40, this->geometry.height - 46, RGB(red, green, blue));
  this->DrawButtonBar(2, 0, 1);

  int visible = 0;
  int index = 0;
  for (std::list<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it, ++index) {
    if (index < this->firstRow) {
      continue;
    }

    const int y = visible * 64;
    DrawItemSlotBackground(*this, 40, y + 17);
    for (unsigned slot = 0; slot < it->chunks.size(); ++slot) {
      DrawItemSlotBackground(*this, 72 + slot * 42, y + 49);
    }
    if (this->hoveredRow == index) {
      unsigned hoverRed;
      unsigned hoverGreen;
      unsigned hoverBlue;
      g_skinInfo.PaletteChannels(6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(72, y + 19, this->geometry.width - 0x5A, 0x3C, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    DrawItemWithQuantity(*this, ResourceMgr::Instance(), 44, y + 21, it->ToItemInfo(), 0, 0);
    {
      const ITEM_INFO item = it->ToItemInfo();
      this->DrawWrappedItemName(76, y + 29, 100, &item, 0, 0, 12);
    }

    if (it->field_28 > 0) {
      std::uint32_t color = 0xFFFFFF;
      const std::string quantity = std::format("+{}", it->field_28);
      this->DrawOutlinedColoredText(50, y + 18, quantity.c_str(), quantity.size(), color, 200, 0, 12, 1);
    }

    int chunkIndex = 0;
    for (std::vector<BarterChunk16>::const_iterator chunk = it->chunks.begin(); chunk != it->chunks.end(); ++chunk) {
      ITEM_INFO chunkItem;
      chunkItem.quantity = chunk->amount * it->value;
      char itemId[33];
      _itoa(chunk->field_0, itemId, 10);
      chunkItem.itemId = itemId;
      chunkItem.refiningLevel = chunk->field_4;
      const int chunkX = 84 + chunkIndex * 42;
      DrawItemWithQuantity(*this, ResourceMgr::Instance(), chunkX - 8, y + 49, chunkItem, 1, 0);
      if (chunkItem.refiningLevel > 0) {
        char chunkAmount[16] = {};
        StringFormat(chunkAmount, sizeof(chunkAmount), static_cast<std::size_t>(-1), "+%d", chunkItem.refiningLevel);
        std::uint32_t color = 0xFFFFFF;
        this->DrawOutlinedColoredText(chunkX, y + 49, chunkAmount, std::strlen(chunkAmount), color, 200, 0, 12, 1);
      }
      ++chunkIndex;
    }

    char amount[23] = {};
    StringFormat(amount, sizeof(amount), static_cast<std::size_t>(-1), "%d z", it->field_8 * it->value);
    this->DrawRightAlignedText(this->geometry.width - 20, y + 60, amount, 0, 0, 12, 0, 0);

    if (++visible == this->rowCount) {
      ++index;
      break;
    }
  }

  for (int row = index; row < this->rowCount; ++row) {
    DrawItemSlotBackground(*this, 40, row * 64 + 17);
    for (int x = 72; x < 156; x += 42) {
      DrawItemSlotBackground(*this, x, row * 64 + 49);
    }
  }
}

// OFFSET: 0x00847AA0
// FUNCTION: fn_10586
void UIExpandedBartermarketWnd::Paint() {
  this->DrawTitleBar(false, LookupMessage(MSI_SHOPITEMS), 0);

  const int leftHeight = this->geometry.height - 46;
  const int leftTiles = leftHeight / 8 + (leftHeight % 8 != 0);
  for (int tile = 0, y = 17; tile < leftTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, bitmap, false);
  }
  const int rightHeight = this->geometry.height - 46;
  const int rightTiles = rightHeight / 8 + (rightHeight % 8 != 0);
  for (int tile = 0, y = 17; tile < rightTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - bitmap->image.width, y, bitmap, false);
  }

  unsigned red;
  unsigned green;
  unsigned blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, this->geometry.width - 40, this->geometry.height - 46, RGB(red, green, blue));
  this->DrawButtonBar(2, 0, 1);

  int visible = 0;
  int index = 0;
  for (std::list<BarterMarketEntry>::iterator it = this->entries.begin(); it != this->entries.end(); ++it, ++index) {
    if (index < this->firstRow) {
      continue;
    }

    const int y = visible * 64;
    DrawItemSlotBackground(*this, 40, y + 17);
    for (unsigned slot = 0; slot < it->chunks.size(); ++slot) {
      DrawItemSlotBackground(*this, 72 + slot * 42, y + 49);
    }
    if (this->hoveredRow == index) {
      unsigned hoverRed;
      unsigned hoverGreen;
      unsigned hoverBlue;
      g_skinInfo.PaletteChannels(6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(72, y + 19, this->geometry.width - 0x5A, 0x3C, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    if (it->flag_30 == true) {
      DrawItemIcon(*this, ResourceMgr::Instance(), 44, y + 21, it->ToItemInfo(), 0);
    } else if (it->value == 0) {
      DrawItemWithQuantityAuxiliary(*this, 1, 44, y + 21, it->ToItemInfo(), 1, 0);
    } else {
      DrawItemWithQuantity(*this, ResourceMgr::Instance(), 44, y + 21, it->ToItemInfo(), 1, 0);
    }
    {
      const ITEM_INFO item = it->ToItemInfo();
      this->DrawWrappedItemName(76, y + 29, 100, &item, 0, 0, 12);
    }
    if (it->field_28 > 0) {
      std::uint32_t color = 0xFFFFFF;
      const std::string quantity = std::format("+{}", it->field_28);
      this->DrawOutlinedColoredText(50, y + 18, quantity.c_str(), quantity.size(), color, 200, 0, 12, 1);
    }

    int chunkIndex = 0;
    for (std::vector<BarterChunk16>::const_iterator chunk = it->chunks.begin(); chunk != it->chunks.end(); ++chunk) {
      ITEM_INFO chunkItem;
      chunkItem.quantity = chunk->amount;
      char itemId[33];
      _itoa(chunk->field_0, itemId, 10);
      chunkItem.itemId = itemId;
      chunkItem.refiningLevel = chunk->field_4;
      const int chunkX = 76 + chunkIndex * 42;
      if (it->value == 0) {
        DrawItemWithQuantityAuxiliary(*this, 1, chunkX, y + 49, chunkItem, 1, 0);
      } else {
        DrawItemWithQuantity(*this, ResourceMgr::Instance(), chunkX, y + 49, chunkItem, 1, 0);
      }
      if (chunkItem.refiningLevel != 0) {
        char chunkAmount[16] = {};
        StringFormat(chunkAmount, sizeof(chunkAmount), static_cast<std::size_t>(-1), "+%d", chunkItem.refiningLevel);
        std::uint32_t color = 0xFFFFFF;
        this->DrawOutlinedColoredText(chunkX + 8, y + 49, chunkAmount, std::strlen(chunkAmount), color, 200, 0, 12, 1);
      }
      ++chunkIndex;
    }

    char amount[23] = {};
    StringFormat(amount, sizeof(amount), static_cast<std::size_t>(-1), "%d z", it->field_8);
    this->DrawRightAlignedText(this->geometry.width - 20, y + 60, amount, 0, 0, 12, 0, 0);

    if (++visible == this->rowCount) {
      ++index;
      break;
    }
  }

  for (int row = index; row < this->rowCount; ++row) {
    DrawItemSlotBackground(*this, 40, row * 64 + 17);
    for (int x = 72; x < 156; x += 42) {
      DrawItemSlotBackground(*this, x, row * 64 + 49);
    }
  }
}

// OFFSET: 0x00840100
// FUNCTION: fn_10538
UIBarterItemPurchaseWnd::UIBarterItemPurchaseWnd() : UIItemPurchaseWnd() {
}

// OFFSET: 0x00840160
// FUNCTION: fn_10539
UIBartermarketWnd::UIBartermarketWnd() : UIItemShopWnd() {
}
