#ifndef RAGNAROK_SOURCE_UI_PARTY_MINIPARTYGRID_H
#define RAGNAROK_SOURCE_UI_PARTY_MINIPARTYGRID_H

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

extern int g_miniPartyCellWidth;
extern int g_miniPartyCellHeight;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MiniPartyGridPoint) == 0x08);
static_assert(offsetof(MiniPartyGridPoint, y) == 0x04);
static_assert(sizeof(MiniPartyGridRectangle) == 0x10);
static_assert(offsetof(MiniPartyGridRectangle, bottom) == 0x0C);
#endif

#endif
