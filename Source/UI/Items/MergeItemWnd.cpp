#include "UI/Window/WindowId.h"
#include "Render/Text/FontCache.h"
#include "UI/Window/UIEnums.h"
#include "Render/Text/TextContext.h"
#include "UI/Items/MergeItemWnd.h"

#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <cstdlib>
#include <string>
#include <windows.h>

struct MergeItemPacketHeader {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint16_t length;
};

struct MergeItemCancelPacket {
  /* 0000 */ std::int16_t id;
};


// OFFSET: 0x007DEC20
// FUNCTION: fn_09659
CMergeItemWnd::CMergeItemWnd()
    : UIFrameWnd(0), m_cellSize(35), m_gridTop(20), m_gridLeft(10),
      m_marginRight(10), m_marginBottom(30), m_slotOffsetX(5), m_slotOffsetY(2), m_checkOffsetX(0), m_checkOffsetY(20), m_slotRadius(10), m_minSelection(2),
      m_mergeButton(nullptr), m_cancelButton(nullptr), m_closeButton(nullptr), m_items(), m_resizer(nullptr), m_scrollBar(nullptr), m_itemCount(0), m_columns(0),
      m_rows(0), m_visibleCount(0), m_scrollRow(0), m_scrollable(0) {}

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
      const std::uint32_t state = (*it)->geometry.state;
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

// INLINED
CMergeItemWnd::~CMergeItemWnd() {
}


// OFFSET: 0x007DF3D0
// FUNCTION: fn_09663
int CMergeItemWnd::ItemIdOf(int id) {
  return std::atoi(LookupClientItemByIdentity(g_session.m_inventoryItems, id).itemId.c_str());
}

// OFFSET: 0x007DF580
// FUNCTION: fn_09664
void CMergeItemWnd::CreateControls() {
  this->m_mergeButton = new UIBitmapButton();
  this->m_mergeButton->SetBitmap("유저인터페이스\\btn_ok.bmp", 0, 0);
  this->m_mergeButton->SetBitmap("유저인터페이스\\btn_ok_a.bmp", 1, 0);
  this->m_mergeButton->SetBitmap("유저인터페이스\\btn_ok_b.bmp", 2, 0);
  this->m_mergeButton->SetSize(this->m_mergeButton->BitmapWidth(), this->m_mergeButton->BitmapHeight());
  this->m_mergeButton->SetId(184);
  this->AddChild(this->m_mergeButton);

  this->m_cancelButton = new UIBitmapButton();
  this->m_cancelButton->SetBitmap("유저인터페이스\\btn_cancel.bmp", 0, 0);
  this->m_cancelButton->SetBitmap("유저인터페이스\\btn_cancel_a.bmp", 1, 0);
  this->m_cancelButton->SetBitmap("유저인터페이스\\btn_cancel_b.bmp", 2, 0);
  this->m_cancelButton->SetSize(this->m_cancelButton->BitmapWidth(), this->m_cancelButton->BitmapHeight());
  this->m_cancelButton->SetId(185);
  this->AddChild(this->m_cancelButton);

  this->m_closeButton = this->AddCloseButton("", 240);

  this->m_resizer = new UIResizer();
  this->m_resizer->SetSize(16, 16);
  this->m_resizer->SetDirection(7);
  this->m_resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->m_resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->m_resizer);

  this->m_scrollBar = new UIScrollBar();
  this->m_scrollBar->Configure(1, 1, 1, 1);
  this->AddChild(this->m_scrollBar);
}

// OFFSET: 0x007DF830
// FUNCTION: fn_09665
void CMergeItemWnd::OnCreate(int, int) {
  this->CreateControls();
  this->FitToGrid(this->geometry.width, this->geometry.height);
}

// OFFSET: 0x007DF850
// FUNCTION: fn_09666
void CMergeItemWnd::Paint() {
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->DrawTitleBar(true, LookupMessage(MSI_MERGE_ITEM_FAILED_NOT_MERGE), 0);
  this->FillRegion(0, 17, this->geometry.width, this->geometry.height, RGB(red, green, blue));

  for (int index = 0; index < this->m_visibleCount; ++index) {
    const int columns = (this->geometry.width - this->m_marginRight - this->m_gridLeft) / this->m_cellSize;
    DrawItemSlotBackground(*this, this->m_cellSize * (index % columns) + this->m_gridLeft, this->m_cellSize * (index / columns) + this->m_gridTop);
  }

  const int first = this->m_scrollRow * this->m_columns;
  for (auto it = this->m_items.begin(); it != this->m_items.end(); ++it) {
    (*it)->SetHidden(true);
  }

  for (int index = 0; index < this->m_visibleCount; ++index) {
    if (first + index >= this->m_itemCount) {
      break;
    }

    const int columns = (this->geometry.width - this->m_marginRight - this->m_gridLeft) / this->m_cellSize;
    const int y = this->m_cellSize * (index / columns) + this->m_gridTop;
    const int x = this->m_cellSize * (index % columns) + this->m_gridLeft;
    UICheckBox* box = this->m_items[first + index];
    if (box != nullptr) {
      ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, box->m_message);
      if (item.identity[1] != 0) {
        box->SetHidden(false);
        box->Move(this->m_checkOffsetX + x, this->m_checkOffsetY + y);
        DrawItemWithQuantity(
          *this, ResourceMgr::Instance(), this->m_slotOffsetX + x, this->m_slotOffsetY + y, item, 0, 0
        );
      }
    }
  }

  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x007DFB20
// FUNCTION: fn_09667
void CMergeItemWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
    return;
  }

  const int id = this->ItemAt(x, y);
  for (auto it = this->m_items.begin(); it != this->m_items.end(); ++it) {
    if (id == (*it)->m_message) {
      (*it)->SetChecked((*it)->geometry.state == 0);
      break;
    }
  }
  this->SelectSameItems(id);
}

// OFFSET: 0x007DFBA0
// FUNCTION: fn_09668
void CMergeItemWnd::OnMouseHover(int x, int y) {
  const int id = this->ItemAt(x, y);
  int screenX = x;
  int screenY = y;
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, id);
  if (item.identity[1] != 0) {
    this->ToScreen(screenX, screenY);
    std::string text = FormatOwnedString(LookupMessage(MSI_EA2), item.DisplayName(0).c_str(), item.quantity);
    g_modeMgr.gameMode->ShowTooltip(text.c_str(), screenX, screenY, g_session.GetItemNameColor(item), 0, 1);
  }
}

// OFFSET: 0x007DFD00
// FUNCTION: fn_09669
void CMergeItemWnd::OnProcess() {
}

// OFFSET: 0x007DFD10
// FUNCTION: fn_09670
void CMergeItemWnd::OnWheel(int delta) {
  if (this->m_scrollable != 0) {
    int step = this->m_rows - 1;
    if (step > 3) {
      step = 3;
    }
    if (delta >= 0) {
      step = -step;
    }
    this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, step, 0, 0, 0);
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

// OFFSET: 0x007DFF10
// FUNCTION: fn_09672
void CMergeItemWnd::UpdateScroll() {
  if (this->m_scrollBar == nullptr) {
    return;
  }

  const int columns = (this->geometry.width - this->m_marginRight - this->m_gridLeft) / this->m_cellSize;
  const int rows = this->m_itemCount / columns;
  const int total = rows > 0 ? rows + 1 : rows;
  int maximum = total - this->m_rows;
  if (maximum <= 0) {
    maximum = 0;
    this->m_scrollRow = 0;
  } else if (this->m_scrollRow < 0) {
    this->m_scrollRow = 0;
  } else if (this->m_scrollRow > maximum) {
    this->m_scrollRow = maximum;
  }
  this->m_scrollable = maximum;
  this->m_scrollBar->SetRange(maximum, this->m_rows, total);
  this->m_scrollBar->SetPosition(this->m_scrollRow);
  this->m_scrollBar->Invalidate();
}

// OFFSET: 0x007DFFC0
// FUNCTION: fn_09673
void CMergeItemWnd::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->m_mergeButton->Move(width - 110, height - 25);
  this->m_cancelButton->Move(width - 60, height - 25);
  this->m_closeButton->Move(width - 14, 3);
  this->m_resizer->Move(width - 15, height - 15);
  this->m_scrollBar->Move(width - 15, 17);
  this->m_scrollBar->Resize(this->m_scrollBar->geometry.width, this->geometry.height - 38);
  this->UpdateScroll();
}

// OFFSET: 0x007E0060
// FUNCTION: fn_09674
void CMergeItemWnd::FitToGrid(int width, int height) {
  if (height <= 0) {
    return;
  }

  const int minWidth = this->m_cellSize * 5;
  const int maxWidth = this->m_cellSize * 10;
  const int minHeight = this->m_gridTop + this->m_cellSize * 3;
  const int maxHeight = this->m_gridTop + this->m_cellSize * 6;

  int columns = width - this->m_marginRight - this->m_gridLeft;
  columns -= columns % this->m_cellSize;
  int clamped = minWidth;
  if (this->m_gridLeft + minWidth <= this->m_gridLeft + columns) {
    clamped = columns;
  }
  int gridWidth = maxWidth;
  if (this->m_gridLeft + clamped <= this->m_gridLeft + maxWidth) {
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

  if (gridWidth + this->m_gridLeft != this->geometry.width || gridHeight != this->geometry.height) {
    this->Resize(this->m_marginRight + gridWidth + this->m_gridLeft, this->m_marginBottom + gridHeight);
    const int visibleColumns = (this->geometry.width - this->m_marginRight - this->m_gridLeft) / this->m_cellSize;
    const int visibleRows = (this->geometry.height - this->m_marginBottom - this->m_gridTop) / this->m_cellSize;
    this->m_columns = visibleColumns;
    this->m_rows = visibleRows;
    this->m_visibleCount = visibleRows * visibleColumns;
  }
  g_windowMgr.StoreSnapRectangle(this);
}

// OFFSET: 0x007E01E0
// FUNCTION: fn_09675
int CMergeItemWnd::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t) {
  switch (message) {
  case UM_SETINFO: {
    this->m_itemCount = argument2;
    const std::int16_t* ids = reinterpret_cast<const std::int16_t*>(argument1);
    for (int index = 0; index < this->m_itemCount; ++index) {
      UICheckBox* box = new UICheckBox();
      box->SetSize(120, 12);
      box->SetBitmaps("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp");
      box->m_message = ids[index];
      box->SetHidden(true);
      box->m_notify = 1;
      this->m_items.push_back(box);
      this->AddChild(box);
    }
    this->FitToGrid(this->geometry.width, this->geometry.height);
    break;
  }
  case UM_REFRESH: {
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, argument1);
    g_session.RemoveInventoryItem(item.identity[1], item.quantity);
    item.quantity = argument2;
    g_session.AddInventoryItem(item, 1);
    break;
  }
  case UM_COMMAND:
    switch (argument1) {
    case ID_OK:
      this->RequestMerge();
      break;
    case ID_CANCEL:
    case ID_CLOSE: {
      MergeItemCancelPacket packet;
      packet.id = ID_CZ_CANCEL_MERGE_ITEM;
      GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
      g_windowMgr.CloseWindow(WID_MERGEITEMWND);
      break;
    }
    }
    break;
  case UM_RESIZE:
    if (argument1 == 7) {
      this->FitToGrid(argument2, argument3);
    }
    break;
  case UM_SCROLLVERT:
    if (argument1 != 0) {
      this->m_scrollRow += argument1;
      this->UpdateScroll();
      this->Invalidate();
    }
    break;
  case UM_TOGGLEPUSH:
    this->SelectSameItems(argument1);
    break;
  }
  return 0;
}

// OFFSET: 0x007E04B0
// FUNCTION: fn_09676
bool CMergeItemWnd::RequestMerge() {
  std::vector<std::uint16_t> ids;
  for (auto it = this->m_items.begin(); it != this->m_items.end(); ++it) {
    UICheckBox* item = *it;
    if (item != nullptr && item->geometry.state != 0) {
      ids.push_back(static_cast<std::uint16_t>(item->m_message));
    }
  }

  if (ids.size() < this->m_minSelection) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_MERGE_ITEM_FAILED_MAX_COUNT)), 0xFF, 0, 0, 0);
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
