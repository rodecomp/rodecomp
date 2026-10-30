#include "Contents/Effects/SphereWind2Effect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00B77340

// OFFSET: 0x00B77380

// OFFSET: 0x00B775E0
// FUNCTION: fn_19184
bool CSphereWind2Effect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B775F0
// FUNCTION: fn_19185
void CSphereWind2Effect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    if (CParticle* particle = *it) {
      if (particle->unknown_00E8 > 0) {
        int alpha = particle->alpha;
        if (particle->unknown_00E8 <= 25) {
          alpha += 2;
        } else if (particle->unknown_00E8 > 70) {
          alpha -= 2;
        }

        const float scale = particle->scale * 1.01f;
        particle->alpha = alpha < 0 ? 0 : alpha;
        particle->scale = scale;

        const int oldAngle = particle->unknown_004C;
        particle->unknown_004C = oldAngle + (oldAngle + 1 < 360 ? 1 : -359);
        const int angle = particle->unknown_004C;

        const int texPart = particle->unknown_000C;
        float& spin = *reinterpret_cast<float*>(&particle->unknown_00C4);
        float rotation = *reinterpret_cast<float*>(particle->unknown_0058) + spin;
        if (rotation >= 360.0f) {
          rotation -= 360.0f;
        }
        spin = rotation;

        const int step = static_cast<int>(static_cast<float>(*reinterpret_cast<int*>(particle->unknown_0050)) / 20.0f);
        const int pitch = static_cast<int>(static_cast<float>(step * texPart) + rotation) % 360;

        const float radius = particle->unknown_00C0;
        const vector3d center = {scale * g_cosTable[angle] * g_cosTable[pitch], scale * g_sinTable[angle] * g_cosTable[pitch], scale * g_sinTable[pitch]};
        const vector3d side = {-g_sinTable[angle] * radius, radius * g_cosTable[angle], 0.0f};
        const vector3d offset = {0.0f, particle->unknown_0054, 0.0f};
        const vector3d inner = {center.x - side.x + offset.x, center.y - side.y + offset.y, center.z - side.z + offset.z};
        const vector3d outer = {center.x + side.x + offset.x, center.y + side.y + offset.y, center.z + side.z + offset.z};

        particle->vertices[1].x = inner.x;
        particle->vertices[1].y = inner.y;
        particle->vertices[1].z = inner.z;
        particle->vertices[3].x = outer.x;
        particle->vertices[3].y = outer.y;
        particle->vertices[3].z = outer.z;

        if (texPart != 0) {
          particle->vertices[0].x = this->lastInner.x;
          particle->vertices[0].y = this->lastInner.y;
          particle->vertices[0].z = this->lastInner.z;
          particle->vertices[2].x = this->lastOuter.x;
          particle->vertices[2].y = this->lastOuter.y;
          particle->vertices[2].z = this->lastOuter.z;
        }

        this->lastInner = inner;
        this->lastOuter = outer;
      }
    }
    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x00B77B40
// FUNCTION: fn_19186
void CSphereWind2Effect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->unknown_00E8 > 0 && particle->unknown_000C > 0) {
      const int texPart = particle->unknown_000C;
      const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(texPart - 1) / 21.0f, static_cast<float>(texPart) / 21.0f, false, true};
      particle->Render(quad);
    }
  }
}
