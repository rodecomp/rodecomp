#ifndef RAGNAROK_SOURCE_UI_WINDOW_MINIPARTYWNDAREAMGR_H
#define RAGNAROK_SOURCE_UI_WINDOW_MINIPARTYWNDAREAMGR_H

#include <cstddef>
#include <vector>
#include <windows.h>

class CMiniPartyWndAreaMgr {
public:
  static CMiniPartyWndAreaMgr* Instance();

  /* 0004 */ RECT area;
  /* 0014 */ int columns;
  /* 0018 */ int rows;
  /* 001C */ std::vector<RECT> excluded;
  /* 0028 */ std::vector<int> available;

  // INLINED
  virtual ~CMiniPartyWndAreaMgr() = default;

  CMiniPartyWndAreaMgr();
};

extern CMiniPartyWndAreaMgr* g_miniPartyWndAreaMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMiniPartyWndAreaMgr, excluded) == 0x1C);
static_assert(offsetof(CMiniPartyWndAreaMgr, available) == 0x28);
static_assert(sizeof(CMiniPartyWndAreaMgr) == 0x34);
#endif

#endif
