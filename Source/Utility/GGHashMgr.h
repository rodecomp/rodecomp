#ifndef RAGNAROK_SOURCE_UTILITY_GGHASHMGR_H
#define RAGNAROK_SOURCE_UTILITY_GGHASHMGR_H

#include "Utility/GGHash.h"

#include <windows.h>

// VTABLE: vt_00504
class CGGHashMgr {
public:
  virtual ~CGGHashMgr();

  static void Destroy();

  /* 0004 */ bool m_initialized;
  /* 0008 */ CGGHash* m_hash;
  /* 000C */ CRITICAL_SECTION m_lock;
};

extern CGGHashMgr* g_ggHashMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGGHashMgr) == 0x24);
#endif

#endif
