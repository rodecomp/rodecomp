#include "UI/Party/MiniPartyPlacement.h"
#include "UI/Party/MiniPartyGrid.h"
#include "UI/Party/MiniPartyWndAreaMgr.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00737680
// FUNCTION: fn_07650
__forceinline CMiniPartyWndSortArea::CMiniPartyWndSortArea(const RECT& bounds) {
  SetRect(&this->area, 0, 0, g_windowMgr.screenWidth, g_windowMgr.screenHeight);
  this->columns = (this->area.right - this->area.left) / g_miniPartyCellWidth;
  this->rows = (this->area.bottom - this->area.top) / g_miniPartyCellHeight;

  const RECT target = bounds;
  int rows = this->rows;
  int height = g_miniPartyCellHeight;

  for (int index = 0; index < this->columns * rows; ++index) {
    const int x = this->area.left + g_miniPartyCellWidth * (index / rows);
    const int y = this->area.top + height * (index % rows);

    if (target.left <= x && x + g_miniPartyCellWidth <= target.right && target.top <= y && y + height <= target.bottom) {
      this->cells.push_back(index);
      height = g_miniPartyCellHeight;
    }
    rows = this->rows;
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

  for (const int& cell : this->available) {
    if (std::find(cells.begin(), cells.end(), cell) == cells.end()) {
      continue;
    }

    if (g_windowMgr.FindSnapExcludedWindow(cell) == nullptr) {
      return cell;
    }
  }

  return -1;
}

// OFFSET: 0x00737D40
// FUNCTION: fn_07658
std::list<int> CMiniPartyWndSortArea::GetCells() const {
  return this->cells;
}
