#include "Contents/Effects/FriggSongEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
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

// OFFSET: 0x00701EE0
// FUNCTION: fn_07001
void CFriggSongEffect::Close() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00701F20
// FUNCTION: fn_07002
void CFriggSongEffect::Slot01() {
  for (int segment = 0; segment <= 21; ++segment) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (!particle) {
      continue;
    }

    particle->SetTexture("effect\\ring_blue.tga");
    particle->unknown_0004 = this->id;
    particle->unknown_000C = segment;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->alpha = 50;
    particle->SetColor(50, 200, 50);
    particle->unknown_0050 = 360;
    particle->unknown_00C0 = 120.0f;
    particle->unknown_004C = 90;
    float& baseAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
    baseAngle = static_cast<float>(particle->unknown_0010 * 10);
    particle->scale = static_cast<float>(particle->unknown_0010) * 0.5f + 2.5f;
    (&particle->unknown_0054)[particle->unknown_0010] = 0.0f;
    particle->unknown_00E8 = particle->unknown_0010;

    int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->unknown_0050) / 20.0f) * particle->unknown_000C) + baseAngle) % 360;
    if (particle->unknown_000C == 20) {
      angle = static_cast<int>(baseAngle);
    }

    const int tilt = particle->unknown_004C;
    const float radius = particle->unknown_00C0;
    vector3d points[2] = {};
    points[0].x = particle->scale * g_cosTable[angle];
    points[0].y = 0.0f;
    points[0].z = particle->scale * g_sinTable[angle];
    points[1].x = points[0].x + radius * g_cosTable[tilt] * g_cosTable[angle];
    points[1].y = 0.0f - radius * g_sinTable[tilt];
    points[1].z = points[0].z + radius * g_cosTable[tilt] * g_sinTable[angle];

    if (particle->unknown_000C >= 1 && particle->unknown_000C <= 20) {
      particle->vertices[1].x = points[1].x;
      particle->vertices[1].y = points[1].y;
      particle->vertices[1].z = points[1].z;
      particle->vertices[3].x = points[0].x;
      particle->vertices[3].y = points[0].y;
      particle->vertices[3].z = points[0].z;
      particle->vertices[0].x = this->lastInner[1].x;
      particle->vertices[0].y = this->lastInner[1].y;
      particle->vertices[0].z = this->lastInner[1].z;
      particle->vertices[2].x = this->lastInner[0].x;
      particle->vertices[2].y = this->lastInner[0].y;
      particle->vertices[2].z = this->lastInner[0].z;
    }

    this->lastInner[0] = points[0];
    this->lastInner[1] = points[1];
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x007021A0
// FUNCTION: fn_07003
void CFriggSongEffect::Slot12() {
  this->AddEvent(0, "effect\\wm_frigg_song.wav", 0, 0);
}

// OFFSET: 0x007023D0
// FUNCTION: fn_07004
bool CFriggSongEffect::Slot06() {
  return !this->IsFinished();
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
