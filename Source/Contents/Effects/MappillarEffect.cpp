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
// FUNCTION: fn_18956
void CMappillarEffect::Slot01() {
  for (int ring = 0; ring < 4; ++ring) {
    for (int segment = 0; segment <= 21; ++segment) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->SetTexture("effect\\ring_blue.tga");
      particle->unknown_0004 = this->id;
      particle->unknown_000C = segment;
      particle->unknown_0010 = ring;
      particle->unknown_0008 = 1;
      particle->alpha = 70;
      particle->SetColor(100, 100, 255);

      float& baseAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
      particle->unknown_0050 = 360;
      particle->unknown_00C0 = 120.0f;
      particle->unknown_004C = 89;
      baseAngle = static_cast<float>(particle->unknown_0010 * 90);
      particle->scale = static_cast<float>(particle->unknown_0010) * 0.5f + 11.0f;
      reinterpret_cast<int*>(&particle->unknown_0054)[particle->unknown_0010] = 0;
      particle->unknown_00E8 = particle->unknown_0010 * 30;

      int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->unknown_0050) / 20.0f) * particle->unknown_000C) + baseAngle) % 360;
      if (particle->unknown_000C == 20) {
        angle = static_cast<int>(baseAngle);
      }

      const int tilt = particle->unknown_004C;
      const float spread = particle->unknown_00C0 * g_cosTable[tilt];
      const float height = particle->unknown_00C0 * g_sinTable[tilt];
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];

      vector3d points[2] = {};
      points[0].x = particle->scale * cosine;
      points[0].y = 0.0f;
      points[0].z = particle->scale * sine;
      points[1].x = points[0].x + spread * cosine;
      points[1].y = 0.0f - height;
      points[1].z = points[0].z + spread * sine;

      if (particle->unknown_000C >= 1 && particle->unknown_000C <= 20) {
        particle->vertices[1].x = points[1].x;
        particle->vertices[1].y = points[1].y;
        particle->vertices[1].z = points[1].z;
        particle->vertices[3].x = points[0].x;
        particle->vertices[3].y = points[0].y;
        particle->vertices[3].z = points[0].z;
        particle->vertices[0].x = this->lastOuter.x;
        particle->vertices[0].y = this->lastOuter.y;
        particle->vertices[0].z = this->lastOuter.z;
        particle->vertices[2].x = this->lastInner.x;
        particle->vertices[2].y = this->lastInner.y;
        particle->vertices[2].z = this->lastInner.z;
      }

      this->lastInner = points[0];
      this->lastOuter = points[1];
      this->particles.push_back(particle);
    }
  }
}

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
