#ifndef RAGNAROK_SOURCE_UI_MINIPARTYGRID_H
#define RAGNAROK_SOURCE_UI_MINIPARTYGRID_H

#include <cstddef>
#include <cstdint>
#include <vector>

struct MiniPartyGridPoint {
  int x;
  int y;
};

struct MiniPartyGridRectangle {
  int left;
  int top;
  int right;
  int bottom;
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
static_assert(offsetof(MiniPartyGrid, exclusions) == 0x1C);
static_assert(offsetof(MiniPartyGrid, availableCells) == 0x28);
static_assert(sizeof(MiniPartyGrid) == 0x34);
#endif

#endif
