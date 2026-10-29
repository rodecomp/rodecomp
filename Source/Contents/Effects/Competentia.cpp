#include "Contents/Effects/Competentia.h"

// OFFSET: 0x00708930
// FUNCTION: fn_07077
CCompetentia::CCompetentia() : CEffect(1851) {
}

// OFFSET: 0x00708CB0
// FUNCTION: fn_07081
bool CCompetentia::Slot06() {
  return !this->particles.empty();
}
