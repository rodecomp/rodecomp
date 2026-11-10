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
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
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
      *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(particle->rowIndex * 90);
      particle->scale = static_cast<float>(particle->rowIndex) * 0.5f + 11.0f;
      reinterpret_cast<int*>(&particle->animationParameter)[particle->rowIndex] = 0;
      particle->elapsedFrames = particle->rowIndex * 30;

      const float baseAngle = *reinterpret_cast<const float*>(&particle->angleBits);
      int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f) * particle->columnIndex) + baseAngle) % 360;
      if (particle->columnIndex == 20) {
        angle = static_cast<int>(baseAngle);
      }

      const int tilt = particle->phaseParameter;
      const float spread = particle->speedParameter * g_cosTable[tilt];
      const float height = particle->speedParameter * g_sinTable[tilt];
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];

      EffectPoint points[2];
      points[0].x = particle->scale * cosine;
      points[0].y = 0.0f;
      points[0].z = particle->scale * sine;
      points[1].x = points[0].x + spread * cosine;
      points[1].y = 0.0f - height;
      points[1].z = points[0].z + spread * sine;

      if (particle->columnIndex >= 1 && particle->columnIndex <= 20) {
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

      this->lastInner.x = points[0].x;
      this->lastInner.y = points[0].y;
      this->lastInner.z = points[0].z;
      this->lastOuter.x = points[1].x;
      this->lastOuter.y = points[1].y;
      this->lastOuter.z = points[1].z;
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
        const int angle = static_cast<int>(*reinterpret_cast<float*>(&particle->angleBits) - static_cast<float>(frame));
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
