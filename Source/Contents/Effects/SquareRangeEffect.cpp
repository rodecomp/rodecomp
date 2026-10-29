#include "Contents/Effects/SquareRangeEffect.h"

#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B4C5E0

// OFFSET: 0x00B4C8E0

// OFFSET: 0x00B4CB40
// FUNCTION: fn_18966
void CSquareRangeEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
