#include "Contents/Effects/FortuneKissEffect.h"

#include "Contents/Effects/Particle.h"

// OFFSET: 0x00701700
// FUNCTION: fn_06996
bool CFortuneKiss::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00701B10
// FUNCTION: fn_06998
void CFortuneKiss::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
