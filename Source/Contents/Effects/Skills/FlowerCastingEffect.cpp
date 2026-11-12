#include "Contents/Effects/Skills/FlowerCastingEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Particles/Particle.h"
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

      particle->effectId = this->id;
      particle->columnIndex = segment;
      particle->rowIndex = ring;
      particle->groupIndex = 1;
      particle->SetTexture("effect\\ring_blue.tga");
      particle->angleParameter = 315;
      particle->alpha = 0;

      float& baseAngle = *reinterpret_cast<float*>(&particle->angleBits);
      if (kind == 0) {
        if (ring == 0) {
          particle->scale = 4.5f;
          particle->phaseParameter = 85;
          particle->speedParameter = 27.0f;
          baseAngle = 0.0f;
        } else if (ring == 1) {
          particle->scale = 5.0f;
          particle->phaseParameter = 80;
          particle->speedParameter = 28.0f;
          baseAngle = 90.0f;
        } else if (ring == 2) {
          particle->scale = 5.5f;
          particle->phaseParameter = 70;
          particle->speedParameter = 29.0f;
          baseAngle = 180.0f;
        } else if (ring == 3) {
          particle->scale = 5.0f;
          particle->phaseParameter = 75;
          particle->speedParameter = 30.0f;
          baseAngle = 270.0f;
        }
      } else if (kind == 1) {
        if (ring == 0) {
          particle->scale = 5.5f;
          particle->phaseParameter = 50;
          particle->speedParameter = 30.0f;
          baseAngle = 90.0f;
        } else if (ring == 1) {
          particle->scale = 6.0f;
          particle->phaseParameter = 55;
          particle->speedParameter = 29.0f;
          baseAngle = 180.0f;
        } else if (ring == 2) {
          particle->scale = 6.5f;
          particle->phaseParameter = 65;
          particle->speedParameter = 28.0f;
          baseAngle = 270.0f;
        } else if (ring == 3) {
          particle->scale = 6.0f;
          particle->phaseParameter = 60;
          particle->speedParameter = 27.0f;
          baseAngle = 0.0f;
        }
      }

      particle->SetColor(100, 100, 255);
      particle->animationParameter = 0.0f;
      *reinterpret_cast<float*>(particle->animationParameters) = particle->speedParameter;
      particle->InitParticleQuad(particle->vertices);
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
void CFlowerCastingEffect::Open() {
  this->effectLifetime = 400;

  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  const vector3d& player = reinterpret_cast<const CRenderObject*>(world->player)->render.position;
  g_soundMgr->PlayWave("effect\\T_안락한마법.wav", this->position[0] - player.x, 0.0f, this->position[2] - player.z, 250, 40, 1.0f, 0);

  this->AddRing(0);
  this->AddRing(1);
}

// OFFSET: 0x00B66D20
// FUNCTION: fn_19062
bool CFlowerCastingEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B66D30
// FUNCTION: fn_19063
void CFlowerCastingEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& particlePosition = (*it)->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];

    if (particle && particle->elapsedFrames > 0) {
      int phase = particle->elapsedFrames;
      phase %= 90;
      float radius = g_sinTable[phase * 4] * 2.5f + *reinterpret_cast<const float*>(particle->animationParameters);
      particle->speedParameter = radius;

      const int age = particle->elapsedFrames;
      int alpha = particle->alpha;
      if (age > 110) {
        alpha = (std::max)(alpha - 1, 0);
      } else if (age < 20) {
        alpha = (std::min)(alpha + 5, 180);
      }
      particle->alpha = alpha;

      const int segment = particle->columnIndex;
      radius *= g_sinTable[segment * 9];
      if (age <= 90) {
        radius *= g_sinTable[age];
      }
      radius = (std::max)(radius, 0.0f);

      const float baseAngle = *reinterpret_cast<const float*>(&particle->angleBits);
      const int angle = static_cast<int>(static_cast<int>(particle->angleParameter / 20.0f) * segment + baseAngle) % 360;
      const int tilt = particle->phaseParameter;
      const float spread = radius * g_cosTable[tilt];
      const float height = radius * g_sinTable[tilt];
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      const float offsetX = spread * cosine;
      const float offsetZ = spread * sine;

      EffectPoint points[2];
      const float innerX = particle->scale * cosine;
      const float innerY = 0.0f;
      const float innerZ = particle->scale * sine;
      const float outerX = innerX + offsetX;
      const float outerY = 0.0f - height;
      const float outerZ = innerZ + offsetZ;

      particle->vertices[3].x = innerX;
      particle->vertices[3].y = innerY;
      particle->vertices[3].z = innerZ;
      particle->vertices[1].x = outerX;
      particle->vertices[1].y = outerY;
      particle->vertices[1].z = outerZ;

      if (segment != 0) {
        particle->vertices[0].x = this->lastOuter.x;
        particle->vertices[0].y = this->lastOuter.y;
        particle->vertices[0].z = this->lastOuter.z;
        particle->vertices[2].x = this->lastInner.x;
        particle->vertices[2].y = this->lastInner.y;
        particle->vertices[2].z = this->lastInner.z;
      }

      this->lastInner.x = innerX;
      this->lastInner.y = innerY;
      this->lastInner.z = innerZ;
      this->lastOuter.x = outerX;
      this->lastOuter.y = outerY;
      this->lastOuter.z = outerZ;
    }

    ++(*it)->elapsedFrames;
  }
}

// OFFSET: 0x00B671E0
// FUNCTION: fn_19064
void CFlowerCastingEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || particle->columnIndex <= 0) {
      continue;
    }

    const int segment = particle->columnIndex;
    ParticleQuad quad;
    quad.billboard = false;
    quad.unused = false;
    quad.depth = true;
    quad.top = 0.0f;
    quad.bottom = 1.0f;
    quad.left = static_cast<float>(segment - 1) / 21.0f;
    quad.right = static_cast<float>(segment) / 21.0f;
    particle->Render(quad);
  }
}
