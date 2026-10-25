#ifndef RAGNAROK_SOURCE_UTILITY_THREADING_CSYNCOBJECT_H
#define RAGNAROK_SOURCE_UTILITY_THREADING_CSYNCOBJECT_H

#include <windows.h>

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
