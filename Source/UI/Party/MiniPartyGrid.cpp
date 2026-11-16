#include "UI/Party/MiniPartyGrid.h"
#include "UI/Party/MiniPartyPlacement.h"
#include "UI/Party/MiniPartyWndAreaMgr.h"

#include <algorithm>

// GLOBAL: gl_24518
int g_miniPartyCellWidth = 177;
// GLOBAL: gl_24519
int g_miniPartyCellHeight = 55;

// OFFSET: 0x00737EC0
// FUNCTION: fn_07659
MiniPartyGridPoint CMiniPartyWndAreaMgr::GetCellPosition(int index) const {
  MiniPartyGridPoint position;
  position.x = this->area.left + g_miniPartyCellWidth * (index / this->rows);
  position.y = this->area.top + g_miniPartyCellHeight * (index % this->rows);
  return position;
}

// OFFSET: 0x00737EF0
// FUNCTION: fn_07660
MiniPartyGridPoint CMiniPartyWndSortArea::GetWindowPosition(int index) const {
  MiniPartyGridPoint position;
  position.x = this->area.left + g_miniPartyCellWidth * (index / this->rows);
  position.y = this->area.top + g_miniPartyCellHeight * (index % this->rows);
  return position;
}

// OFFSET: 0x00737F20
// FUNCTION: fn_07661
int CMiniPartyWndAreaMgr::FindCell(int x, int y) const {
  int column = (x - this->area.left) / g_miniPartyCellWidth;
  if (static_cast<double>((x - this->area.left) % g_miniPartyCellWidth) > static_cast<double>(g_miniPartyCellWidth) * 0.5) {
    ++column;
  }
  column = (std::max)(0, (std::min)(column, this->columns - 1));

  int row = (y - this->area.top) / g_miniPartyCellHeight;
  if (static_cast<double>((y - this->area.top) % g_miniPartyCellHeight) > static_cast<double>(g_miniPartyCellHeight) * 0.5) {
    ++row;
  }
  row = (std::max)(0, (std::min)(row, this->rows - 1));

  const int cell = this->rows * column + row;
  for (const int entry : this->available) {
    if (entry == cell) {
      return cell;
    }
  }
  return -1;
}

// OFFSET: 0x00737FE0
// FUNCTION: fn_07662
void CMiniPartyWndAreaMgr::BuildAvailableCells() {
  this->columns = (this->area.right - this->area.left) / g_miniPartyCellWidth;
  this->rows = (this->area.bottom - this->area.top) / g_miniPartyCellHeight;

  for (int index = 0; index < this->columns * this->rows; ++index) {
    const int x = this->area.left + g_miniPartyCellWidth * (index / this->rows);
    const int y = this->area.top + g_miniPartyCellHeight * (index % this->rows);
    bool excluded = false;

    for (const RECT& rectangle : this->excluded) {
      if (x < rectangle.right && x + g_miniPartyCellWidth > rectangle.left && y < rectangle.bottom && y + g_miniPartyCellHeight > rectangle.top) {
        excluded = true;
        break;
      }
    }

    if (!excluded) {
      this->available.push_back(index);
    }
  }
}
