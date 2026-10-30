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
// FUNCTION: fn_19182
void CSphereWind2Effect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B77380
// FUNCTION: fn_19183
void CSphereWind2Effect::Slot01() {
  for (int base = 0; base < 45; base += 9) {
    int angle = base;
    for (int ring = 0; ring < 4; ++ring) {
      const int delay = -std::max(rand() % 20, 0);
      const float radius = static_cast<float>(std::max(rand() % 11, 0)) * 0.2f + 2.0f;
      const int rotation = std::max(rand() % 360, 0);
      const int step = std::max(rand() % 91, 0) + 180;
      const int speed = std::max(rand() % 4, 0) + 3;

      for (int part = 0; part < 21; ++part) {
        CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
        if (!particle) {
          continue;
        }

        particle->unknown_0004 = this->id;
        particle->unknown_000C = part;
        particle->unknown_0010 = ring;
        particle->unknown_0008 = 1;
        particle->unknown_00E8 = delay;
        particle->unknown_00C0 = radius;
        particle->scale = 5.0f;
        *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(rotation);
        particle->unknown_004C = angle;
        particle->alpha = 0;
        particle->unknown_0050 = step;
        particle->unknown_0054 = -10.0f;
        *reinterpret_cast<float*>(particle->unknown_0058) = static_cast<float>(speed);
        particle->SetTexture("effect\\fire_ivy.bmp");
        particle->SetColor(255, 89, 182);
        InitParticleQuad(particle->vertices);
        this->particles.push_back(particle);
      }

      angle += 45;
    }
  }

  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  const vector3d& player = reinterpret_cast<const CRenderObject*>(world->player)->render.position;
  g_soundMgr->PlayWave("effect\\EF_Blessing.wav", this->position[0] - player.x, 0.0f, this->position[2] - player.z, 250, 40, 1.0f, 0);
  this->unknown_002C = 200;
}

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
