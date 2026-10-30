#include "Contents/Effects/MappillarEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00B4BBB0
// FUNCTION: fn_18955
void CMappillarEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B4BBF0

// OFFSET: 0x00B4C0B0
// FUNCTION: fn_18957
bool CMappillarEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B4C0C0
// FUNCTION: fn_18958
void CMappillarEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    if (CParticle* particle = *it) {
      if (particle->unknown_0010 != 3 || particle->unknown_000C != 21 || particle->unknown_00E8 <= 999) {
        const int frame = particle->unknown_00E8;
        const int angle = static_cast<int>(*reinterpret_cast<float*>(&particle->unknown_00C4) - static_cast<float>(frame));
        float height;
        if (frame >= 800) {
          particle->unknown_00C0 = std::max(0.0f, particle->unknown_00C0 - 1.0f);
          height = particle->unknown_00C0;
        } else if (frame < 200) {
          height = 0.0f;
        } else if (frame > 290) {
          height = particle->unknown_00C0;
        } else {
          height = g_sinTable[frame - 200] * particle->unknown_00C0;
        }

        particle->unknown_0054 = height;
        if (height < 0.0f) {
          particle->unknown_0054 = 0.0f;
        }

        matrix rotation;
        std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
        rotation.MakeYRotation(static_cast<float>(angle) / 180.0f * 3.141592f);
        matrix scale;
        std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
        scale.SetScale(1.0f, particle->unknown_0054 / 100.0f * 1.1f, 1.0f);
        const matrix first = rotation;
        rotation.Compose(first, scale);
        particle->transform = rotation;
      }
    }
    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x00B4C4C0
// FUNCTION: fn_18959
void CMappillarEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      const int texPart = particle->unknown_000C;
      ParticleQuad quad;
      quad.billboard = false;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(texPart - 1) / 21.0f;
      quad.right = static_cast<float>(texPart) / 21.0f;
      quad.unused = false;
      quad.depth = true;
      particle->Render(quad);
    }
  }
}
