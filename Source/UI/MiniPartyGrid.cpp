#include "UI/MiniPartyGrid.h"

// GLOBAL: gl_24518
int g_miniPartyCellWidth = 177;
// GLOBAL: gl_24519
int g_miniPartyCellHeight = 55;

// FUNCTION: fn_07659
MiniPartyGridPoint* MiniPartyGrid::GetCellPosition(MiniPartyGridPoint* position, int index) const {
  const int y = this->top + g_miniPartyCellHeight * (index % this->rows);
  position->x = this->left + g_miniPartyCellWidth * (index / this->rows);
  position->y = y;
  return position;
}

// FUNCTION: fn_07660
MiniPartyGridPoint* MiniPartyGrid::GetWindowPosition(MiniPartyGridPoint* position, int index) const {
  const int y = this->top + g_miniPartyCellHeight * (index % this->rows);
  position->x = this->left + g_miniPartyCellWidth * (index / this->rows);
  position->y = y;
  return position;
}

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

// FUNCTION: fn_07662
int MiniPartyGrid::BuildAvailableCells() {
  int index = 0;
  this->columns = (this->right - this->left) / g_miniPartyCellWidth;
  int rowCount = (this->bottom - this->top) / g_miniPartyCellHeight;
  this->rows = rowCount;
  int result = rowCount;
  if (this->columns * rowCount > 0) {
    int cellWidth = g_miniPartyCellWidth;
    do {
      const int x = this->left + cellWidth * (index / rowCount);
      const int y = this->top + g_miniPartyCellHeight * (index % rowCount);
      bool excluded = false;
      for (const MiniPartyGridRectangle& rectangle : this->exclusions) {
        if (x < rectangle.right && x + cellWidth > rectangle.left && y < rectangle.bottom && y + g_miniPartyCellHeight > rectangle.top) {
          excluded = true;
          break;
        }
      }
      if (!excluded) {
        this->availableCells.push_back(index);
        cellWidth = g_miniPartyCellWidth;
      }
      rowCount = this->rows;
      result = rowCount * this->columns;
      ++index;
    } while (index < result);
  }
  return result;
}
