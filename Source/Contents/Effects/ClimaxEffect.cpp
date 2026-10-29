#include "Contents/Effects/ClimaxEffect.h"

// OFFSET: 0x00706840
CClimaxEffect::CClimaxEffect() : CEffect(-1) {
  this->particles[0] = nullptr;
  this->particles[1] = nullptr;
}
