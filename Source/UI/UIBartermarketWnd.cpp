#include "UI/UIBartermarketWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Text/ColoredText.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <map>
#include <new>
#include <string>

// OFFSET: 0x00844BE0
// FUNCTION: fn_10567
UIExpandedBarterItemPurchaseWnd::UIExpandedBarterItemPurchaseWnd(UIWindowContext& windowContext, UIFrameContext& frameContext)
    : UIFrameWnd(windowContext, frameContext, 0) {
  new (&this->unknown_00B4) std::list<BarterMarketEntry>();
  this->resizer = nullptr;
  this->scrollBar = nullptr;
  this->unknown_00C4 = 0;
  this->rowCount = 0;
  this->unknown_00CC = 0;
  this->scrollEnabled = 0;
}

// OFFSET: 0x00844D00
// FUNCTION: fn_10569
UIExpandedBartermarketWnd::UIExpandedBartermarketWnd(UIWindowContext& windowContext, UIFrameContext& frameContext)
    : UIFrameWnd(windowContext, frameContext, 0) {
  new (&this->unknown_00B4) std::list<BarterMarketEntry>();
  this->unknown_00BC = nullptr;
  this->scrollBar = nullptr;
  this->unknown_00C4 = 0;
  this->rowCount = 0;
  this->unknown_00CC = 0;
  this->scrollEnabled = 0;
}

// OFFSET: 0x00840B30
// FUNCTION: fn_10545
void UIBarterItemPurchaseWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 31) {
    if (g_session.IsEquipItemType(drag->type) == true || g_session.IsShadowItemType(drag->type) == true) {
      this->SendMsg(nullptr, 38, 0, 0, 0, 0);
      return;
    }

    reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
    if (UIWindow* window = g_windowMgr.LookupWindow(15)) {
      window->SendMsg(nullptr, 13, 24, 0, 0, 0);
    }
  } else {
    g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00840C00
// FUNCTION: fn_10546
void UIBartermarketWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 4) {
    if (CMode* mode = g_modeMgr.GetGameMode()) {
      mode->SendMsg(18, 0, 0, 0, 0);
    }
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  }

  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  if (UIWindow* window = g_windowMgr.LookupWindow(15)) {
    window->SendMsg(nullptr, 13, 23, 0, 0, 0);
  }
}

// OFFSET: 0x00840CC0
// FUNCTION: fn_10547
std::pair<bool, BarterMarketItem> UIBartermarketWnd::GetBarterItemAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return std::pair<bool, BarterMarketItem>(false, BarterMarketItem());
  }

  const int row = (y - 17) / 32;
  std::list<BarterMarketItem> items = *g_barterMarketItemMgr->GetItems();
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketItem>::iterator it = items.begin(); it != items.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (visible == this->columns * row) {
        return std::pair<bool, BarterMarketItem>(true, *it);
      }
      ++visible;
    }
  }
  return std::pair<bool, BarterMarketItem>(false, BarterMarketItem());
}

// OFFSET: 0x00840FF0
// FUNCTION: fn_10548
std::pair<bool, BarterMarketItem> UIBarterItemPurchaseWnd::GetBarterItemOrPriceAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return std::pair<bool, BarterMarketItem>(false, BarterMarketItem());
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
          return std::pair<bool, BarterMarketItem>(true, *it);
        }
        BarterMarketItem price;
        char buffer[100];
        std::memset(buffer, 0, sizeof(buffer));
        _itoa(it->field_2c, buffer, 10);
        price.name = buffer;
        price.field_1c = 20;
        return std::pair<bool, BarterMarketItem>(true, price);
      }
      ++visible;
    }
  }
  return std::pair<bool, BarterMarketItem>(false, BarterMarketItem());
}

// OFFSET: 0x00841320
// FUNCTION: fn_10549
std::pair<bool, BarterMarketItem> UIBartermarketWnd::GetBarterItemOrPriceAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return std::pair<bool, BarterMarketItem>(false, BarterMarketItem());
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
          return std::pair<bool, BarterMarketItem>(true, *it);
        }
        BarterMarketItem price;
        char buffer[100];
        std::memset(buffer, 0, sizeof(buffer));
        _itoa(it->field_2c, buffer, 10);
        price.name = buffer;
        price.field_1c = 20;
        return std::pair<bool, BarterMarketItem>(true, price);
      }
      ++visible;
    }
  }
  return std::pair<bool, BarterMarketItem>(false, BarterMarketItem());
}

// OFFSET: 0x00841650
// FUNCTION: fn_10550
std::pair<bool, BarterMarketItem> UIBarterItemPurchaseWnd::GetBarterItemAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return std::pair<bool, BarterMarketItem>(false, BarterMarketItem());
  }

  const int row = (y - 17) / 32;
  std::list<BarterMarketItem> items = *g_barterMarketItemMgr->GetAllItems();
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketItem>::iterator it = items.begin(); it != items.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (visible == this->columns * row) {
        return std::pair<bool, BarterMarketItem>(true, *it);
      }
      ++visible;
    }
  }
  return std::pair<bool, BarterMarketItem>(false, BarterMarketItem());
}

struct SessionMessengerContext {
  int GetList0Size() const;
};

// OFFSET: 0x00841910
// FUNCTION: fn_10551
int UIBarterItemPurchaseWnd::GetInventoryItemCount(int itemId) {
  const int count = reinterpret_cast<const SessionMessengerContext*>(&g_session)->GetList0Size();
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
void UIExpandedBarterItemPurchaseWnd::Slot10(int x, int y, std::intptr_t value) {
  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 31) {
    mode->SendMsg(18, 0, 0, 0, 0);
    return;
  }
  if (g_session.IsEquipItemType(drag->type) == true || g_session.IsShadowItemType(drag->type) == true) {
    this->SendMsg(0, 38, 0, 0, 0, 0);
    return;
  }
  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  g_windowMgr.LookupWindow(15)->SendMsg(0, 13, 26, 0, 0, 0);
}

// OFFSET: 0x008459F0
// FUNCTION: fn_10577
void UIExpandedBartermarketWnd::Slot10(int x, int y, std::intptr_t value) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 32) {
    g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
    return;
  }
  if (drag->quantity == 1) {
    this->SendMsg(0, 38, 0, 0, 0, 0);
    return;
  }
  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  g_windowMgr.LookupWindow(15)->SendMsg(0, 13, 25, 0, 0, 0);
}

// OFFSET: 0x00845F60
// FUNCTION: fn_10578
std::pair<bool, BarterMarketEntry> UIExpandedBarterItemPurchaseWnd::GetEntryAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->rowCount * 64 + 17) {
    return std::pair<bool, BarterMarketEntry>(false, BarterMarketEntry());
  }

  const int row = (y - 17) / 64;
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketEntry>::iterator it = this->Entries().begin(); it != this->Entries().end(); ++it, ++index) {
    if (index >= this->unknown_00C4) {
      if (visible == row) {
        return std::pair<bool, BarterMarketEntry>(true, *it);
      }
      ++visible;
    }
  }
  return std::pair<bool, BarterMarketEntry>(false, BarterMarketEntry());
}

// OFFSET: 0x00846110
// FUNCTION: fn_10579
std::pair<bool, BarterMarketEntry> UIExpandedBartermarketWnd::GetEntryAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->rowCount * 64 + 17) {
    return std::pair<bool, BarterMarketEntry>(false, BarterMarketEntry());
  }

  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketEntry>::iterator it = this->Entries().begin(); it != this->Entries().end(); ++it, ++index) {
    if (index >= this->unknown_00C4) {
      if (visible == (y - 17) / 64) {
        return std::pair<bool, BarterMarketEntry>(true, *it);
      }
      ++visible;
    }
  }
  return std::pair<bool, BarterMarketEntry>(false, BarterMarketEntry());
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
  for (std::list<BarterMarketEntry>::iterator it = this->Entries().begin(); it != this->Entries().end(); ++it, ++index) {
    if (index >= this->unknown_00C4) {
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
    return std::pair<bool, BarterMarketEntry>(false, BarterMarketEntry());
  }

  const int offsetY = y - 17;
  const int row = offsetY / 64;
  int index = 0;
  int visible = 0;
  for (std::list<BarterMarketEntry>::iterator it = this->Entries().begin(); it != this->Entries().end(); ++it, ++index) {
    if (index >= this->unknown_00C4) {
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
  return std::pair<bool, BarterMarketEntry>(false, BarterMarketEntry());
}

// OFFSET: 0x00849030
// FUNCTION: fn_10594
void UIExpandedBarterItemPurchaseWnd::Slot35(int delta) {
  if (this->scrollEnabled == 1) {
    int step = this->rowCount - 1;
    if (step > 3) {
      step = 3;
    }
    this->SendMsg(this->scrollBar, 7, delta > 0 ? -step : step, 0, 0, 0);
  }
}

// OFFSET: 0x00849080
// FUNCTION: fn_10595
void UIExpandedBartermarketWnd::Slot35(int delta) {
  if (this->scrollEnabled == 1) {
    int step = this->rowCount - 1;
    if (step > 3) {
      step = 3;
    }
    this->SendMsg(this->scrollBar, 7, delta > 0 ? -step : step, 0, 0, 0);
  }
}

// OFFSET: 0x008490D0
// FUNCTION: fn_10596
void UIExpandedBarterItemPurchaseWnd::UpdateScrollBar() {
  if (!this->scrollBar) {
    return;
  }

  const int rows = this->rowCount;
  const int count = this->itemCount;
  const int overflow = std::max(count - rows, 0);

  if (overflow != 0) {
    this->scrollEnabled = 1;
    this->scrollBar->SetRange(overflow, rows, count);

    int position = this->unknown_00C4;
    if (position < 0) {
      this->unknown_00C4 = 0;
      position = 0;
    }

    const int limit = count - this->rowCount;
    if (limit < position) {
      this->unknown_00C4 = limit;
      position = limit;
    }

    this->scrollBar->SetPosition(position);
  } else {
    this->scrollEnabled = 0;
    this->unknown_00C4 = 0;
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
  const int count = this->itemCount;
  const int overflow = std::max(count - rows, 0);

  if (overflow != 0) {
    this->scrollEnabled = 1;
    this->scrollBar->SetRange(overflow, rows, count);

    int position = this->unknown_00C4;
    if (position < 0) {
      this->unknown_00C4 = 0;
      position = 0;
    }

    const int limit = count - this->rowCount;
    if (limit < position) {
      this->unknown_00C4 = limit;
      position = limit;
    }

    this->scrollBar->SetPosition(position);
  } else {
    this->scrollEnabled = 0;
    this->unknown_00C4 = 0;
    this->scrollBar->SetRange(0, rows, count);
  }

  this->scrollBar->Invalidate();
}

// OFFSET: 0x00849210
// FUNCTION: fn_10598
int UIExpandedBarterItemPurchaseWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6: {
    CMode* mode = g_modeMgr.GetGameMode();
    if (a == 184) {
      if (this->Purchase()) {
        mode->SendMsg(321, 0, 0, 0, 0);
        mode->SendMsg(40, 0, 0, 0, 0);
        g_expandedBarterMarketItemMgr->Clear();
      }
    } else if (a == 185) {
      mode->SendMsg(40, 0, 0, 0, 0);
      g_expandedBarterMarketItemMgr->Clear();
    }
    return 0;
  }

  case 7:
  case 9:
  case 10: {
    const int previous = this->unknown_00C4;
    if (message == 9) {
      this->unknown_00C4 = previous - this->rowCount + 1;
    } else {
      if (message == 10) {
        a = this->rowCount - 1;
      }
      this->unknown_00C4 = a + previous;
    }

    if (previous != this->unknown_00C4) {
      this->UpdateScrollBar();
      this->Invalidate();
    }
    return this->unknown_00C4 - previous;
  }

  case 14: {
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
    this->m_frameContext->snapping.Store(this);
    return 0;
  }

  case 23: {
    this->Entries().erase(this->Entries().begin(), this->Entries().end());
    std::vector<BarterMarketEntry>* entries = g_expandedBarterMarketItemMgr->GetEntries();
    for (std::vector<BarterMarketEntry>::iterator it = entries->begin(); it != entries->end(); ++it) {
      this->Entries().emplace_back(*it);
    }
    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }

  case 38: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    std::pair<bool, BarterMarketEntry> found = g_expandedBarterMarketItemMgr->FindItem(mode->drag.unknown_03B0);
    if (!found.first) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3554)), 255, 0, 0, 0);
      mode->SendMsg(18, 0, 0, 0, 0);
      return 0;
    }

    found.second.value = mode->drag.quantity;
    for (std::vector<BarterChunk16>::iterator chunk = found.second.chunks.begin(); chunk != found.second.chunks.end(); ++chunk) {
      ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, chunk->field_0);
      bool missing = owned.unknown_0000[1] > 0;
      if (!missing && g_session.HasItemCount(chunk->field_0, chunk->field_4) == true) {
        const int itemId = chunk->field_0;
        const int refine = chunk->field_4;
        const int count = reinterpret_cast<const SessionMessengerContext*>(&g_session)->GetList0Size();
        int available = 0;
        for (int i = 0; i < count; ++i) {
          ITEM_INFO item = g_session.GetInventoryItemAt(i);
          if (std::atoi(item.itemId.c_str()) == itemId && item.unknown_0060 >= refine) {
            available = item.quantity;
            break;
          }
        }
        missing = available <= 0;
      }

      if (missing) {
        mode->SendMsg(18, 0, 0, 0, 0);
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3665)), 255, 0, 0, 0);
        return 0;
      }
    }

    if (g_session.IsEquipItemType(mode->drag.type) == true) {
      if (g_expandedBarterMarketItemMgr->HasEntryByName(mode->drag.name) == true) {
        mode->SendMsg(18, 0, 0, 0, 0);
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(119)), 255, 0, 0, 0);
        return 0;
      }
    } else if (g_session.IsShadowItemType(mode->drag.type) == true) {
      if (g_expandedBarterMarketItemMgr->HasEntryByName(mode->drag.name) == true) {
        mode->SendMsg(18, 0, 0, 0, 0);
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3569)), 255, 0, 0, 0);
        return 0;
      }
    }

    if (mode->drag.quantity + g_expandedBarterMarketItemMgr->GetValueById(mode->drag.unknown_03B0) > 30000) {
      char buffer[200];
      std::memset(buffer, 0, sizeof(buffer));
      StringFormat(buffer, sizeof(buffer), static_cast<std::size_t>(-1), LookupMessage(3692), 30000);
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(buffer), 255, 0, 0, 0);
      return 0;
    }

    const int factor = static_cast<int>(g_expandedBarterMarketItemMgr->GetItemFactorById(mode->drag.unknown_03B0));
    if (factor < 0) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3554)), 255, 0, 0, 0);
      mode->SendMsg(18, 0, 0, 0, 0);
      return 0;
    }

    if (factor > 0) {
      const int available = g_session.m_maxWeight - g_session.m_weight;
      const float remaining = static_cast<float>(available) - g_expandedBarterMarketItemMgr->GetWeightedTotal();
      if (static_cast<int>(remaining) / factor < mode->drag.quantity) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(56)), 255, 0, 0, 0);
        mode->SendMsg(18, 0, 0, 0, 0);
        return 0;
      }
    }

    if (static_cast<__int64>(mode->drag.quantity * found.second.field_8) + g_expandedBarterMarketItemMgr->total > g_session.m_zeny) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(55)), 255, 0, 0, 0);
      mode->SendMsg(18, 0, 0, 0, 0);
      return 0;
    }

    g_expandedBarterMarketItemMgr->AddEntry(found.second);
    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    g_windowMgr.LookupWindow(341)->SendMsg(nullptr, 23, 0, 0, 0, 0);
    mode->SendMsg(18, 0, 0, 0, 0);
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
  case 7:
  case 9:
  case 10: {
    const int previous = this->unknown_00C4;
    if (message == 9) {
      this->unknown_00C4 = previous - this->rowCount + 1;
    } else {
      if (message == 10) {
        a = this->rowCount - 1;
      }
      this->unknown_00C4 = a + previous;
    }

    if (previous != this->unknown_00C4) {
      this->UpdateScrollBar();
      this->Invalidate();
    }
    return this->unknown_00C4 - previous;
  }

  case 14: {
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
      this->unknown_00BC->Move(this->geometry.width - 15, this->geometry.height - 15);
      this->scrollBar->Move(this->geometry.width - 15, 14);
      this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 46);
      this->UpdateScrollBar();
    }
    this->m_frameContext->snapping.Store(this);
    return 0;
  }

  case 23: {
    this->Entries().clear();
    std::vector<BarterMarketEntry>* items = g_expandedBarterMarketItemMgr->GetItems();
    for (std::vector<BarterMarketEntry>::iterator it = items->begin(); it != items->end(); ++it) {
      this->Entries().push_back(*it);
    }
    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }

  case 38: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    std::pair<bool, BarterMarketEntry> found = g_expandedBarterMarketItemMgr->FindEntry(mode->drag.unknown_03B0);
    if (found.first) {
      found.second.value = mode->drag.quantity;
      g_expandedBarterMarketItemMgr->RemoveEntry(found.second);
      g_windowMgr.LookupWindow(342)->SendMsg(nullptr, 23, 0, 0, 0, 0);
      this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    }
    mode->SendMsg(18, 0, 0, 0, 0);
    return 0;
  }

  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x00AE75C0
// FUNCTION: fn_18076
ITEM_INFO BarterMarketItem::ToItemInfo() const {
  ITEM_INFO item;
  item.unknown_0000[0] = this->field_1c;
  item.quantity = this->value;
  item.unknown_0014[0] = this->field_20;
  item.itemId = this->name;
  item.identified = 1;
  item.unknown_0000[2] = this->field_40;
  item.unknown_0070 = this->field_44;
  return item;
}

// OFFSET: 0x00AE7660
// FUNCTION: fn_18077
ITEM_INFO BarterMarketEntry::ToItemInfo() const {
  ITEM_INFO item;
  item.unknown_0000[0] = this->field_0;
  item.quantity = this->value;
  item.unknown_0014[0] = this->field_8;
  item.itemId = this->name;
  item.unknown_0060 = this->field_28;
  item.identified = 1;
  item.unknown_006C = static_cast<std::uint16_t>(this->factor);
  item.unknown_0000[2] = this->field_38;
  item.unknown_0070 = this->field_3c;
  return item;
}

// OFFSET: 0x00843190
// FUNCTION: fn_10556
void UIBarterItemPurchaseWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);
  if (previous != this->hovered) {
    this->Invalidate();
  }

  std::pair<bool, BarterMarketItem> found = this->GetBarterItemAt(x, y);
  if (found.first == true) {
    if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
      mode->drag.BeginBarterItem(4, found.second, found.second.value, -1);
    }
  }
}

// OFFSET: 0x00843270
// FUNCTION: fn_10557
void UIBartermarketWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  const int previous = this->hovered;
  int hovered = -1;
  if (x >= 40 && x < this->geometry.width - 20 && y >= 17 && y < this->visibleRows * 32 + 17) {
    const int column = std::min((x - 40) / 32, this->columns - 1);
    int visible = 0;
    int index = 0;
    std::list<BarterMarketItem> items = *g_barterMarketItemMgr->GetItems();
    for (std::list<BarterMarketItem>::iterator it = items.begin(); it != items.end(); ++it, ++index) {
      if (index >= this->scroll * this->columns) {
        if (visible == this->columns * ((y - 17) / 32) + column) {
          hovered = index;
          break;
        }
        ++visible;
      }
    }
  }
  this->hovered = hovered;
  if (previous != hovered) {
    this->Invalidate();
  }

  std::pair<bool, BarterMarketItem> found = this->GetBarterItemAt(x, y);
  if (found.first) {
    if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
      mode->drag.BeginBarterItem(31, found.second, 1, -1);
    }
  }
}

// OFFSET: 0x00843DE0
// FUNCTION: fn_10561
void UIBarterItemPurchaseWnd::Slot32(int x, int y) {
  std::pair<bool, BarterMarketItem> found = this->GetBarterItemOrPriceAt(x, y);
  if (found.first == true) {
    UIWindow* window = g_windowMgr.EnsureWindow(12);
    ITEM_INFO item = found.second.ToItemInfo();
    if (window) {
      window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00843EC0
// FUNCTION: fn_10562
void UIBartermarketWnd::Slot32(int x, int y) {
  std::pair<bool, BarterMarketItem> found = this->GetBarterItemOrPriceAt(x, y);
  if (found.first) {
    UIWindow* window = g_windowMgr.EnsureWindow(12);
    ITEM_INFO item = found.second.ToItemInfo();
    if (window) {
      window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x008481E0
// FUNCTION: fn_10587
void UIExpandedBarterItemPurchaseWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  const int previous = this->unknown_00CC;
  int hovered = -1;
  if (x >= 40 && x < this->geometry.width - 20 && y >= 17 && y < this->rowCount * 64 + 17) {
    const int row = (y - 17) / 64;
    int visible = 0;
    int index = 0;
    for (std::list<BarterMarketEntry>::iterator it = this->Entries().begin(); it != this->Entries().end(); ++it, ++index) {
      if (index >= this->unknown_00C4) {
        if (visible == row) {
          hovered = index;
          break;
        }
        ++visible;
      }
    }
  }
  this->unknown_00CC = hovered;
  if (previous != hovered) {
    this->Invalidate();
  }

  std::pair<bool, BarterMarketEntry> found = this->GetEntryAt(x, y);
  if (found.first == true) {
    if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
      mode->drag.BeginBarterEntry(32, found.second, found.second.value, -1);
    }
  }
}

// OFFSET: 0x00848320
// FUNCTION: fn_10588
void UIExpandedBartermarketWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  const int previous = this->unknown_00CC;
  int hovered = -1;
  if (x >= 40 && x < this->geometry.width - 20 && y >= 17 && y < this->rowCount * 64 + 17) {
    const int row = (y - 17) / 64;
    int visible = 0;
    int index = 0;
    for (std::list<BarterMarketEntry>::iterator it = this->Entries().begin(); it != this->Entries().end(); ++it, ++index) {
      if (index >= this->unknown_00C4) {
        if (visible == row) {
          hovered = index;
          break;
        }
        ++visible;
      }
    }
  }
  this->unknown_00CC = hovered;
  if (previous != hovered) {
    this->Invalidate();
  }

  std::pair<bool, BarterMarketEntry> found = this->GetEntryAt(x, y);
  if (found.first == true) {
    if (found.second.value == 0) {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3557)), 255, 0, 0, 0);
    } else if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
      mode->drag.BeginBarterEntry(31, found.second, 1, -1);
    }
  }
}

// OFFSET: 0x00848E70
// FUNCTION: fn_10592
void UIExpandedBarterItemPurchaseWnd::Slot32(int x, int y) {
  std::pair<bool, BarterMarketEntry> found = this->GetEntryOrChunkAt(x, y);
  if (found.first == true) {
    UIWindow* window = g_windowMgr.EnsureWindow(12);
    ITEM_INFO item = found.second.ToItemInfo();
    if (window) {
      window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00848F50
// FUNCTION: fn_10593
void UIExpandedBartermarketWnd::Slot32(int x, int y) {
  std::pair<bool, BarterMarketEntry> found = this->GetEntryOrChunkAt(x, y);
  if (found.first == true) {
    UIWindow* window = g_windowMgr.EnsureWindow(12);
    ITEM_INFO item = found.second.ToItemInfo();
    if (window) {
      window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00843420
// FUNCTION: fn_10558
void UIBarterItemPurchaseWnd::Slot28(int x, int y) {
  std::pair<bool, BarterMarketItem> found = this->GetBarterItemAt(x, y);
  if (found.first) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);
    std::string text;
    BarterMarketItem item = found.second;
    ITEM_DESC description = FindItemDescription(item.field_2c);
    text = FormatOwnedString(LookupMessage(3553), description.identifiedName, item.field_28 * item.value);
    ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode);
    ITEM_INFO info = item.ToItemInfo();
    if (tooltip) {
      tooltip->Show(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(info), 0, 1);
    }
  }
}

// OFFSET: 0x008436B0
// FUNCTION: fn_10559
void UIBartermarketWnd::Slot28(int x, int y) {
  std::pair<bool, BarterMarketItem> found = this->GetBarterItemAt(x, y);
  if (found.first) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);
    std::string text;
    ITEM_DESC description = FindItemDescription(found.second.field_2c);
    text = FormatOwnedString(LookupMessage(3553), description.identifiedName, found.second.field_28);
    ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode);
    ITEM_INFO info = found.second.ToItemInfo();
    if (tooltip) {
      tooltip->Show(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(info), 0, 1);
    }
  }
}

// OFFSET: 0x008484A0
// FUNCTION: fn_10589
void UIExpandedBarterItemPurchaseWnd::Slot28(int x, int y) {
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
      text += FormatOwnedString(LookupMessage(3553), name, chunk->amount * found.second.value);
      if (index != static_cast<int>(found.second.chunks.size())) {
        text += "\n";
      }
    }
    ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.GetGameMode());
    ITEM_INFO info = found.second.ToItemInfo();
    if (tooltip) {
      tooltip->Show(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(info), 0, 1);
    }
  }
}

// OFFSET: 0x00848B80
// FUNCTION: fn_10591
void UIExpandedBartermarketWnd::Slot28(int x, int y) {
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
      text += FormatOwnedString(LookupMessage(3553), name, chunk->amount);
      if (index != static_cast<int>(found.second.chunks.size())) {
        text += "\n";
      }
    }
    ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.GetGameMode());
    ITEM_INFO info = found.second.ToItemInfo();
    if (tooltip) {
      tooltip->Show(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(info), 0, 1);
    }
  }
}

// OFFSET: 0x00843910
// FUNCTION: fn_10560
void UIPurchaseButton::Slot28(int x, int y) {
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
      text += FormatOwnedString(LookupMessage(3553), description.identifiedName, amount);
    } else {
      std::string line = FormatOwnedString(LookupMessage(3553), description.identifiedName, amount);
      text += line + '\n';
    }
  }
  int tooltipX = 0;
  int tooltipY = -4 - count * 16;
  this->ToScreen(tooltipX, tooltipY);
  ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode);
  if (tooltip) {
    tooltip->Show(text.c_str(), tooltipX, tooltipY, 0xFFFFFFFF, 0, 1);
  }
}

// OFFSET: 0x00846940
// FUNCTION: fn_10582
std::string UIExpandedBarterPurchaseButton::FormatChunkText(int refine, std::string name, int amount) {
  std::string text;
  if (refine > 0) {
    char buffer[5] = {};
    _itoa(refine, buffer, 10);
    text += "+" + std::string(buffer) + " ";
  }
  char line[1024];
  std::memset(line, 0, sizeof(line));
  StringFormat(line, sizeof(line), static_cast<std::size_t>(-1), LookupMessage(3553), name.c_str(), amount);
  text += std::string(line);
  return text;
}

// OFFSET: 0x00848790
// FUNCTION: fn_10590
void UIExpandedBarterPurchaseButton::Slot28(int x, int y) {
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
    std::string total = FormatOwnedString(LookupMessage(3699), g_expandedBarterMarketItemMgr->total);
    text += "\n" + total;
  }
  if (!text.empty()) {
    int tooltipX = 0;
    int tooltipY = -4 - index * 16;
    this->ToScreen(tooltipX, tooltipY);
    ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode);
    if (tooltip) {
      tooltip->Show(text.c_str(), tooltipX, tooltipY, 0xFFFFFFFF, 0, 1);
    }
  }
}

// GLOBAL: gl_25056
int g_extraInventorySlots;
// GLOBAL: gl_25087
int g_barterValueA;
// GLOBAL: gl_25088
int g_barterValueB;

// OFFSET: 0x00840530
// FUNCTION: fn_10544
bool UIBarterItemPurchaseWnd::Purchase() {
  std::map<std::string, int> totals;
  std::list<BarterMarketItem> items = *g_barterMarketItemMgr->GetAllItems();

  int count = reinterpret_cast<const SessionMessengerContext*>(&g_session)->GetList0Size();
  count += g_session.CountItems17F0();
  count += g_session.CountItems2B50();

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

  if (count > g_extraInventorySlots + 100) {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3568)), 255, 0, 0, 0);
    this->SendMsg(nullptr, 6, 185, 0, 0, 0);
    return false;
  }

  std::map<int, int>* required = g_barterMarketItemMgr->GetTotals();
  for (std::map<int, int>::iterator it = required->begin(); it != required->end(); ++it) {
    if (this->GetInventoryItemCount(it->first) < it->second) {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3556)), 255, 0, 0, 0);
      this->SendMsg(nullptr, 6, 185, 0, 0, 0);
      return false;
    }
  }

  for (std::map<std::string, int>::iterator it = totals.begin(); it != totals.end(); ++it) {
    if (it->second + this->GetInventoryItemCount(std::atoi(it->first.c_str())) > 30000) {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      const std::string text = FormatOwnedString(LookupMessage(3567), 30000);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 255, 0, 0, 0);
      this->SendMsg(nullptr, 6, 185, 0, 0, 0);
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00841A10
// FUNCTION: fn_10552
void UIBarterItemPurchaseWnd::Slot15(int width, int height) {
  UIResizer* resizer = new UIResizer(*this->m_context);
  this->resizer = resizer;
  resizer->SetSize(16, 16);
  resizer->SetDirection(3);
  resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(resizer);

  UIScrollBar* scrollBar = new UIScrollBar(*this->m_context);
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

  this->SendMsg(nullptr, 23, 0, 0, 0, 0);
}

// OFFSET: 0x00843FA0
// FUNCTION: fn_10563
int UIBarterItemPurchaseWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (!mode) {
      return 0;
    }

    if (a == 184) {
      if (!this->Purchase()) {
        return 0;
      }
      mode->SendMsg(308, 0, 0, 0, 0);
    } else if (a != 185) {
      return 0;
    }

    mode->SendMsg(40, 0, 0, 0, 0);
    g_barterMarketItemMgr->Clear();
    return 0;
  }

  case 14: {
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

    this->m_frameContext->snapping.Store(this);
    return 0;
  }

  case 23: {
    this->items.clear();
    std::list<BarterMarketItem>* items = g_barterMarketItemMgr->GetAllItems();
    for (std::list<BarterMarketItem>::iterator it = items->begin(); it != items->end(); ++it) {
      this->items.push_back(it->ToItemInfo());
    }
    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }

  case 34: {
    this->saved = reinterpret_cast<ItemPurchaseWndInfo*>(a);
    if (!this->saved) {
      return 0;
    }
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->SendMsg(nullptr, 14, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  }

  case 38: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (!mode) {
      return 0;
    }

    std::pair<bool, BarterMarketItem> found = g_barterMarketItemMgr->FindItem(mode->drag.unknown_03B0);
    if (found.first == false) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3554)), 255, 0, 0, 0);
      mode->SendMsg(18, 0, 0, 0, 0);
      return 0;
    }

    found.second.value = mode->drag.quantity;
    if (g_session.HasItemId(found.second.field_2c)) {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3665)), 255, 0, 0, 0);
      mode->SendMsg(18, 0, 0, 0, 0);
      return 0;
    }

    if (g_barterMarketItemMgr->HasItemByName(mode->drag.name)) {
      int notice = 0;
      if (g_session.IsEquipItemType(mode->drag.type)) {
        mode->SendMsg(18, 0, 0, 0, 0);
        notice = 119;
      } else if (g_session.IsShadowItemType(mode->drag.type)) {
        mode->SendMsg(18, 0, 0, 0, 0);
        notice = 3569;
      }

      if (notice != 0) {
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(notice)), 255, 0, 0, 0);
        return 0;
      }
    }

    if (mode->drag.quantity + g_barterMarketItemMgr->GetAllItemValueByName(mode->drag.name) > 30000) {
      char buffer[200] = {};
      StringFormat(buffer, 200, static_cast<std::size_t>(-1), LookupMessage(1704), 30000);
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(buffer), 255, 0, 0, 0);
      return 0;
    }

    const int amountById = g_barterMarketItemMgr->GetAmountById(mode->drag.unknown_03B0);
    if (amountById < 0) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3554)), 255, 0, 0, 0);
      mode->SendMsg(18, 0, 0, 0, 0);
      return 0;
    }

    if (amountById > 0) {
      const int available = g_barterValueA - g_barterValueB;
      const float remaining = static_cast<float>(available) - g_barterMarketItemMgr->GetTotalAmount();
      if (static_cast<int>(remaining) / amountById < mode->drag.quantity) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(56)), 255, 0, 0, 0);
        mode->SendMsg(18, 0, 0, 0, 0);
        return 0;
      }
    }

    g_barterMarketItemMgr->AddItem(found.second);
    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    if (UIWindow* window = g_windowMgr.LookupWindow(334)) {
      window->SendMsg(nullptr, 23, 0, 0, 0, 0);
    }
    mode->SendMsg(18, 0, 0, 0, 0);
    return 0;
  }

  default:
    break;
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x00846FB0
// FUNCTION: fn_10583
void UIExpandedBarterItemPurchaseWnd::Slot15(int width, int height) {
  UIResizer* resizer = new UIResizer(*this->m_context);
  this->resizer = resizer;
  resizer->SetSize(16, 16);
  resizer->SetDirection(3);
  resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(resizer);

  UIScrollBar* scrollBar = new UIScrollBar(*this->m_context);
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 46);
  this->AddChild(scrollBar);

  UIExpandedBarterPurchaseButton* buyButton = new UIExpandedBarterPurchaseButton();
  this->buyButton = buyButton;
  buyButton->SetBitmap("유저인터페이스\\btn_buy.bmp", 0, 0);
  buyButton->SetBitmap("유저인터페이스\\btn_buy_a.bmp", 1, 0);
  buyButton->SetBitmap("유저인터페이스\\btn_buy_b.bmp", 2, 0);
  buyButton->SetSize(buyButton->BitmapWidth(), buyButton->BitmapHeight());
  buyButton->Move(this->geometry.width - 105, this->geometry.height - 24);
  buyButton->SetId(184);
  this->AddChild(buyButton);

  UIBitmapButton* cancelButton = new UIBitmapButton();
  this->cancelButton = cancelButton;
  cancelButton->SetBitmap("유저인터페이스\\btn_cancel.bmp", 0, 0);
  cancelButton->SetBitmap("유저인터페이스\\btn_cancel_a.bmp", 1, 0);
  cancelButton->SetBitmap("유저인터페이스\\btn_cancel_b.bmp", 2, 0);
  cancelButton->SetSize(cancelButton->BitmapWidth(), cancelButton->BitmapHeight());
  cancelButton->Move(this->geometry.width - 60, this->geometry.height - 24);
  cancelButton->SetId(185);
  this->AddChild(cancelButton);

  this->rowCount = (height - 46) / 64;

  this->SendMsg(nullptr, 23, 0, 0, 0, 0);
}

// OFFSET: 0x008446D0
// FUNCTION: fn_10564
int UIBartermarketWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 14: {
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
        const int contentHeight = this->geometry.height - this->unknown_0108 - this->unknown_0104 - 17;
        this->visibleRows = contentHeight / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, contentHeight);
        this->UpdateScrollBar();
      }

      this->m_frameContext->snapping.Store(this);
    }
    return 0;
  }

  case 23: {
    this->items.clear();
    std::list<BarterMarketItem>* items = g_barterMarketItemMgr->GetItems();
    for (std::list<BarterMarketItem>::iterator it = items->begin(); it != items->end(); ++it) {
      this->items.push_back(it->ToItemInfo());
    }
    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }

  case 34: {
    this->saved = reinterpret_cast<ItemShopWndInfo*>(a);
    if (a == 0) {
      return 0;
    }
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->SendMsg(nullptr, 14, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  }

  case 38: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (!mode) {
      return 0;
    }

    std::pair<bool, BarterMarketItem> found = g_barterMarketItemMgr->FindAllItem(mode->drag.unknown_03B0);
    if (found.first == false) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3554)), 255, 0, 0, 0);
      mode->SendMsg(18, 0, 0, 0, 0);
      return 0;
    }

    found.second.value = mode->drag.quantity;
    g_barterMarketItemMgr->RemoveItem(found.second);
    if (UIWindow* window = g_windowMgr.LookupWindow(335)) {
      window->SendMsg(nullptr, 23, 0, 0, 0, 0);
    }
    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    mode->SendMsg(18, 0, 0, 0, 0);
    return 0;
  }

  default:
    break;
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x00842570
// FUNCTION: fn_10553
void UIBartermarketWnd::Slot15(int width, int height) {
  this->resizer = new UIResizer(*this->m_context);
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(7);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  this->scrollBar = new UIScrollBar(*this->m_context);
  static_cast<UIScrollBar*>(this->scrollBar)->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - this->unknown_0104 - 17);
  this->AddChild(this->scrollBar);

  this->columns = 1;
  this->visibleRows = (height - this->unknown_0104 - 17) / 32;

  this->SendMsg(nullptr, 23, 0, 0, 0, 0);
}

// OFFSET: 0x00845220
// FUNCTION: fn_10575
bool UIExpandedBarterItemPurchaseWnd::Purchase() {
  std::map<std::string, int> totals;
  std::vector<BarterMarketEntry> entries = *g_expandedBarterMarketItemMgr->GetEntries();

  int count = reinterpret_cast<const SessionMessengerContext*>(&g_session)->GetList0Size();
  count += g_session.CountItems17F0();
  count += g_session.CountItems2B50();

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

  if (count > g_extraInventorySlots + 100) {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3568)), 255, 0, 0, 0);
    this->SendMsg(nullptr, 6, 185, 0, 0, 0);
    return false;
  }

  if (!g_expandedBarterMarketItemMgr->HasRequiredItems()) {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3556)), 255, 0, 0, 0);
    this->SendMsg(nullptr, 6, 185, 0, 0, 0);
    return false;
  }

  for (std::map<std::string, int>::iterator it = totals.begin(); it != totals.end(); ++it) {
    const int itemId = std::atoi(it->first.c_str());
    const int inventoryCount = reinterpret_cast<const SessionMessengerContext*>(&g_session)->GetList0Size();
    int owned = 0;
    for (int i = 0; i < inventoryCount; ++i) {
      ITEM_INFO item = g_session.GetInventoryItemAt(i);
      if (std::atoi(item.itemId.c_str()) == itemId) {
        owned = item.quantity;
        break;
      }
    }

    if (it->second + owned > 30000) {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      const std::string text = FormatOwnedString(LookupMessage(3691), 30000);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 255, 0, 0, 0);
      this->SendMsg(nullptr, 6, 185, 0, 0, 0);
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00847290
// FUNCTION: fn_10584
void UIExpandedBartermarketWnd::Slot15(int width, int height) {
  this->unknown_00BC = new UIResizer(*this->m_context);
  this->unknown_00BC->SetSize(16, 16);
  static_cast<UIResizer*>(this->unknown_00BC)->SetDirection(7);
  static_cast<UIResizer*>(this->unknown_00BC)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->unknown_00BC->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->unknown_00BC)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->unknown_00BC);

  this->scrollBar = new UIScrollBar(*this->m_context);
  this->scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 46);
  this->AddChild(this->scrollBar);

  this->rowCount = (height - 46) / 64;

  this->SendMsg(nullptr, 23, 0, 0, 0, 0);
}

// OFFSET: 0x008426E0
// FUNCTION: fn_10554
void UIBarterItemPurchaseWnd::Paint() {
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, false, LookupMessage(0xE69), 0);
  if (this->geometry.height == 17) {
    return;
  }

  const int borderHeight = this->geometry.height - 38;
  const int borderTiles = borderHeight / 8 + (borderHeight % 8 != 0);
  for (int tile = 0, y = 17; tile < borderTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, &bitmap->image, false);
  }
  for (int tile = 0, y = 17; tile < borderTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - bitmap->image.width, y, &bitmap->image, false);
  }

  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  const std::uint32_t background = RGB(red, green, blue);
  int rightRemainder = (this->geometry.width - 60) % 32;
  if (rightRemainder < 0) {
    rightRemainder += 32;
  }
  int bottomRemainder = (this->geometry.height - 38) % 32;
  if (bottomRemainder < 0) {
    bottomRemainder += 32;
  }
  this->FillRegion(20, 17, rightRemainder, this->geometry.height - 38, background);
  this->FillRegion(20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, bottomRemainder, background);
  this->DrawButtonBar(2, 0, 1);

  const int rows = (this->geometry.height - 38) / 32;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(40, 17, this->geometry.width - 50, rows * 32, RGB(red, green, blue));

  for (int row = 0; row < rows; ++row) {
    DrawItemSlotBackground(*this, 40, row * 32 + 17);
  }

  ItemDescriptions descriptions{g_itemDescriptions};
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
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
      this->FillRegion(x + 72, y + 19, 0xBF, 0x1C, RGB(red, green, blue));
    }

    const ITEM_INFO item = it->ToItemInfo();
    DrawItemWithQuantity(*this, this->m_controls->resources, descriptions, this->m_controls->fonts, this->m_controls->locale, x + 44, y + 21, item, 0, 0);
    this->DrawItemName(x + 76, y + 29, 100, &item, 0, 0, 12);
    const std::string iconId = std::to_string(it->field_2c);
    DrawItemIconById(*this, descriptions, this->geometry.width - 70, y + 21, iconId.c_str(), 1);
    if (it->field_28 > 0) {
      const std::string amount = std::to_string(it->value * it->field_28);
      this->DrawRightAlignedText(this->geometry.width - 25, y + 29, amount.c_str(), 0, 0, 12, 0, 0);
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }
}

// OFFSET: 0x00842BF0
// FUNCTION: fn_10555
void UIBartermarketWnd::Paint() {
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, false, LookupMessage(0xBA), 0);
  if (this->geometry.height == 17) {
    return;
  }

  const int leftHeight = this->geometry.height - this->unknown_0104 - 17;
  const int leftTiles = leftHeight / 8 + (leftHeight % 8 != 0);
  for (int tile = 0, y = 17; tile < leftTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, &bitmap->image, false);
  }
  const int rightHeight = this->geometry.height - this->unknown_0104 - 17;
  const int rightTiles = rightHeight / 8 + (rightHeight % 8 != 0);
  for (int tile = 0, y = 17; tile < rightTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - bitmap->image.width, y, &bitmap->image, false);
  }

  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  const std::uint32_t background = RGB(red, green, blue);
  const int contentHeight = this->geometry.height - this->unknown_0104 - 17;
  int rightRemainder = (this->geometry.width - 60) % 32;
  if (rightRemainder < 0) {
    rightRemainder += 32;
  }
  int bottomRemainder = contentHeight % 32;
  if (bottomRemainder < 0) {
    bottomRemainder += 32;
  }
  this->FillRegion(20, 17, rightRemainder, contentHeight, background);
  this->FillRegion(20, contentHeight / 32 * 32 + 17, this->geometry.width - 24, bottomRemainder, background);
  this->DrawButtonBar(2, 0, 1);

  const int rows = (this->geometry.height - this->unknown_0108 - this->unknown_0104 - 17) / 32;
  const int columns = (this->geometry.width - 40) / 32;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(72, 17, columns * 32 - 32, rows * 32, RGB(red, green, blue));
  for (int row = 0; row < rows; ++row) {
    DrawItemSlotBackground(*this, 40, row * 32 + 17);
  }

  ItemDescriptions descriptions{g_itemDescriptions};
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
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
      this->FillRegion(x + 72, y + 19, this->geometry.width - 0x58, 0x1C, RGB(red, green, blue));
    }

    const ITEM_INFO item = it->ToItemInfo();
    if (it->flag_38) {
      DrawItemIcon(*this, this->m_controls->resources, descriptions, x + 44, y + 21, item, 0);
    } else {
      DrawItemWithQuantity(*this, this->m_controls->resources, descriptions, this->m_controls->fonts, this->m_controls->locale, x + 44, y + 21, item, 0, 0);
    }
    this->DrawItemName(x + 76, y + 29, 120, &item, 0, 0, 12);
    const std::string iconId = std::to_string(it->field_2c);
    DrawItemIconById(*this, descriptions, this->geometry.width - 70, y + 21, iconId.c_str(), 1);
    if (it->field_28 > 0) {
      const std::string amount = std::to_string(it->field_28);
      this->DrawRightAlignedText(this->geometry.width - 25, y + 29, amount.c_str(), 0, 0, 12, 0, 0);
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }
}

// OFFSET: 0x008473F0
// FUNCTION: fn_10585
void UIExpandedBartermarketWnd::Paint() {
  this->DrawTitleBar(this->m_context->fonts, this->m_context->locale, false, LookupMessage(0xE69), 0);

  const int borderTiles = (this->geometry.height - 46 + 7) / 8;
  for (int tile = 0, y = 17; tile < borderTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, &bitmap->image, false);
  }
  for (int tile = 0, y = 17; tile < borderTiles; ++tile, y += 8) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - bitmap->image.width, y, &bitmap->image, false);
  }

  this->DrawButtonBar(2, 0, 1);
  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(20, 17, this->geometry.width - 40, this->geometry.height - 46, RGB(red, green, blue));
  this->DrawButtonBar(2, 0, 1);

  ItemDescriptions descriptions{g_itemDescriptions};
  int visible = 0;
  int index = 0;
  for (std::list<BarterMarketEntry>::iterator it = this->Entries().begin(); it != this->Entries().end(); ++it, ++index) {
    if (index < this->unknown_00C4) {
      continue;
    }

    const int y = visible * 64;
    DrawItemSlotBackground(*this, 40, y + 17);
    for (int slot = 0; slot < static_cast<int>(it->chunks.size()); ++slot) {
      DrawItemSlotBackground(*this, 72 + slot * 42, y + 49);
    }
    if (this->unknown_00CC == index) {
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
      this->FillRegion(72, y + 19, this->geometry.width - 0x5A, 0x3C, RGB(red, green, blue));
    }

    const ITEM_INFO item = it->ToItemInfo();
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), descriptions, this->m_context->fonts, this->m_context->locale, 44, y + 21, item, 0, 0);
    this->DrawItemName(76, y + 29, 100, &item, 0, 0, 12);

    if (it->field_28 > 0) {
      const std::string quantity = FormatOwnedString("+%d", it->field_28);
      DrawOutlinedText(*this, 50, y + 18, quantity.c_str(), 0, 0xFFFFFF, 200, 0, 12, 1, 0);
    }

    int chunkIndex = 0;
    for (std::vector<BarterChunk16>::const_iterator chunk = it->chunks.begin(); chunk != it->chunks.end(); ++chunk) {
      ITEM_INFO chunkItem;
      chunkItem.SetItemId(chunk->field_0);
      chunkItem.quantity = chunk->amount * it->value;
      chunkItem.unknown_0060 = chunk->field_4;
      const int chunkX = 84 + chunkIndex * 42;
      DrawItemWithQuantity(*this, ResourceMgr::Instance(), descriptions, this->m_context->fonts, this->m_context->locale, chunkX - 8, y + 49, chunkItem, 1, 0);
      if (chunk->field_4 > 0) {
        const std::string chunkAmount = FormatOwnedString("+%d", chunk->field_4);
        DrawOutlinedText(*this, chunkX, y + 49, chunkAmount.c_str(), 0, 0xFFFFFF, 200, 0, 12, 1, 0);
      }
      ++chunkIndex;
    }

    const std::string amount = FormatOwnedString("%d z", it->field_8 * it->value);
    this->DrawRightAlignedText(this->geometry.width - 20, y + 60, amount.c_str(), 0, 0, 12, 0, 0);

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

