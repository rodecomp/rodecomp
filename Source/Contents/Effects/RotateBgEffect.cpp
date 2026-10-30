#include "Contents/Effects/RotateBgEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00B72760
// FUNCTION: fn_19141
void CRotateBgEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();

  for (CParticle* particle : this->segmentParticles) {
    FreeParticle(particle);
  }

  this->segmentParticles.clear();
}

// OFFSET: 0x00B727D0
// FUNCTION: fn_19142
void CRotateBgEffect::Slot01() {
  for (int ring = 0; ring < 2; ++ring) {
    const int start = std::max(rand() % 360, 0);
    for (int index = 0; index < 10; ++index) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->unknown_0004 = this->id;
        particle->unknown_000C = index;
        particle->unknown_0010 = ring;
        particle->unknown_0008 = 1;
        particle->SetTexture("effect\\storm2.tga");
        particle->alpha = 0;
        particle->scale = 2.0f;
        particle->SetColor(50, 50, 255);
        float* values = &particle->unknown_0054;
        values[0] = 40.0f;
        values[1] = 30.0f;
        values[2] = 5.0f;
        particle->unknown_004C = start;
        this->particles.push_back(particle);
      }
    }
  }

  int layer = 0;
  for (int angle = 0; angle < 360; angle += 90) {
    const int span = std::max(rand() % 21, 0) + 80;
    for (int index = 0; index < 21; ++index) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->unknown_0004 = this->id;
        particle->unknown_000C = index;
        particle->unknown_0010 = layer;
        particle->unknown_0008 = 2;
        particle->SetTexture("effect\\cloud11.tga");
        particle->unknown_0050 = 30;
        *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(angle);
        particle->alpha = 0;
        particle->unknown_004C = span;
        particle->unknown_0054 = 28.0f - static_cast<float>(layer) * 5.0f;
        particle->scale = 7.0f - static_cast<float>(layer);
        this->segmentParticles.push_back(particle);
      }
    }

    ++layer;
  }
}

// OFFSET: 0x00B729E0
// FUNCTION: fn_19143
bool CRotateBgEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B729F0
// FUNCTION: fn_19144
void CRotateBgEffect::Slot03() {
  for (CParticle* particle : this->particles) {
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
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
        const int index = particle->unknown_000C;
        const float* values = &particle->unknown_0054;
        float height;
        if (index == 0) {
          this->height = -values[1];
          this->radius = values[0];
          const int step = std::max(rand() % 3, 0);
          this->angle = (particle->unknown_004C + 3 + step) % 360;
          particle->unknown_004C = this->angle;
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

    ++particle->unknown_00E8;
  }

  for (CParticle* particle : this->segmentParticles) {
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      if (particle->unknown_000C == 0) {
        this->segmentAngle = static_cast<int>(*reinterpret_cast<float*>(&particle->unknown_00C4));
        this->segmentSpan = particle->unknown_0050;
        this->segmentAlpha = particle->alpha;
        this->segmentScale = particle->scale;
        this->segmentReady = 1;
      } else if (!this->segmentReady) {
        ++particle->unknown_00E8;
        continue;
      }

      this->segmentAngle += 5;
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(this->segmentAngle % 360);
      this->segmentSpan += 3;
      if (this->segmentSpan > 180) {
        this->segmentSpan = 180;
      }

      particle->unknown_0050 = this->segmentSpan;
      if (particle->unknown_00E8 <= 12) {
        ++this->segmentAlpha;
        particle->alpha = this->segmentAlpha;
      } else {
        this->segmentScale += 0.1f;
        particle->scale = this->segmentScale;
        if (particle->unknown_00E8 > 50) {
          if (--this->segmentAlpha < 0) {
            this->segmentAlpha = 0;
          }

          particle->alpha = this->segmentAlpha;
        }
      }

      const int index = particle->unknown_000C;
      const int span = particle->unknown_0050;
      const float rotation = *reinterpret_cast<float*>(&particle->unknown_00C4);
      int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(span) / 21.0f) * index) + rotation) % 360;
      if (span == 360 && index == 20) {
        angle = static_cast<int>(rotation);
      }

      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      const int tilt = particle->unknown_004C;
      const float tiltCosine = g_cosTable[tilt];
      const float scale = particle->scale;
      const float innerX = scale * cosine;
      const float innerY = -particle->unknown_0054;
      const float innerZ = scale * sine;
      const float outerX = innerX + tiltCosine * cosine;
      const float outerY = innerY - g_sinTable[tilt];
      const float outerZ = innerZ + tiltCosine * sine;
      particle->vertices[3].x = innerX;
      particle->vertices[3].y = innerY;
      particle->vertices[1].z = outerZ;
      particle->vertices[1].x = outerX;
      particle->vertices[1].y = outerY;
      particle->vertices[3].z = innerZ;
      if (index != 0) {
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

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B734E0
// FUNCTION: fn_19145
void CRotateBgEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }

  for (auto it = this->segmentParticles.begin(); it != this->segmentParticles.end(); ++it) {
    if (CParticle* particle = *it) {
      const int segment = particle->unknown_000C;
      const ParticleQuad quad = {false, 0.0f, 1.0f, (segment - 1) / 21.0f, segment / 21.0f, false, true};
      particle->Render(quad);
    }
  }
}
