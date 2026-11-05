#include "UI/MiniPartyGrid.h"

// GLOBAL: gl_24518
int g_miniPartyCellWidth = 177;
// GLOBAL: gl_24519
int g_miniPartyCellHeight = 55;

// OFFSET: 0x00737EC0
// FUNCTION: fn_07659
MiniPartyGridPoint* MiniPartyGrid::GetCellPosition(MiniPartyGridPoint* position, int index) const {
  position->x = this->left + g_miniPartyCellWidth * (index / this->rows);
  position->y = this->top + g_miniPartyCellHeight * (index % this->rows);

  return position;
}

// OFFSET: 0x00737EF0
// FUNCTION: fn_07660
MiniPartyGridPoint* MiniPartyGrid::GetWindowPosition(MiniPartyGridPoint* position, int index) const {
  position->x = this->left + g_miniPartyCellWidth * (index / this->rows);
  position->y = this->top + g_miniPartyCellHeight * (index % this->rows);

  return position;
}

// OFFSET: 0x00737F20
// FUNCTION: fn_07661
int MiniPartyGrid::FindCell(int x, int y) const {
  const int dx = x - this->left;
  int column = dx / g_miniPartyCellWidth;
  if (static_cast<double>(dx % g_miniPartyCellWidth) > static_cast<double>(g_miniPartyCellWidth) * 0.5) {
    ++column;
  }
  if (this->columns - 1 < column) {
    column = this->columns - 1;
  }
  if (column < 0) {
    column = 0;
  }

  const int dy = y - this->top;
  int row = dy / g_miniPartyCellHeight;
  if (static_cast<double>(dy % g_miniPartyCellHeight) > static_cast<double>(g_miniPartyCellHeight) * 0.5) {
    ++row;
  }
  if (this->rows - 1 < row) {
    row = this->rows - 1;
  }
  if (row < 0) {
    row = 0;
  }

  const int cell = this->rows * column + row;
  for (const int available : this->availableCells) {
    if (available == cell) {
      return cell;
    }
  }
  return -1;
}

// OFFSET: 0x00737FE0
// FUNCTION: fn_07662
void MiniPartyGrid::BuildAvailableCells() {
  this->columns = (this->right - this->left) / g_miniPartyCellWidth;
  this->rows = (this->bottom - this->top) / g_miniPartyCellHeight;

  for (int index = 0; index < this->columns * this->rows; ++index) {
    const int x = this->left + g_miniPartyCellWidth * (index / this->rows);
    const int y = this->top + g_miniPartyCellHeight * (index % this->rows);
    bool excluded = false;

    for (const MiniPartyGridRectangle& rectangle : this->exclusions) {
      if (x < rectangle.right && x + g_miniPartyCellWidth > rectangle.left && y < rectangle.bottom && y + g_miniPartyCellHeight > rectangle.top) {
        excluded = true;
        break;
      }
    }

    if (!excluded) {
      this->availableCells.push_back(index);
    }
  }
}
