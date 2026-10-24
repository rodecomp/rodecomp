#include "UI/ShortcutGrid.h"

#include <bit>
#include <cstdint>

// OFFSET: 0x009122B0
ShortcutGrid::ShortcutGrid() : hovered(-1), row(-1), rows(1) {
}

// OFFSET: 0x0091C540
// FUNCTION: fn_12616
int ShortcutGrid::HitTest(int x, int y, bool itemSlotActive, unsigned char& currentRow) {
  this->row = -1;
  std::uint32_t extent = 33u * static_cast<std::uint32_t>(this->rows);
  if (itemSlotActive) {
    extent += 33;
  }
  if (y <= 3 || y >= std::bit_cast<std::int32_t>(extent)) {
    return -1;
  }
  const std::uint32_t columnOffset = static_cast<std::uint32_t>(x) - 15;
  if (columnOffset > 260) {
    return -1;
  }
  this->row = (y - 4) / 33;
  currentRow = static_cast<unsigned char>(this->row);
  if (y > this->row * 33 + 30) {
    this->row = -1;
  }
  const int column = static_cast<int>(columnOffset) / 29;
  if (x > column * 29 + 41) {
    return -1;
  }
  return column;
}
