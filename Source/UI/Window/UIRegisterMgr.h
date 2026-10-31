#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIREGISTERMGR_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIREGISTERMGR_H

#include <cstddef>
#include <map>
#include <windows.h>

class CUIRegisterMgr {
public:
  bool sub_A970A0(int key);

  /* 0000 */ void* vtable;
  /* 0004 */ std::map<int, int> entries;
  /* 000C */ std::map<int, int> unknown_000C;
  /* 0014 */ std::map<int, int> unknown_0014;
  /* 001C */ CRITICAL_SECTION lock;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUIRegisterMgr) == 0x34);
static_assert(offsetof(CUIRegisterMgr, entries) == 0x4);
static_assert(offsetof(CUIRegisterMgr, lock) == 0x1C);
#endif

#endif
