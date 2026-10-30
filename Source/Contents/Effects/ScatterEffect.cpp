#include "Contents/Effects/ScatterEffect.h"

#include "Contents/Effects/SpriteParticle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// OFFSET: 0x00B75D10
// FUNCTION: fn_19167
CScatterEffect::CScatterEffect() : CEffect(-1) {
}

// OFFSET: 0x00B76130
// FUNCTION: fn_19172
void CScatterEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    if (CSpriteParticle* particle = *it) {
      particle->alpha -= 3;
      if (particle->alpha < 0) {
        particle->alpha = 0;
      }

      const int angle = particle->unknown_004C;
      particle->velocity.y += particle->frameDelay;
      particle->velocity.x += g_cosTable[angle] * 0.2f;
      particle->velocity.z += g_sinTable[angle] * 0.2f;

      matrix translate;
      std::memcpy(translate.values, g_identityMatrix, sizeof(translate.values));
      translate.MakeTranslate(particle->velocity.x + 0.0f, particle->velocity.y - 40.0f, particle->velocity.z + 0.0f);
      particle->transform = translate;
    }
    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x00B76390
// FUNCTION: fn_19173
void CScatterEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->sub_B40F30(1);
    }
  }
}
