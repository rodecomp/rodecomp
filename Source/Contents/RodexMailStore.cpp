#include "Contents/RodexMailStore.h"

#include <new>

// GLOBAL: gl_24911
RodexMailStore* g_rodexMailStore;

// OFFSET: 0x007FE5E0

// OFFSET: 0x007FE7B0

// OFFSET: 0x007FE7D0
// FUNCTION: fn_09987
void RodexMailStore::Clear() {
  this->m_mails0.clear();
  this->m_mails1.clear();
  this->m_mails2.clear();
  this->unknown_0008 = 0;
  this->unknown_000C = 0;
  this->unknown_0010 = false;
  this->unknown_0018 = 0;
}

// OFFSET: 0x007FE850
