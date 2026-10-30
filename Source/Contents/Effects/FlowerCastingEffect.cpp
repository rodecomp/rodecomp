#include "Contents/Effects/FlowerCastingEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>

// OFFSET: 0x00B66870
// FUNCTION: fn_19059
void CFlowerCastingEffect::AddRing(int kind) {
  for (int ring = 0; ring < 4; ++ring) {
    for (int segment = 0; segment < 21; ++segment) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->unknown_0004 = this->id;
      particle->unknown_000C = segment;
      particle->unknown_0010 = ring;
      particle->unknown_0008 = 1;
      particle->SetTexture("effect\\ring_blue.tga");
      particle->unknown_0050 = 315;
      particle->alpha = 0;

      float& baseAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
      if (kind == 0) {
        if (ring == 0) {
          particle->scale = 4.5f;
          particle->unknown_004C = 85;
          particle->unknown_00C0 = 27.0f;
          baseAngle = 0.0f;
        } else if (ring == 1) {
          particle->scale = 5.0f;
          particle->unknown_004C = 80;
          particle->unknown_00C0 = 28.0f;
          baseAngle = 90.0f;
        } else if (ring == 2) {
          particle->scale = 5.5f;
          particle->unknown_004C = 70;
          particle->unknown_00C0 = 29.0f;
          baseAngle = 180.0f;
        } else if (ring == 3) {
          particle->scale = 5.0f;
          particle->unknown_004C = 75;
          particle->unknown_00C0 = 30.0f;
          baseAngle = 270.0f;
        }
      } else if (kind == 1) {
        if (ring == 0) {
          particle->scale = 5.5f;
          particle->unknown_004C = 50;
          particle->unknown_00C0 = 30.0f;
          baseAngle = 90.0f;
        } else if (ring == 1) {
          particle->scale = 6.0f;
          particle->unknown_004C = 55;
          particle->unknown_00C0 = 29.0f;
          baseAngle = 180.0f;
        } else if (ring == 2) {
          particle->scale = 6.5f;
          particle->unknown_004C = 65;
          particle->unknown_00C0 = 28.0f;
          baseAngle = 270.0f;
        } else if (ring == 3) {
          particle->scale = 6.0f;
          particle->unknown_004C = 60;
          particle->unknown_00C0 = 27.0f;
          baseAngle = 0.0f;
        }
      }

      particle->SetColor(100, 100, 255);
      particle->unknown_0054 = 0.0f;
      *reinterpret_cast<float*>(particle->unknown_0058) = particle->unknown_00C0;
      InitParticleQuad(particle->vertices);
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B66C60
// FUNCTION: fn_19060
void CFlowerCastingEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B66CA0
// FUNCTION: fn_19061
void CFlowerCastingEffect::Slot01() {
  this->unknown_002C = 400;

  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  const vector3d& player = reinterpret_cast<const CRenderObject*>(world->player)->render.position;
  g_soundMgr->PlayWave("effect\\T_안락한마법.wav", this->position[0] - player.x, 0.0f, this->position[2] - player.z, 250, 40, 1.0f, 0);

  this->AddRing(0);
  this->AddRing(1);
}

// OFFSET: 0x00B66D20
// FUNCTION: fn_19062
bool CFlowerCastingEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B66D30
// FUNCTION: fn_19063
void CFlowerCastingEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];

    CParticle* particle = *it;
    if (particle && particle->unknown_00E8 > 0) {
      float radius = g_sinTable[particle->unknown_00E8 % 90 * 4] * 2.5f + *reinterpret_cast<const float*>(particle->unknown_0058);
      particle->unknown_00C0 = radius;

      const int age = particle->unknown_00E8;
      int alpha = particle->alpha;
      if (age > 110) {
        alpha = (std::max)(alpha - 1, 0);
      } else if (age < 20) {
        alpha = (std::min)(alpha + 5, 180);
      }
      particle->alpha = alpha;

      const int segment = particle->unknown_000C;
      radius *= g_sinTable[segment * 9];
      if (age <= 90) {
        radius *= g_sinTable[age];
      }
      radius = (std::max)(radius, 0.0f);

      const float baseAngle = *reinterpret_cast<const float*>(&particle->unknown_00C4);
      const int angle = static_cast<int>(static_cast<int>(particle->unknown_0050 / 20.0f) * segment + baseAngle) % 360;
      const int tilt = particle->unknown_004C;
      const float spread = radius * g_cosTable[tilt];
      const float height = radius * g_sinTable[tilt];
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];

      vector3d points[2] = {};
      points[0].x = particle->scale * cosine;
      points[0].y = 0.0f;
      points[0].z = particle->scale * sine;
      points[1].x = points[0].x + spread * cosine;
      points[1].y = 0.0f - height;
      points[1].z = points[0].z + spread * sine;

      particle->vertices[3].x = points[0].x;
      particle->vertices[3].y = points[0].y;
      particle->vertices[3].z = points[0].z;
      particle->vertices[1].x = points[1].x;
      particle->vertices[1].y = points[1].y;
      particle->vertices[1].z = points[1].z;

      if (segment != 0) {
        particle->vertices[0].x = this->lastOuter.x;
        particle->vertices[0].y = this->lastOuter.y;
        particle->vertices[0].z = this->lastOuter.z;
        particle->vertices[2].x = this->lastInner.x;
        particle->vertices[2].y = this->lastInner.y;
        particle->vertices[2].z = this->lastInner.z;
      }

      this->lastInner = points[0];
      this->lastOuter = points[1];
    }

    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x00B671E0
// FUNCTION: fn_19064
void CFlowerCastingEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || particle->unknown_000C <= 0) {
      continue;
    }

    const int segment = particle->unknown_000C;
    const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(segment - 1) / 21.0f, static_cast<float>(segment) / 21.0f, false, true};
    particle->Render(quad);
  }
}
