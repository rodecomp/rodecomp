#include "Contents/Effects/StealEffect.h"

#include "Contents/Effects/SpriteParticle.h"

// OFFSET: 0x00B4D720
// FUNCTION: fn_18975
void CSteal::Slot04() {
  for (std::list<CSpriteParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->sub_B40F30(1);
  }
}
