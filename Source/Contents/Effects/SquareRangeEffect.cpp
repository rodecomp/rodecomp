#include "Contents/Effects/SquareRangeEffect.h"

#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B4C5E0
// FUNCTION: fn_18960
CSquareRangeEffect::CSquareRangeEffect() : CEffect(-1), size(1.0f), resize(0), particles() {
}

// OFFSET: 0x00B4C8E0
// FUNCTION: fn_18965
void CSquareRangeEffect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      if (this->resize) {
        particle->scale = this->size * 2.5f;
        this->resize = 0;
      }

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeXRotation(1.5707964f);
      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(particle->scale, particle->scale, 1.0f);
      matrix transform;
      transform.Compose(scale, rotation);
      particle->transform = transform;
    }

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B4CB40
// FUNCTION: fn_18966
void CSquareRangeEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}
