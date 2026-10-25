#include "Utility/Threading/CSyncObject.h"

CSyncObject::CSyncObject(CRITICAL_SECTION* criticalSection) : m_criticalSection(criticalSection) {
  EnterCriticalSection(this->m_criticalSection);
}

// OFFSET: 0x00AEE560
// FUNCTION: fn_18172
CSyncObject::~CSyncObject() {
  LeaveCriticalSection(this->m_criticalSection);
}
