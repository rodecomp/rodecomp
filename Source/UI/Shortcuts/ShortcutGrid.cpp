#include "UI/Shortcuts/UIShortCutWnd.h"
#include "Client/Session/Session.h"

#include <bit>
#include <cstdint>

// OFFSET: 0x0091C540
// FUNCTION: fn_12616
int UIShortCutWnd::HitTestSlot(int x, int y) {
  ShortcutGrid& grid = this->m_grid;
  grid.row = -1;
  int extent = 33 * grid.rows;
  if (g_session.ItemSlotActive()) {
    extent += 33;
  }

  if (y <= 3 || y >= extent) {
    return -1;
  }

  const std::uint32_t columnOffset = static_cast<std::uint32_t>(x) - 15;
  if (columnOffset > 260) {
    return -1;
  }

  grid.row = (y - 4) / 33;
  g_session.m_hotkeyPage = static_cast<unsigned char>(grid.row);
  if (y > grid.row * 33 + 30) {
    grid.row = -1;
  }

  const int column = static_cast<int>(columnOffset) / 29;
  if (x > column * 29 + 41) {
    return -1;
  }

  return column;
}
