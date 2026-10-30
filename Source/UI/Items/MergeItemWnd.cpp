#include "UI/Items/MergeItemWnd.h"

#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdlib>
#include <windows.h>

struct MergeItemPacketHeader {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint16_t length;
};

// OFFSET: 0x007DEF00

// OFFSET: 0x007DEF80
// FUNCTION: fn_09662
void CMergeItemWnd::SelectSameItems(int id) {
  if (HIBYTE(GetAsyncKeyState(VK_CONTROL)) == 0) {
    return;
  }

  const int itemId = this->ItemIdOf(id);
  int checked = 0;
  for (auto it = this->m_items.begin(); it != this->m_items.end(); ++it) {
    if (itemId == std::atoi(LookupClientItemByIdentity(g_session.m_inventoryItems, (*it)->m_message).itemId.c_str())) {
      const std::uint32_t state = (*it)->geometry.unknown_0030;
      if ((*it)->m_message == id) {
        if (state == 0) {
          continue;
        }
      } else if (state != 0) {
        continue;
      }
      checked = 1;
      break;
    }
  }

  for (auto it = this->m_items.begin(); it != this->m_items.end(); ++it) {
    const int value = std::atoi(LookupClientItemByIdentity(g_session.m_inventoryItems, (*it)->m_message).itemId.c_str());
    (*it)->SetChecked(itemId == value ? checked : 0);
  }
}

// OFFSET: 0x007DF3D0
// FUNCTION: fn_09663
int CMergeItemWnd::ItemIdOf(int id) {
  return std::atoi(LookupClientItemByIdentity(g_session.m_inventoryItems, id).itemId.c_str());
}

// OFFSET: 0x007DFB20
// FUNCTION: fn_09667
void CMergeItemWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
    return;
  }

  const int id = this->ItemAt(x, y);
  for (auto it = this->m_items.begin(); it != this->m_items.end(); ++it) {
    if (id == (*it)->m_message) {
      (*it)->SetChecked((*it)->geometry.unknown_0030 == 0);
      break;
    }
  }
  this->SelectSameItems(id);
}

// OFFSET: 0x007DFD00
// FUNCTION: fn_09669
void CMergeItemWnd::Slot19() {
}

// OFFSET: 0x007DFD10
// FUNCTION: fn_09670
void CMergeItemWnd::Slot35(int delta) {
  if (this->m_scrollable != 0) {
    int step = this->m_rows - 1;
    if (step > 3) {
      step = 3;
    }
    if (delta >= 0) {
      step = -step;
    }
    this->SendMsg(this->m_scrollBar, 7, step, 0, 0, 0);
  }
}

// OFFSET: 0x007DFD60
// FUNCTION: fn_09671
int CMergeItemWnd::ItemAt(int x, int y) {
  const int column = (x - this->m_gridLeft) / this->m_cellSize;
  const int row = (y - this->m_gridTop) / this->m_cellSize;
  const int index = (this->m_scrollRow + row) * this->m_columns + column;
  if (index < this->m_itemCount) {
    const int dx = x - this->m_cellSize * column - this->m_slotOffsetX - this->m_slotRadius - this->m_gridLeft;
    const int dy = y - this->m_cellSize * row - this->m_slotOffsetY - this->m_slotRadius - this->m_gridTop;
    if (SquareRoot(static_cast<float>(dy * dy + dx * dx)) < static_cast<float>(this->m_slotRadius)) {
      UICheckBox* item = this->m_items[index];
      if (item != nullptr) {
        return item->m_message;
      }
    }
  }
  return -1;
}

// OFFSET: 0x007E0060
// FUNCTION: fn_09674
void CMergeItemWnd::FitToGrid(int width, int height) {
  if (height <= 0) {
    return;
  }

  const int cell = this->m_cellSize;
  const int left = this->m_gridLeft;
  const int minHeight = this->m_gridTop + cell * 3;
  const int maxHeight = this->m_gridTop + cell * 6;

  int columns = width - this->m_marginRight - left;
  columns -= columns % this->m_cellSize;
  int clamped = cell * 5;
  if (cell * 5 + left <= columns + left) {
    clamped = columns;
  }
  int gridWidth = cell * 10;
  if (clamped + left <= cell * 10 + left) {
    gridWidth = clamped;
  }

  int gridHeight = height - this->m_marginBottom - this->m_gridTop;
  gridHeight = gridHeight - gridHeight % this->m_cellSize + this->m_gridTop;
  if (gridHeight < minHeight) {
    gridHeight = minHeight;
  }
  if (maxHeight < gridHeight) {
    gridHeight = maxHeight;
  }

  if (gridWidth + left != this->geometry.width || gridHeight != this->geometry.height) {
    this->Resize(this->m_marginRight + gridWidth + left, this->m_marginBottom + gridHeight);
    const int visibleColumns = (this->geometry.width - this->m_marginRight - this->m_gridLeft) / this->m_cellSize;
    const int visibleRows = (this->geometry.height - this->m_marginBottom - this->m_gridTop) / this->m_cellSize;
    this->m_columns = visibleColumns;
    this->m_rows = visibleRows;
    this->m_visibleCount = visibleRows * visibleColumns;
  }
  this->m_frameContext->snapping.Store(this);
}

// OFFSET: 0x007E04B0
// FUNCTION: fn_09676
bool CMergeItemWnd::RequestMerge() {
  std::vector<std::uint16_t> ids;
  for (auto it = this->m_items.begin(); it != this->m_items.end(); ++it) {
    UICheckBox* item = *it;
    if (item != nullptr && item->geometry.unknown_0030 != 0) {
      ids.push_back(static_cast<std::uint16_t>(item->m_message));
    }
  }

  if (ids.size() < this->m_minSelection) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2170)), 0xFF, 0, 0, 0);
    return false;
  }

  std::vector<char> packet;
  MergeItemPacketHeader header{0x096E, 0};
  packet.insert(packet.end(), reinterpret_cast<const char*>(&header), reinterpret_cast<const char*>(&header + 1));
  packet.insert(packet.end(), reinterpret_cast<const char*>(ids.data()), reinterpret_cast<const char*>(ids.data() + ids.size()));
  reinterpret_cast<MergeItemPacketHeader*>(packet.data())->length = static_cast<std::uint16_t>(packet.size());
  GetRagConnection().Send(static_cast<int>(packet.size()), packet.data());
  return true;
}
