#include "Contents/Effects/Skills/RedLineEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

namespace {
// INLINED
__forceinline int ClampedRandom(int range) {
  const int value = rand() % range;
  return value >= 0 ? value : 0;
}
}

// OFFSET: 0x00B71770
// FUNCTION: fn_19134
void CRedLineEffect::CreateLines(int layer) {
  int row = 0;
  for (int angle = 0; angle < 360; angle += 90) {
    const int life = ClampedRandom(40) + 60;
    const float scale = static_cast<float>(ClampedRandom(11)) * 0.015f + 0.15f;
    const short direction = static_cast<short>(ClampedRandom(360));
    const float directionX = g_cosTable[direction] * 5.0f;
    const float directionZ = g_sinTable[direction] * 5.0f;

    for (int segment = 0; segment <= 21; ++segment) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->SetTexture("effect\\ring_red.tga");
        particle->rowIndex = row;
        particle->effectId = this->id;
        particle->columnIndex = segment;
        particle->groupIndex = 3;
        particle->angleParameter = 360;
        particle->angle = static_cast<float>(angle);
        particle->alpha = 150;
        particle->speedParameter = 120.0f;
        particle->phaseParameter = 90;
        particle->scale = scale;
        particle->offsetX = directionX;
        particle->offsetZ = directionZ;
        particle->offsetY = 0.0f;
        particle->elapsedFrames = life;
        particle->animationParameter = static_cast<float>(layer);
        particle->InitParticleQuad(particle->vertices);
        this->lines.push_back(particle);
      }
    }

    ++row;
  }
}

// OFFSET: 0x00B71B80
// FUNCTION: fn_19135
void CRedLineEffect::Close() {
  for (std::vector<CParticle*>::iterator it = this->lines.begin(); it != this->lines.end(); ++it) {
    FreeParticle(*it);
  }

  this->lines.clear();
}

// OFFSET: 0x00B71BC0
// FUNCTION: fn_19136
void CRedLineEffect::Open() {
  this->CreateLines(0);
  this->CreateLines(1);
  this->effectLifetime = 75;
}

// OFFSET: 0x00B71BE0
// FUNCTION: fn_19137
bool CRedLineEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B71EA0
// FUNCTION: fn_19138
void CRedLineEffect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->lines.begin(); it != this->lines.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = particle->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];

    if (particle != nullptr) {
      float& spinAngle = particle->angle;
      float spin = spinAngle + 1.0f;
      const int age = particle->elapsedFrames;

      if (spin >= 360.0f) {
        spin -= 360.0f;
      }

      spinAngle = spin;

      float width;

      if (age >= 125) {
        const int alpha = particle->alpha - 15;
        particle->alpha = alpha >= 0 ? alpha : 0;
        particle->speedParameter = std::max(0.0f, particle->speedParameter - 5.0f);
      }

      if (age < 100) {
        width = 0.0f;
      } else if (age <= 115) {
        width = g_sinTable[(age - 100) * 6] * particle->speedParameter;
      } else {
        width = particle->speedParameter;
      }

      width = std::max(0.0f, width);

      const int segment = particle->columnIndex;
      const int step = static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f);
      const int angle = static_cast<int>(static_cast<float>(step * segment) + spin) % 360;
      const float flat = width * g_cosTable[particle->phaseParameter];
      const float rise = width * g_sinTable[particle->phaseParameter];

      EffectPoint points[2];
      float innerX = particle->scale * g_cosTable[angle];
      float innerY = 0.0f;
      float innerZ = particle->scale * g_sinTable[angle];

      if (particle->animationParameter == 1.0f) {
        innerX *= 1.5f;
        innerZ *= 1.5f;
      }

      float outerX = flat * g_cosTable[angle] + innerX;
      float outerY = innerY - rise;
      float outerZ = flat * g_sinTable[angle] + innerZ;

      innerX += particle->offsetX;
      innerY += particle->offsetY;
      innerZ += particle->offsetZ;
      outerX += particle->offsetX;
      outerY += particle->offsetY;
      outerZ += particle->offsetZ;

      particle->vertices[1].x = outerX;
      particle->vertices[1].y = outerY;
      particle->vertices[1].z = outerZ;
      particle->vertices[3].x = innerX;
      particle->vertices[3].y = innerY;
      particle->vertices[3].z = innerZ;

      if (segment != 0) {
        particle->vertices[0].x = this->previousOuter.x;
        particle->vertices[0].y = this->previousOuter.y;
        particle->vertices[0].z = this->previousOuter.z;
        particle->vertices[2].x = this->previousInner.x;
        particle->vertices[2].y = this->previousInner.y;
        particle->vertices[2].z = this->previousInner.z;
      }

      this->previousInner.x = innerX;
      this->previousInner.y = innerY;
      this->previousInner.z = innerZ;
      this->previousOuter.x = outerX;
      this->previousOuter.y = outerY;
      this->previousOuter.z = outerZ;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B72240
// FUNCTION: fn_19139
void CRedLineEffect::Render() {
  for (std::vector<CParticle*>::iterator it = this->lines.begin(); it != this->lines.end(); ++it) {
    CParticle* particle = *it;

    if (particle && particle->columnIndex > 0) {
      int blend;
      short shade;

      if (particle->animationParameter == 0.0f) {
        shade = 10;
        blend = 6;
      } else {
        shade = 100;
        blend = 2;
      }

      particle->destinationBlend = blend;
      particle->SetColor(255, shade, shade);

      const int segment = particle->columnIndex;
      const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(segment - 1) / 21.0f, static_cast<float>(segment) / 21.0f, false, true};
      particle->Render(quad);
    }
  }
}
