#include "Contents/Effects/AxeBoomerang.h"

// OFFSET: 0x006F5820
// FUNCTION: fn_06837
CAxeBoomerang::CAxeBoomerang() : CEffect(-1) {
  this->unknown_00DC.x = 0.0f;
  this->unknown_00DC.y = 0.0f;
  this->unknown_00DC.z = 0.0f;
  this->unknown_00E8.x = 0.0f;
  this->unknown_0050 = 0;
  this->unknown_00E8.x = 0.0f;
  this->unknown_00E8.y = 0.0f;
  this->unknown_00E8.z = 0.0f;
}

// OFFSET: 0x006F5CD0
// FUNCTION: fn_06841
bool CAxeBoomerang::Slot06() {
  return this->unknown_00D0 != 0;
}
