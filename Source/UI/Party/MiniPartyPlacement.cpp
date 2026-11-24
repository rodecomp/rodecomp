#include "UI/Party/MiniPartyPlacement.h"
#include "UI/Party/MiniPartyGrid.h"
#include "UI/Party/MiniPartyWndAreaMgr.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

// INLINED
static bool ContainsCell(const RECT& bounds, const MiniPartyGridPoint& position) {
  return bounds.left <= position.x && position.x + g_miniPartyCellWidth <= bounds.right && bounds.top <= position.y && position.y + g_miniPartyCellHeight <= bounds.bottom;
}

// OFFSET: 0x00737680
// FUNCTION: fn_07650
__forceinline CMiniPartyWndSortArea::CMiniPartyWndSortArea(const RECT& bounds) {
  SetRect(&this->area, 0, 0, g_windowMgr.screenWidth, g_windowMgr.screenHeight);
  this->columns = (this->area.right - this->area.left) / g_miniPartyCellWidth;
  this->rows = (this->area.bottom - this->area.top) / g_miniPartyCellHeight;

  const RECT target = bounds;
  for (int index = 0; index < this->columns * this->rows; ++index) {
    MiniPartyGridPoint position;
    position.x = this->area.left + g_miniPartyCellWidth * (index / this->rows);
    position.y = this->area.top + g_miniPartyCellHeight * (index % this->rows);
    if (ContainsCell(target, position)) {
      this->cells.push_back(index);
    }
  }
}

// OFFSET: 0x007379A0
// FUNCTION: fn_07653
__declspec(noinline) CMiniPartyWndSortArea::~CMiniPartyWndSortArea() {
}

// OFFSET: 0x00737830
// FUNCTION: fn_07651
void __stdcall Nullsub228() {
}

// OFFSET: 0x00A91870
// FUNCTION: fn_17165
UIWindow* UIWindowMgr::FindSnapExcludedWindow(int cell) {
  for (UIWindow* window : this->snapExcluded) {
    if (cell == static_cast<UIMiniPartyWnd*>(window)->m_cell) {
      return window;
    }
  }
  return nullptr;
}

// OFFSET: 0x00737B00
// FUNCTION: fn_07657
int CMiniPartyWndAreaMgr::FindFreeCell() {
  CMiniPartyWndSortArea area(g_windowMgr.GetMiniPartyUsableArea());
  const std::list<int>& cells = area.GetCells();

  const auto end = this->available.end();
  auto it = this->available.begin();
  for (; it != end; ++it) {
    if (std::find(cells.begin(), cells.end(), *it) != cells.end() && g_windowMgr.FindSnapExcludedWindow(*it) == nullptr) {
      break;
    }
  }

  return it != end ? *it : -1;
}
