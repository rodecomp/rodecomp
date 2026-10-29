#include "Contents/Effects/ThrowEffects.h"
#include "Contents/Effects/Particle.h"

// OFFSET: 0x00B7D8B0
// FUNCTION: fn_19234
void CThrowItemEffect::Slot12() {
  switch (this->id) {
  case 613:
  case 614:
  case 615:
  case 616:
    this->AddEvent(0, "effect\\닌자_던지기.wav", 0, 0);
    break;
  case 298:
    this->AddEvent(0, "_heal_effect.wav", 0, 0);
    break;
  case 914:
    this->AddEvent(0, "effect\\G스포어익스플로젼.wav", 0, 0);
    break;
  }
}

// OFFSET: 0x00B7F190
// FUNCTION: fn_19243
bool CVolcanoEffect::Slot06() {
  for (auto particle = this->particles.begin(); particle != this->particles.end(); ++particle) {
    if ((*particle)->alpha > 0) {
      return true;
    }
  }

  return false;
}
