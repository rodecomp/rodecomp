#include "Contents/Effects/SphereWind2Effect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/EffectPoint.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00B77340
// FUNCTION: fn_19182
void CSphereWind2Effect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B77380
// FUNCTION: fn_19183
void CSphereWind2Effect::Open() {
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

        particle->effectId = this->id;
        particle->columnIndex = part;
        particle->rowIndex = ring;
        particle->groupIndex = 1;
        particle->elapsedFrames = delay;
        particle->speedParameter = radius;
        particle->scale = 5.0f;
        *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(rotation);
        particle->phaseParameter = angle;
        particle->alpha = 0;
        particle->angleParameter = step;
        particle->animationParameter = -10.0f;
        *reinterpret_cast<float*>(particle->animationParameters) = static_cast<float>(speed);
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
  this->effectLifetime = 200;
}

// OFFSET: 0x00B775E0
// FUNCTION: fn_19184
bool CSphereWind2Effect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B775F0
// FUNCTION: fn_19185
void CSphereWind2Effect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      if (particle->elapsedFrames > 0) {
        int alpha = particle->alpha;
        if (particle->elapsedFrames <= 25) {
          alpha += 2;
        } else if (particle->elapsedFrames > 70) {
          alpha -= 2;
        }

        const float scale = particle->scale * 1.01f;
        particle->alpha = alpha >= 0 ? alpha : 0;
        particle->scale = scale;

        const int oldAngle = particle->phaseParameter;
        particle->phaseParameter = oldAngle + (oldAngle + 1 < 360 ? 1 : -359);
        const int angle = particle->phaseParameter;

        const int texPart = particle->columnIndex;
        float& spin = *reinterpret_cast<float*>(&particle->angleBits);
        float rotation = *reinterpret_cast<float*>(particle->animationParameters) + spin;
        if (rotation >= 360.0f) {
          rotation -= 360.0f;
        }
        spin = rotation;

        const int step = static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f);
        const int pitch = static_cast<int>(static_cast<float>(step * texPart) + rotation) % 360;

        const float radius = particle->speedParameter;
        EffectPoint points[4];
        points[0].x = scale * g_cosTable[angle] * g_cosTable[pitch];
        points[0].y = scale * g_sinTable[angle] * g_cosTable[pitch];
        points[0].z = scale * g_sinTable[pitch];
        points[1].x = -g_sinTable[angle] * radius;
        points[1].y = radius * g_cosTable[angle];
        points[1].z = 0.0f;
        const vector3d offset = {0.0f, particle->animationParameter, 0.0f};
        points[2].x = points[0].x - points[1].x + offset.x;
        points[2].y = points[0].y - points[1].y + offset.y;
        points[2].z = points[0].z - points[1].z + offset.z;
        points[3].x = points[0].x + points[1].x + offset.x;
        points[3].y = points[0].y + points[1].y + offset.y;
        points[3].z = points[0].z + points[1].z + offset.z;

        particle->vertices[1].x = points[2].x;
        particle->vertices[1].y = points[2].y;
        particle->vertices[1].z = points[2].z;
        particle->vertices[3].x = points[3].x;
        particle->vertices[3].y = points[3].y;
        particle->vertices[3].z = points[3].z;

        if (texPart != 0) {
          particle->vertices[0].x = this->lastInner.x;
          particle->vertices[0].y = this->lastInner.y;
          particle->vertices[0].z = this->lastInner.z;
          particle->vertices[2].x = this->lastOuter.x;
          particle->vertices[2].y = this->lastOuter.y;
          particle->vertices[2].z = this->lastOuter.z;
        }

        this->lastInner.x = points[2].x;
        this->lastInner.y = points[2].y;
        this->lastInner.z = points[2].z;
        this->lastOuter.x = points[3].x;
        this->lastOuter.y = points[3].y;
        this->lastOuter.z = points[3].z;
      }
    }
    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B77B40
// FUNCTION: fn_19186
void CSphereWind2Effect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->elapsedFrames > 0 && particle->columnIndex > 0) {
      const int texPart = particle->columnIndex;
      ParticleQuad quad;
      quad.billboard = false;
      quad.unused = false;
      quad.depth = true;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(texPart - 1) / 21.0f;
      quad.right = static_cast<float>(texPart) / 21.0f;
      particle->Render(quad);
    }
  }
}
