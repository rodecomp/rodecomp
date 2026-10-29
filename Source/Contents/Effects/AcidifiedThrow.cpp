#include "Contents/Effects/AcidifiedThrow.h"

// OFFSET: 0x006F3540
// FUNCTION: fn_06821
CAcidifiedThrow::CAcidifiedThrow() : CEffect(-1) {
  this->unknown_00E4.x = 0.0f;
  this->unknown_00E4.y = 0.0f;
  this->unknown_00E4.z = 0.0f;
}

// OFFSET: 0x006F3F90
// FUNCTION: fn_06825
bool CAcidifiedThrow::Slot06() {
  return this->active;
}
