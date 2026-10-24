#ifndef RAGNAROK_SOURCE_UI_SHORTCUTGRID_H
#define RAGNAROK_SOURCE_UI_SHORTCUTGRID_H

// UIShortCutWnd contiguous fields+340..+351. The current-row byte lives in
// original client state and is supplied explicitly, not duplicated globally.
struct ShortcutGrid {
  int hovered;
  int row;
  int rows;

  ShortcutGrid();
  int HitTest(int x, int y, bool itemSlotActive, unsigned char& currentRow);
};

static_assert(sizeof(ShortcutGrid) == 12);

#endif
