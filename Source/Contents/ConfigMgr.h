#ifndef RAGNAROK_SOURCE_CONTENTS_CONFIGMGR_H
#define RAGNAROK_SOURCE_CONTENTS_CONFIGMGR_H

#include <windows.h>

// VTABLE: vt_00751
class CConfigMgr {
public:
  // INLINED
  CConfigMgr() : m_initialized(false), m_hasPerformanceCounter(true) {
    if (!QueryPerformanceFrequency(&this->m_frequency)) {
      this->m_hasPerformanceCounter = false;
      this->m_frequency.QuadPart = 1000000;
    }

    this->m_ticksPerFrame = this->m_frequency.QuadPart / 60;
    this->m_initialized = true;
  }
  virtual ~CConfigMgr();

  static bool Create();
  static void Update();
  static void Destroy();

  /* 0008 */ bool m_initialized;
  /* 0009 */ bool m_hasPerformanceCounter;
  /* 0010 */ LARGE_INTEGER m_frequency;
  /* 0018 */ LONGLONG m_ticksPerFrame;
};

extern CConfigMgr* g_configMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CConfigMgr) == 0x20);
#endif

#endif
