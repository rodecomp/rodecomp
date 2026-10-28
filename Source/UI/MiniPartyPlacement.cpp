#include "UI/MiniPartyPlacement.h"
#include "UI/MiniPartyGrid.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00737680
// FUNCTION: fn_07650
CMiniPartyWndSortArea::CMiniPartyWndSortArea(const RECT& bounds) {
  SetRect(&this->area, 0, 0, g_windowMgr.unknown_047C, g_windowMgr.unknown_0480);
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

// OFFSET: 0x00737D40
// FUNCTION: fn_07658
std::list<int> CMiniPartyWndSortArea::GetCells() const {
  return this->cells;
}
