#include "Contents/Effects/RedLineEffect.h"

#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00B71EA0
// FUNCTION: fn_19138
void CRedLineEffect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->lines.begin(); it != this->lines.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = particle->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];

    if (particle != nullptr) {
      float& spinAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
      float spin = spinAngle + 1.0f;
      const int age = particle->unknown_00E8;

      if (spin >= 360.0f) {
        spin -= 360.0f;
      }

      spinAngle = spin;

      float width;

      if (age < 125) {
        if (age < 100) {
          width = 0.0f;
        } else if (age <= 115) {
          width = g_sinTable[(age - 100) * 6] * particle->unknown_00C0;
        } else {
          width = particle->unknown_00C0;
        }
      } else {
        const float shrunk = particle->unknown_00C0 - 5.0f;
        const int alpha = particle->alpha - 15;
        particle->alpha = alpha < 0 ? 0 : alpha;
        particle->unknown_00C0 = shrunk < 0.0f ? 0.0f : shrunk;
        width = particle->unknown_00C0;
      }

      if (width < 0.0f) {
        width = 0.0f;
      }

      const int segment = particle->unknown_000C;
      const int step = static_cast<int>(static_cast<float>(particle->unknown_0050) / 20.0f);
      const int angle = static_cast<int>(static_cast<float>(step * segment) + spin) % 360;
      const float flat = width * g_cosTable[particle->unknown_004C];
      const float rise = width * g_sinTable[particle->unknown_004C];

      vector3d points[2]{};
      points[0].x = particle->scale * g_cosTable[angle];
      points[0].z = particle->scale * g_sinTable[angle];

      if (particle->unknown_0054 == 1.0f) {
        points[0].x *= 1.5f;
        points[0].z *= 1.5f;
      }

      points[1].x = flat * g_cosTable[angle] + points[0].x;
      points[1].y = points[0].y - rise;
      points[1].z = flat * g_sinTable[angle] + points[0].z;

      points[0].x += particle->unknown_00D0;
      points[0].y += particle->unknown_00D4;
      points[0].z += particle->unknown_00D8;
      points[1].x += particle->unknown_00D0;
      points[1].y += particle->unknown_00D4;
      points[1].z += particle->unknown_00D8;

      particle->vertices[1].x = points[1].x;
      particle->vertices[1].y = points[1].y;
      particle->vertices[1].z = points[1].z;
      particle->vertices[3].x = points[0].x;
      particle->vertices[3].y = points[0].y;
      particle->vertices[3].z = points[0].z;

      if (segment != 0) {
        particle->vertices[0].x = this->previousOuter.x;
        particle->vertices[0].y = this->previousOuter.y;
        particle->vertices[0].z = this->previousOuter.z;
        particle->vertices[2].x = this->previousInner.x;
        particle->vertices[2].y = this->previousInner.y;
        particle->vertices[2].z = this->previousInner.z;
      }

      this->previousInner = points[0];
      this->previousOuter = points[1];
    }

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B72240
// FUNCTION: fn_19139
void CRedLineEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->lines.begin(); it != this->lines.end(); ++it) {
    CParticle* particle = *it;

    if (particle && particle->unknown_000C > 0) {
      int blend;
      short shade;

      if (particle->unknown_0054 == 0.0f) {
        shade = 10;
        blend = 6;
      } else {
        shade = 100;
        blend = 2;
      }

      particle->destinationBlend = blend;
      particle->SetColor(255, shade, shade);

      const int segment = particle->unknown_000C;
      const ParticleQuad quad = {false, 0.0f, 1.0f, static_cast<float>(segment - 1) / 21.0f, static_cast<float>(segment) / 21.0f, false, true};
      particle->Render(quad);
    }
  }
}
