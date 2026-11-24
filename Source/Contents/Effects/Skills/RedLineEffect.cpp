#include "Contents/Effects/Skills/RedLineEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Render/Projection/RenderProjection.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

namespace {
// INLINED
__forceinline int ClampedRandom(int range) {
  const int value = rand() % range;
  return value >= 0 ? value : 0;
}

// INLINED
void SetSegment(CParticle* particle, int effectId, int row, int segment) {
  particle->rowIndex = row;
  particle->effectId = effectId;
  particle->columnIndex = segment;
}
}

// OFFSET: 0x00B715F0
// FUNCTION: fn_46049
CRedLineEffect::CRedLineEffect() : CEffect(EF_REDLINE) {
}

// OFFSET: 0x00B71690
// FUNCTION: fn_46050
__declspec(noinline) CRedLineEffect::~CRedLineEffect() {
}

// OFFSET: 0x00B71770
// FUNCTION: fn_19134
void CRedLineEffect::CreateLines(int layer) {
  for (int row = 0; row < 4; ++row) {
    const int life = ClampedRandom(40) + 60;
    const float scale = static_cast<float>(ClampedRandom(11)) * 0.015f + 0.15f;
    const short direction = static_cast<short>(ClampedRandom(360));
    const float directionX = g_cosTable[direction] * 5.0f;
    const float directionZ = g_sinTable[direction] * 5.0f;

    for (int segment = 0; segment <= 21; ++segment) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (particle) {
        particle->SetTexture("effect\\ring_red.tga");
        SetSegment(particle, this->id, row, segment);
        particle->groupIndex = 3;
        particle->angleParameter = 360;
        particle->angle = static_cast<float>(row * 90);
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
  }
}

// OFFSET: 0x00B71940
// FUNCTION: fn_46051
__forceinline void CRedLineEffect::UpdateSegmentVertices(CParticle* particle) {
  if (!particle) {
    return;
  }

  const int age = particle->elapsedFrames;
  const int segment = particle->columnIndex;
  float width;

  if (age < 100) {
    width = 0.0f;
  } else if (age <= 115) {
    width = g_sinTable[(age - 100) * 6] * particle->speedParameter;
  } else {
    width = particle->speedParameter;
  }

  width = std::max(width, 0.0f);

  const int step = static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f);
  const int angle = static_cast<int>(static_cast<float>(step * segment) + particle->angle) % 360;
  const float cosAngle = g_cosTable[angle];
  const float sinAngle = g_sinTable[angle];
  const float flat = width * g_cosTable[particle->phaseParameter];
  const float rise = width * g_sinTable[particle->phaseParameter];
  float outerX = flat * cosAngle;
  float outerZ = flat * sinAngle;

  EffectPoint points[2];
  float innerX = particle->scale * cosAngle;
  float innerY = 0.0f;
  float innerZ = particle->scale * sinAngle;

  if (1.0f == particle->animationParameter) {
    innerX *= 1.5f;
    innerZ *= 1.5f;
  }

  outerX += innerX;
  float outerY = innerY - rise;
  outerZ += innerZ;

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
    particle->vertices[0].x = this->previousPoints[1].x;
    particle->vertices[0].y = this->previousPoints[1].y;
    particle->vertices[0].z = this->previousPoints[1].z;
    particle->vertices[2].x = this->previousPoints[0].x;
    particle->vertices[2].y = this->previousPoints[0].y;
    particle->vertices[2].z = this->previousPoints[0].z;
  }

  this->previousPoints[0].x = innerX;
  this->previousPoints[0].y = innerY;
  this->previousPoints[0].z = innerZ;
  this->previousPoints[1].x = outerX;
  this->previousPoints[1].y = outerY;
  this->previousPoints[1].z = outerZ;
}

// OFFSET: 0x00B71BF0
// FUNCTION: fn_46052
__forceinline void CRedLineEffect::UpdateSegment(CParticle* particle) {
  if (!particle) {
    return;
  }

  float spin = particle->angle + 1.0f;
  const int age = particle->elapsedFrames;
  if (spin >= 360.0f) {
    spin -= 360.0f;
  }
  particle->angle = spin;

  if (age >= 125) {
    const int alpha = particle->alpha - 15;
    if (alpha < 0) {
      particle->alpha = 0;
    } else {
      particle->alpha = alpha;
    }
    particle->speedParameter = std::max(particle->speedParameter - 5.0f, 0.0f);
  }

  this->UpdateSegmentVertices(particle);
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

    this->UpdateSegment(particle);

    ++particle->elapsedFrames;
  }
}

// INLINED
template <bool zeroFirst>
static __forceinline void RenderRedLineSegment(CParticle* particle) {
  if (particle && particle->columnIndex > 0) {
    int blend;
    short shade;

    if (zeroFirst) {
      if (0.0f == particle->animationParameter) {
        shade = 10;
        blend = 6;
      } else {
        shade = 100;
        blend = 2;
      }
    } else {
      if (particle->animationParameter == 0.0f) {
        shade = 10;
        blend = 6;
      } else {
        shade = 100;
        blend = 2;
      }
    }

    particle->destinationBlend = blend;
    particle->SetColor(255, shade, shade);

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

// OFFSET: 0x00B72180
// FUNCTION: fn_46053
void CRedLineEffect::RenderSegment(CParticle* particle) {
  RenderRedLineSegment<true>(particle);
}

// OFFSET: 0x00B72240
// FUNCTION: fn_19139
void CRedLineEffect::Render() {
  for (std::vector<CParticle*>::iterator it = this->lines.begin(); it != this->lines.end(); ++it) {
    CParticle* particle = *it;

    RenderRedLineSegment<false>(particle);
  }
}
