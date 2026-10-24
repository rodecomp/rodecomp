#ifndef RAGNAROK_SOURCE_UTILITY_NSYNC_OBJECT_H
#define RAGNAROK_SOURCE_UTILITY_NSYNC_OBJECT_H

#include <windows.h>

// Original class name established by RTTI; owns one critical-section entry.
class NSyncObject {
public:
  explicit NSyncObject(CRITICAL_SECTION* criticalSection);
  virtual ~NSyncObject();

private:
  CRITICAL_SECTION* m_criticalSection;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NSyncObject) == 8);
#endif

#endif
