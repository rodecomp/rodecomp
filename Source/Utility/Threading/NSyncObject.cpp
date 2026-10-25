#include "Utility/Threading/NSyncObject.h"

// OFFSET: 0x00737130
// FUNCTION: fn_07640
NSyncObject::NSyncObject(CRITICAL_SECTION* criticalSection) : m_criticalSection(criticalSection) {
  EnterCriticalSection(this->m_criticalSection);
}

// OFFSET: 0x00737150
// FUNCTION: fn_07641
// OFFSET: 0x007371A0
// FUNCTION: fn_07642
NSyncObject::~NSyncObject() {
  LeaveCriticalSection(this->m_criticalSection);
}
