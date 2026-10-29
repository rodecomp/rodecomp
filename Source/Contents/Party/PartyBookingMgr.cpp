#include "Contents/Party/PartyBookingMgr.h"

#include "Client/Session.h"
#include "Contents/MessageTable.h"

#include <cstdio>
#include <cstring>
#include <ctime>

// OFFSET: 0x00B15C80

// OFFSET: 0x00B16810

// OFFSET: 0x00B16A80
// FUNCTION: fn_18443
void PartyBookingMgr::Clear() {
  this->m_entries.clear();
  this->m_pending.clear();
}

// OFFSET: 0x00B16AD0

// OFFSET: 0x00B16CD0

// OFFSET: 0x00B16CF0

// OFFSET: 0x00B17820

// OFFSET: 0x00B17C40
// FUNCTION: fn_18450
void PartyBookingMgr::Reset() {
  this->m_lastPage = true;
  this->m_lastIndex = -1;
  this->unknown_0000 = 0;
  this->m_entries.clear();
  this->m_pending.clear();
}

// OFFSET: 0x00B18210

// OFFSET: 0x00B18230
