#include "Contents/Effects/Skills/MappillarEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Particles/Particle.h"
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
void CMappillarEffect::Open() {
  for (int ring = 0; ring < 4; ++ring) {
    for (int segment = 0; segment <= 21; ++segment) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (!particle) {
        continue;
      }

      particle->SetTexture("effect\\ring_blue.tga");
      particle->effectId = this->id;
      particle->columnIndex = segment;
      particle->rowIndex = ring;
      particle->groupIndex = 1;
      particle->alpha = 70;
      particle->SetColor(100, 100, 255);

      particle->angleParameter = 360;
      particle->speedParameter = 120.0f;
      particle->phaseParameter = 89;
      const int row = particle->rowIndex;
      particle->angle = static_cast<float>(row * 90);
      particle->scale = static_cast<float>(row) * 0.5f + 11.0f;
      particle->animationValues[row] = 0.0f;
      particle->elapsedFrames = particle->rowIndex * 30;

      const float baseAngle = particle->angle;
      int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f) * particle->columnIndex) + baseAngle) % 360;
      if (particle->columnIndex == 20) {
        angle = static_cast<int>(baseAngle);
      }

      const int tilt = particle->phaseParameter;
      const float spread = particle->speedParameter * g_cosTable[tilt];
      const float height = particle->speedParameter * g_sinTable[tilt];
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

      if (particle->columnIndex >= 1 && particle->columnIndex <= 20) {
        particle->vertices[1].x = outerX;
        particle->vertices[1].y = outerY;
        particle->vertices[1].z = outerZ;
        particle->vertices[3].x = innerX;
        particle->vertices[3].y = innerY;
        particle->vertices[3].z = innerZ;
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
      this->particles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B4C0B0
// FUNCTION: fn_18957
bool CMappillarEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00B4C0C0
// FUNCTION: fn_18958
void CMappillarEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      if (particle->rowIndex != 3 || particle->columnIndex != 21 || particle->elapsedFrames <= 999) {
        const int frame = particle->elapsedFrames;
        const int angle = static_cast<int>(particle->angle - static_cast<float>(frame));
        float height;
        if (frame < 800) {
          if (frame < 200) {
            height = 0.0f;
          } else if (frame > 290) {
            height = particle->speedParameter;
          } else {
            height = g_sinTable[frame - 200] * particle->speedParameter;
          }
        } else {
          particle->speedParameter = std::max(0.0f, particle->speedParameter - 1.0f);
          height = particle->speedParameter;
        }

        particle->animationParameter = height;
        if (height < 0.0f) {
          particle->animationParameter = 0.0f;
        }

        matrix rotation;
        std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
        rotation.MakeYRotation(static_cast<float>(angle) / 180.0f * 3.141592f);
        matrix scale;
        std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
        scale.SetScale(1.0f, particle->animationParameter / 100.0f * 1.1f, 1.0f);
        const matrix first = rotation;
        rotation.Compose(first, scale);
        particle->transform = rotation;
      }
    }
    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B4C4C0
// FUNCTION: fn_18959
void CMappillarEffect::Render() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
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
