#include "Contents/Effects/FriggSongEffect.h"

#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00701BB0
// FUNCTION: fn_06999
CFriggSongEffect::CFriggSongEffect() : CEffect(0x423), lastInner{} {
  this->unknown_002C = EffectLifetime(0x423);
}

// OFFSET: 0x007021A0
// FUNCTION: fn_07003
void CFriggSongEffect::Slot12() {
  this->AddEvent(0, "effect\\wm_frigg_song.wav", 0, 0);
}

// OFFSET: 0x007023E0
// FUNCTION: fn_07005
void CFriggSongEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    if (CParticle* particle = *it) {
      const int age = particle->unknown_00E8;
      const float baseAngle = *reinterpret_cast<const float*>(&particle->unknown_00C4);
      const int angle = static_cast<int>(baseAngle - static_cast<float>(age));
      float& height = *reinterpret_cast<float*>(&particle->unknown_0048[0xC]);
      if (age < 20) {
        height = 1.0f;
      } else if (age <= 140) {
        height = (std::max)(0.0f, g_sinTable[age - 20] * particle->unknown_00C0);
      } else {
        const float speed = particle->unknown_00C0;
        particle->unknown_00C0 = (std::max)(0.0f, speed - 0.5f - speed * 0.01f);
        height = particle->unknown_00C0;
      }

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeYRotation(static_cast<float>(angle) / 180.0f * 3.141592f);
      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(1.0f, height / 100.0f * 1.1f, 1.0f);
      matrix first = rotation;
      rotation.Compose(first, scale);
      particle->transform = rotation;
    }
    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x007027A0
// FUNCTION: fn_07006
void CFriggSongEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    const int texPart = particle->unknown_000C;
    const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(texPart - 1) / 21.0f, static_cast<float>(texPart) / 21.0f, false, true};
    particle->Render(quad);
  }
}
