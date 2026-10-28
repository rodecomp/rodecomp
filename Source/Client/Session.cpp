#include "Client/Session.h"

// STATIC: 0x01732638
// GLOBAL: gl_24474
CSession g_session;

// OFFSET: 0x00DDE860

// OFFSET: 0x00E13B00
// FUNCTION: fn_23826
int CSession::SyncUnknown0E98() {
  if (this->unknown_0E98 != this->unknown_5558) {
    this->unknown_0E98 = this->unknown_5558;
  }

  return this->unknown_0E98;
}
