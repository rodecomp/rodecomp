#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTGRID_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTGRID_H

#include <cstddef>

struct ShortcutGrid {
  /* 0154 */ int hovered;
  /* 0158 */ int row;
  /* 015C */ int rows;

  ShortcutGrid();
  int HitTest(int x, int y, bool itemSlotActive, unsigned char& currentRow);
};

static_assert(sizeof(ShortcutGrid) == 12);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(ShortcutGrid, hovered) + 0x0154 == 0x0154);
static_assert(offsetof(ShortcutGrid, row) + 0x0154 == 0x0158);
static_assert(offsetof(ShortcutGrid, rows) + 0x0154 == 0x015C);
#endif

#endif
