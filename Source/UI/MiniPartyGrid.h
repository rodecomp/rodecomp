#ifndef RAGNAROK_SOURCE_UI_MINIPARTYGRID_H
#define RAGNAROK_SOURCE_UI_MINIPARTYGRID_H

#include <cstddef>
#include <cstdint>
#include <vector>

struct MiniPartyGridPoint {
  /* 0000 */ int x;
  /* 0004 */ int y;
};

struct MiniPartyGridRectangle {
  /* 0000 */ int left;
  /* 0004 */ int top;
  /* 0008 */ int right;
  /* 000C */ int bottom;
};

struct MiniPartyGrid {
  MiniPartyGridPoint* GetCellPosition(MiniPartyGridPoint* position, int index) const;
  MiniPartyGridPoint* GetWindowPosition(MiniPartyGridPoint* position, int index) const;
  int FindCell(int x, int y) const;
  int BuildAvailableCells();

  /* 0000 */ std::uint32_t unknown_0000;
  /* 0004 */ int left;
  /* 0008 */ int top;
  /* 000C */ int right;
  /* 0010 */ int bottom;
  /* 0014 */ int columns;
  /* 0018 */ int rows;
  /* 001C */ std::vector<MiniPartyGridRectangle> exclusions;
  /* 0028 */ std::vector<int> availableCells;
};

extern int g_miniPartyCellWidth;
extern int g_miniPartyCellHeight;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MiniPartyGridPoint) == 0x08);
static_assert(offsetof(MiniPartyGridPoint, y) == 0x04);
static_assert(sizeof(MiniPartyGridRectangle) == 0x10);
static_assert(offsetof(MiniPartyGridRectangle, bottom) == 0x0C);
static_assert(offsetof(MiniPartyGrid, exclusions) == 0x1C);
static_assert(offsetof(MiniPartyGrid, availableCells) == 0x28);
static_assert(sizeof(MiniPartyGrid) == 0x34);
#endif

#endif
