#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTGRID_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTGRID_H

struct ShortcutGrid {
  int hovered;
  int row;
  int rows;

  ShortcutGrid();
  int HitTest(int x, int y, bool itemSlotActive, unsigned char& currentRow);
};

static_assert(sizeof(ShortcutGrid) == 12);

#endif
