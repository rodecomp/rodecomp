#include "Contents/Effects/BlastForge.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x006F7360
// FUNCTION: fn_06858
CBlastForge::CBlastForge() : CEffect(-1), unknown_00DC(0), startTime(timeGetTime()) {
  this->unknown_00CC = nullptr;
  this->unknown_00D0 = nullptr;
  this->unknown_00D4 = nullptr;
  this->unknown_00D8 = nullptr;
}

// OFFSET: 0x006F7A20
// FUNCTION: fn_06863
bool CBlastForge::Slot06() {
  return this->ActorExists();
}
