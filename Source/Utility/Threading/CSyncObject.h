#ifndef RAGNAROK_SOURCE_UTILITY_THREADING_CSYNCOBJECT_H
#define RAGNAROK_SOURCE_UTILITY_THREADING_CSYNCOBJECT_H

#include <windows.h>

class CSyncObject {
public:
  explicit CSyncObject(CRITICAL_SECTION* criticalSection);
  virtual ~CSyncObject();

private:
  /* 0004 */ CRITICAL_SECTION* m_criticalSection;
};

// INLINED
inline CSyncObject::CSyncObject(CRITICAL_SECTION* criticalSection) : m_criticalSection(criticalSection) {
  EnterCriticalSection(this->m_criticalSection);
}

// INLINED
inline CSyncObject::~CSyncObject() {
  LeaveCriticalSection(this->m_criticalSection);
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSyncObject) == 8);
#endif

#endif
