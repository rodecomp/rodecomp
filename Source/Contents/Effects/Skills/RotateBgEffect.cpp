#include "Contents/Effects/Skills/RotateBgEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

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
  for (int ring = 0; ring < 2; ++ring) {
    const int startRoll = rand() % 360;
    const int start = startRoll >= 0 ? startRoll : 0;
    for (int index = 0; index < 10; ++index) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->effectId = this->id;
        particle->columnIndex = index;
        particle->rowIndex = ring;
        particle->groupIndex = 1;
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

  int layer = 0;
  for (int angle = 0; angle < 360; angle += 90) {
    const int spanRoll = rand() % 21;
    const int span = (spanRoll >= 0 ? spanRoll : 0) + 80;
    for (int index = 0; index < 21; ++index) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->effectId = this->id;
        particle->columnIndex = index;
        particle->rowIndex = layer;
        particle->groupIndex = 2;
        particle->SetTexture("effect\\cloud11.tga");
        particle->angleParameter = 30;
        *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(angle);
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

// OFFSET: 0x00B729E0
// FUNCTION: fn_19143
bool CRotateBgEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B729F0
// FUNCTION: fn_19144
void CRotateBgEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    CParticle* particle = *it;
    if (particle) {
      int alpha = particle->alpha;
      if (this->frame > 60) {
        --alpha;
      } else if (this->frame <= 40) {
        ++alpha;
      }

      alpha = std::max(alpha, 0);
      particle->alpha = alpha;
      if (alpha > 0) {
        const int index = particle->columnIndex;
        const float* values = &particle->animationParameter;
        float height;
        if (index == 0) {
          this->height = -values[1];
          this->radius = values[0];
          const int step = std::max(rand() % 3, 0);
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

    ++particle->elapsedFrames;
  }

  for (auto it = this->segmentParticles.begin(); it != this->segmentParticles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    CParticle* particle = *it;
    if (particle) {
      if (particle->columnIndex == 0) {
        this->segmentAngle = static_cast<int>(*reinterpret_cast<float*>(&particle->angleBits));
        this->segmentSpan = particle->angleParameter;
        this->segmentAlpha = particle->alpha;
        this->segmentScale = particle->scale;
        this->segmentReady = 1;
      } else if (!this->segmentReady) {
        ++particle->elapsedFrames;
        continue;
      }

      this->segmentAngle += 5;
      *reinterpret_cast<float*>(&particle->angleBits) = static_cast<float>(this->segmentAngle % 360);
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
          if (--this->segmentAlpha < 0) {
            this->segmentAlpha = 0;
          }

          particle->alpha = this->segmentAlpha;
        }
      }

      const int index = particle->columnIndex;
      const int span = particle->angleParameter;
      const float rotation = *reinterpret_cast<float*>(&particle->angleBits);
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
      particle->vertices[1].z = edge[1].z;
      particle->vertices[1].x = edge[1].x;
      particle->vertices[1].y = edge[1].y;
      particle->vertices[3].z = edge[0].z;
      if (index != 0) {
        particle->vertices[0].x = this->lastOuter.x;
        particle->vertices[0].y = this->lastOuter.y;
        particle->vertices[0].z = this->lastOuter.z;
        particle->vertices[2].x = this->lastInner.x;
        particle->vertices[2].y = this->lastInner.y;
        particle->vertices[2].z = this->lastInner.z;
      }

      this->lastInner.x = edge[0].x;
      this->lastInner.y = edge[0].y;
      this->lastInner.z = edge[0].z;
      this->lastOuter.x = edge[1].x;
      this->lastOuter.y = edge[1].y;
      this->lastOuter.z = edge[1].z;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B734E0
// FUNCTION: fn_19145
void CRotateBgEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (auto it = this->segmentParticles.begin(); it != this->segmentParticles.end(); ++it) {
    if (CParticle* particle = *it) {
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
}
