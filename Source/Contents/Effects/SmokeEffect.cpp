#include "Contents/Effects/SmokeEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B763E0

// OFFSET: 0x00B76540

// OFFSET: 0x00B76580

// OFFSET: 0x00B76710
// FUNCTION: fn_19178
bool CSmokeEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B76720

// OFFSET: 0x00B76D80
// FUNCTION: fn_19180
void CSmokeEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it && (*it)->unknown_000C < this->visibleCount) {
      (*it)->sub_B40F30(1);
    }
  }
}
