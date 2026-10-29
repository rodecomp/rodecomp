#include "Contents/Actors/GroundScope.h"

#include <windows.h>
#include <mmsystem.h>

// OFFSET: 0x00E4C2F0
// FUNCTION: fn_24284
bool CGroundScope::OnProcess() {
  if (this->m_lastingTime < timeGetTime()) {
    return false;
  }

  return true;
}
