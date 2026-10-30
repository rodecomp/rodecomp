#ifndef RAGNAROK_SOURCE_SECURITY_THEMIDAMGR_H
#define RAGNAROK_SOURCE_SECURITY_THEMIDAMGR_H

#include <cstddef>

// VTABLE: vt_00507
class CThemidaMgr {
public:
  CThemidaMgr();
  virtual ~CThemidaMgr();

  static bool Create();
  static void Destroy();

  /* 0004 */ bool m_valid;
};

extern CThemidaMgr* g_themidaMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CThemidaMgr) == 8);
static_assert(offsetof(CThemidaMgr, m_valid) == 4);
#endif

#endif
