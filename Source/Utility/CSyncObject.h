#ifndef RAGNAROK_SOURCE_UTILITY_CSYNC_OBJECT_H
#define RAGNAROK_SOURCE_UTILITY_CSYNC_OBJECT_H

#include <windows.h>

// RTTI CSyncObject. Owns a scoped critical-section entry with virtual destruction.
class CSyncObject {
public:
  explicit CSyncObject(CRITICAL_SECTION* criticalSection);
  virtual ~CSyncObject();

private:
  CRITICAL_SECTION* m_criticalSection;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSyncObject) == 8);
#endif

#endif
