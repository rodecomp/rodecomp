#include "Contents/Effects/RotateLineEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00B741B0
// FUNCTION: fn_19153
CRotateLineEffect::CRotateLineEffect() : CEffect(-1), scale(0), angle(0), spread(0), alpha(0), started(false), previous{} {
}

// OFFSET: 0x00B74360
// FUNCTION: fn_19155
void CRotateLineEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B743A0
// FUNCTION: fn_19156
void CRotateLineEffect::Slot01() {
  int ring = 0;
  for (int startAngle = 0; startAngle < 360; startAngle += 90) {
    const int tilt = std::max(rand() % 21, 0) + 80;
    for (int i = 0; i < 21; ++i) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->unknown_0004 = this->id;
      particle->unknown_000C = i;
      particle->unknown_0010 = ring;
      particle->unknown_0008 = 1;
      particle->SetTexture("effect\\cloud11.tga");
      particle->unknown_0050 = 30;
      *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(startAngle);
      particle->alpha = 50;
      particle->unknown_004C = tilt;
      particle->scale = 7.0f - static_cast<float>(ring);
      particle->unknown_0054 = 28.0f - static_cast<float>(ring) * 5.0f;
      *reinterpret_cast<float*>(particle->unknown_0058) = 3.0f;
      switch (this->id) {
      case 986:
        particle->SetColor(50, 50, 50);
        particle->destinationBlend = 6;
        break;
      case 1000:
        particle->SetColor(100, 100, 255);
        particle->scale += 3.0f;
        break;
      }

      this->particles.push_back(particle);
    }
    ++ring;
  }
}

// OFFSET: 0x00B74580
// FUNCTION: fn_19157
bool CRotateLineEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B74590
// FUNCTION: fn_19158
void CRotateLineEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->position.x = this->position[0];
    (*it)->position.y = this->position[1];
    (*it)->position.z = this->position[2];
    if (CParticle* particle = *it) {
      float& particleAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
      int& particleSpread = particle->unknown_0050;
      if (particle->unknown_000C == 0) {
        this->angle = static_cast<int>(particleAngle);
        this->spread = particleSpread;
        this->alpha = particle->alpha;
        this->scale = particle->scale;
        this->started = true;
      } else if (!this->started) {
        ++(*it)->unknown_00E8;
        continue;
      }

      this->angle += 5;
      particleAngle = static_cast<float>(this->angle % 360);
      this->spread += 3;
      if (this->spread > 180) {
        this->spread = 180;
      }
      particleSpread = this->spread;

      if (particle->unknown_00E8 <= 12) {
        particle->alpha = ++this->alpha;
      } else {
        this->scale += 0.1f;
        particle->scale = this->scale;
        if (particle->unknown_00E8 > 50) {
          if (--this->alpha < 0) {
            this->alpha = 0;
          }
          particle->alpha = this->alpha;
        }
      }

      const int segment = particle->unknown_000C;
      int index = static_cast<int>(static_cast<float>(static_cast<int>(particleSpread / 21.0f) * segment) + particleAngle) % 360;
      if (particleSpread == 360 && segment == 20) {
        index = static_cast<int>(particleAngle);
      }

      const float cosine = g_cosTable[index];
      const float sine = g_sinTable[index];
      const int tilt = particle->unknown_004C;
      const float tiltCosine = g_cosTable[tilt];
      vector3d points[2] = {};
      const float height = *reinterpret_cast<float*>(particle->unknown_0058);
      points[0].x = cosine * particle->scale;
      points[0].y = -particle->unknown_0054;
      points[0].z = sine * particle->scale;
      points[1].x = points[0].x + tiltCosine * cosine;
      points[1].y = height - g_sinTable[tilt] + points[0].y;
      points[1].z = points[0].z + tiltCosine * sine;

      particle->vertices[3].x = points[0].x;
      particle->vertices[3].y = points[0].y;
      particle->vertices[3].z = points[0].z;
      particle->vertices[1].x = points[1].x;
      particle->vertices[1].y = points[1].y;
      particle->vertices[1].z = points[1].z;
      if (segment != 0) {
        particle->vertices[0].x = this->previous[1].x;
        particle->vertices[0].y = this->previous[1].y;
        particle->vertices[0].z = this->previous[1].z;
        particle->vertices[2].x = this->previous[0].x;
        particle->vertices[2].y = this->previous[0].y;
        particle->vertices[2].z = this->previous[0].z;
      }
      this->previous[0] = points[0];
      this->previous[1] = points[1];
    }
    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x00B74B00
// FUNCTION: fn_19159
void CRotateLineEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (CParticle* particle = *it) {
      const int segment = particle->unknown_000C;
      const ParticleQuad quad = {false, 0.0f, 1.0f, (segment - 1) / 21.0f, segment / 21.0f, false, true};
      particle->Render(quad);
    }
  }
}
