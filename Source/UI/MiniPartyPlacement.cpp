#include "UI/MiniPartyPlacement.h"
#include "UI/MiniPartyGrid.h"
#include "UI/Window/MiniPartyWndAreaMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00737680
// FUNCTION: fn_07650
__forceinline CMiniPartyWndSortArea::CMiniPartyWndSortArea(const RECT& bounds) {
  SetRect(&this->area, 0, 0, g_windowMgr.screenWidth, g_windowMgr.screenHeight);
  this->columns = (this->area.right - this->area.left) / g_miniPartyCellWidth;
  this->rows = (this->area.bottom - this->area.top) / g_miniPartyCellHeight;

  const RECT target = bounds;

  for (int index = 0; index < this->columns * this->rows; ++index) {
    const int x = this->area.left + g_miniPartyCellWidth * (index / this->rows);
    const int y = this->area.top + g_miniPartyCellHeight * (index % this->rows);

    if (target.left <= x && x + g_miniPartyCellWidth <= target.right && target.top <= y && y + g_miniPartyCellHeight <= target.bottom) {
      this->cells.push_back(index);
    }
  }
}

// OFFSET: 0x00737830
// FUNCTION: fn_07651
void __stdcall Nullsub228() {
}

// OFFSET: 0x00A91870
// FUNCTION: fn_17165
UIWindow* UIWindowMgr::FindSnapExcludedWindow(int cell) {
  for (UIWindow* window : this->snapExcluded) {
    if (cell == *reinterpret_cast<int*>(reinterpret_cast<char*>(window) + 0x104)) {
      return window;
    }
  }
  return nullptr;
}

// OFFSET: 0x00737B00
// FUNCTION: fn_07657
int CMiniPartyWndAreaMgr::FindFreeCell() {
  CMiniPartyWndSortArea area(g_windowMgr.GetMiniPartyUsableArea());
  const std::list<int> cells = area.GetCells();

  for (std::vector<int>::iterator it = this->available.begin(); it != this->available.end(); ++it) {
    const std::list<int>::const_iterator found = std::find(cells.begin(), cells.end(), *it);

    if (found == cells.end()) {
      continue;
    }

    const UIWindow* const window = g_windowMgr.FindSnapExcludedWindow(*it);
    if (window == nullptr) {
      return *it;
    }
  }

  return -1;
}

// OFFSET: 0x00737D40
// FUNCTION: fn_07658
std::list<int> CMiniPartyWndSortArea::GetCells() const {
  return this->cells;
}
