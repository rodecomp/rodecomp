#include "Contents/Effects/Skills/RotateBgEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// INLINED
static inline void SetIndices(CParticle* particle, int group, int column, int row) {
  particle->columnIndex = column;
  particle->rowIndex = row;
  particle->groupIndex = group;
}

// OFFSET: 0x00B72330
// FUNCTION: fn_46060
CRotateBgEffect::CRotateBgEffect()
    : CEffect(-1), height(0.0f), radius(0.0f), segmentScale(0.0f), angle(0), segmentAngle(0), segmentSpan(0), segmentAlpha(0), segmentReady(0) {
}

// OFFSET: 0x00B72430
// FUNCTION: fn_46061
__declspec(noinline) CRotateBgEffect::~CRotateBgEffect() {
}

// OFFSET: 0x00B72530
// FUNCTION: fn_46062
__forceinline void CRotateBgEffect::CreateRings() {
  for (int ring = 0; ring < 2; ++ring) {
    const int startRoll = rand() % 360;
    const int start = startRoll >= 0 ? startRoll : 0;
    for (int index = 0; index < 10; ++index) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (particle) {
        particle->effectId = this->id;
        SetIndices(particle, 1, index, ring);
        particle->SetTexture("effect\\storm2.tga");
        particle->alpha = 0;
        particle->scale = 2.0f;
        particle->SetColor(50, 50, 255);
        float* values = &particle->animationParameter;
        values[0] = 40.0f;
        values[1] = 30.0f;
        values[2] = 5.0f;
        particle->phaseParameter = start;
        this->particles.push_back(particle);
      }
    }
  }
}

// OFFSET: 0x00B72630
// FUNCTION: fn_46063
__forceinline void CRotateBgEffect::CreateSegments() {
  int layer = 0;
  for (int angle = 0; angle < 360; angle += 90) {
    const int spanRoll = rand() % 21;
    const int span = (spanRoll >= 0 ? spanRoll : 0) + 80;
    for (int index = 0; index < 21; ++index) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (particle) {
        particle->effectId = this->id;
        SetIndices(particle, 2, index, layer);
        particle->SetTexture("effect\\cloud11.tga");
        particle->angleParameter = 30;
        particle->angle = static_cast<float>(angle);
        particle->alpha = 0;
        particle->phaseParameter = span;
        particle->animationParameter = 28.0f - static_cast<float>(layer) * 5.0f;
        particle->scale = 7.0f - static_cast<float>(layer);
        this->segmentParticles.push_back(particle);
      }
    }

    ++layer;
  }
}

// OFFSET: 0x00B72760
// FUNCTION: fn_19141
void CRotateBgEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();

  for (auto it = this->segmentParticles.begin(); it != this->segmentParticles.end(); ++it) {
    FreeParticle(*it);
  }

  this->segmentParticles.clear();
}

// OFFSET: 0x00B727D0
// FUNCTION: fn_19142
void CRotateBgEffect::Open() {
  this->CreateRings();
  this->CreateSegments();
}

// OFFSET: 0x00B729E0
// FUNCTION: fn_19143
bool CRotateBgEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B72FD0
// FUNCTION: fn_46064
__forceinline void CRotateBgEffect::UpdateRing(CParticle* particle) {
  if (particle) {
    int alpha = particle->alpha;
    if (this->frame > 60) {
      --alpha;
    } else if (this->frame <= 40) {
      ++alpha;
    }

    alpha = alpha >= 0 ? alpha : 0;
    particle->alpha = alpha;
    if (alpha > 0) {
      const int index = particle->columnIndex;
      const float* values = &particle->animationParameter;
      float height;
      if (index == 0) {
        this->height = -values[1];
        this->radius = values[0];
        const int roll = rand() % 3;
        const int step = roll >= 0 ? roll : 0;
        this->angle = (particle->phaseParameter + 3 + step) % 360;
        particle->phaseParameter = this->angle;
        height = this->height;
      } else {
        this->height = values[2] + this->height;
        height = this->height;
        this->radius = this->radius - (2.2f - static_cast<float>(index) * 0.2f) * particle->scale;
      }

      const short rotationAngle = static_cast<short>((index * 45 + this->angle) % 360);

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeZRotation(static_cast<float>(rotationAngle) / 180.0f * 3.141592f);
      rotation.AppendXRotation(1.570796f);

      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(this->radius * 0.5f, this->radius * 0.5f, 1.0f);

      matrix translation;
      std::memcpy(translation.values, g_identityMatrix, sizeof(translation.values));
      translation.MakeTranslate(0.0f, height, 0.0f);

      matrix scaled;
      scaled.Compose(scale, rotation);
      matrix result;
      result.Compose(scaled, translation);
      particle->transform = result;
    }
  }
}

// OFFSET: 0x00B73690
// FUNCTION: fn_46068
__forceinline void CRotateBgEffect::UpdateSegmentVertices(CParticle* particle) {
  if (particle) {
    const int index = particle->columnIndex;
    const int span = particle->angleParameter;
    const float rotation = particle->angle;
    int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(span) / 21.0f) * index) + rotation) % 360;
    if (span == 360 && index == 20) {
      angle = static_cast<int>(rotation);
    }

    const float cosine = g_cosTable[angle];
    const float sine = g_sinTable[angle];
    const int tilt = particle->phaseParameter;
    const float tiltX = g_cosTable[tilt] * cosine;
    const float tiltZ = g_cosTable[tilt] * sine;
    EffectPoint edge[2];
    const float scale = particle->scale;
    edge[0].x = scale * cosine;
    edge[0].y = -particle->animationParameter;
    edge[0].z = scale * sine;
    edge[1].x = edge[0].x + tiltX;
    edge[1].y = edge[0].y - g_sinTable[tilt];
    edge[1].z = edge[0].z + tiltZ;
    particle->vertices[3].x = edge[0].x;
    particle->vertices[3].y = edge[0].y;
    particle->vertices[3].z = edge[0].z;
    particle->vertices[1].x = edge[1].x;
    particle->vertices[1].y = edge[1].y;
    particle->vertices[1].z = edge[1].z;
    if (index != 0) {
      particle->vertices[0].x = this->previousPoints[1].x;
      particle->vertices[0].y = this->previousPoints[1].y;
      particle->vertices[0].z = this->previousPoints[1].z;
      particle->vertices[2].x = this->previousPoints[0].x;
      particle->vertices[2].y = this->previousPoints[0].y;
      particle->vertices[2].z = this->previousPoints[0].z;
    }

    this->previousPoints[0].x = edge[0].x;
    this->previousPoints[0].y = edge[0].y;
    this->previousPoints[0].z = edge[0].z;
    this->previousPoints[1].x = edge[1].x;
    this->previousPoints[1].y = edge[1].y;
    this->previousPoints[1].z = edge[1].z;
  }
}

// OFFSET: 0x00B73250
// FUNCTION: fn_46065
__forceinline void CRotateBgEffect::UpdateSegment(CParticle* particle) {
  if (particle) {
    if (particle->columnIndex == 0) {
      this->segmentAngle = static_cast<int>(particle->angle);
      this->segmentSpan = particle->angleParameter;
      this->segmentAlpha = particle->alpha;
      this->segmentScale = particle->scale;
      this->segmentReady = 1;
    } else if (!this->segmentReady) {
      return;
    }

    this->segmentAngle += 5;
    particle->angle = static_cast<float>(this->segmentAngle % 360);
    this->segmentSpan += 3;
    if (this->segmentSpan > 180) {
      this->segmentSpan = 180;
    }

    particle->angleParameter = this->segmentSpan;
    if (particle->elapsedFrames <= 12) {
      ++this->segmentAlpha;
      particle->alpha = this->segmentAlpha;
    } else {
      this->segmentScale += 0.1f;
      particle->scale = this->segmentScale;
      if (particle->elapsedFrames > 50) {
        --this->segmentAlpha;
        if (this->segmentAlpha < 0) {
          this->segmentAlpha = 0;
        }

        particle->alpha = this->segmentAlpha;
      }
    }

    this->UpdateSegmentVertices(particle);
  }
}

// OFFSET: 0x00B729F0
// FUNCTION: fn_19144
void CRotateBgEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position = this->positionVector;
    this->UpdateRing(particle);

    ++particle->elapsedFrames;
  }

  for (auto it = this->segmentParticles.begin(); it != this->segmentParticles.end(); ++it) {
    CParticle* particle = *it;
    particle->position = this->positionVector;
    this->UpdateSegment(particle);

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B735D0
// FUNCTION: fn_46066
__forceinline void CRotateBgEffect::RenderRing(CParticle* particle) {
  if (particle) {
    particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }
}

// OFFSET: 0x00B73610
// FUNCTION: fn_46067
__forceinline void CRotateBgEffect::RenderSegment(CParticle* particle) {
  if (particle) {
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

// OFFSET: 0x00B734E0
// FUNCTION: fn_19145
void CRotateBgEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->RenderRing(*it);
  }

  for (auto it = this->segmentParticles.begin(); it != this->segmentParticles.end(); ++it) {
    this->RenderSegment(*it);
  }
}
