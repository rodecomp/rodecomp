#ifndef RAGNAROK_SOURCE_UI_MINIPARTYPLACEMENT_H
#define RAGNAROK_SOURCE_UI_MINIPARTYPLACEMENT_H

#include <cstddef>
#include <list>
#include <windows.h>

class CMiniPartyWndSortArea {
public:
  /* 0004 */ RECT area;
  /* 0014 */ int columns;
  /* 0018 */ int rows;
  /* 001C */ std::list<int> cells;

  virtual ~CMiniPartyWndSortArea() = default;

  explicit CMiniPartyWndSortArea(const RECT& bounds);
  std::list<int> GetCells() const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMiniPartyWndSortArea, area) == 0x04);
static_assert(offsetof(CMiniPartyWndSortArea, cells) == 0x1C);
static_assert(sizeof(CMiniPartyWndSortArea) == 0x24);
#endif

#endif
