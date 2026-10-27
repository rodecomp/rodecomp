#ifndef RAGNAROK_SOURCE_UTILITY_THREADING_NSYNCOBJECT_H
#define RAGNAROK_SOURCE_UTILITY_THREADING_NSYNCOBJECT_H

#include <windows.h>

class NSyncObject {
public:
  explicit NSyncObject(CRITICAL_SECTION* criticalSection);
  virtual ~NSyncObject();

private:
  /* 0004 */ CRITICAL_SECTION* m_criticalSection;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NSyncObject) == 8);
#endif

#endif
