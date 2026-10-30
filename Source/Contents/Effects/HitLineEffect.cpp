#include "Contents/Effects/HitLineEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00705040
// FUNCTION: fn_07035
void CHitLineEffect::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00705100
// FUNCTION: fn_07036
void CHitLineEffect::Slot01() {
  for (int pass = 0; pass < 4; ++pass) {
    for (int line = 0; line < 4; ++line) {
      const int delay = -std::max(rand() % 26, 0);
      const int brightness = std::max(rand() % 51, 0) + 125;
      const int angle = std::max(rand() % 360, 0);
      for (int segment = 0; segment < 20; ++segment) {
        CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
        if (particle) {
          particle->SetTexture("effect\\white02.bmp");
          particle->unknown_0010 = line;
          particle->unknown_0004 = this->id;
          particle->unknown_000C = segment;
          particle->unknown_0008 = 4;
          particle->unknown_00E8 = delay;
          particle->scale = 0.1f;
          *reinterpret_cast<int*>(particle->unknown_0048) = brightness;
          particle->unknown_004C = angle;
          particle->unknown_00C0 = 14.0f;
          float* turns = &particle->unknown_0054;
          unsigned char* halfTurns = &particle->unknown_00A8;
          for (int index = 0; index < 21; ++index) {
            halfTurns[index] = 0;
            turns[index] = 0.0f;
          }
          InitParticleQuad(particle->vertices);
          this->particles.push_back(particle);
        }
      }
    }
  }
}

// OFFSET: 0x00705270
// FUNCTION: fn_07037
bool CHitLineEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00705280
// FUNCTION: fn_07038
void CHitLineEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle && particle->unknown_00E8 > 0) {
      int& brightness = *reinterpret_cast<int*>(particle->unknown_0048);
      short level = static_cast<short>(brightness);
      if (particle->unknown_00E8 > 35) {
        level -= 5;
      }
      brightness = std::max<short>(level, 0);

      float* turns = &particle->unknown_0054;
      unsigned char* halfTurns = &particle->unknown_00A8;
      float turn = turns[0] + 8.0f;
      if (turn > 180.0f) {
        ++halfTurns[0];
        turn -= 180.0f;
      }
      turns[0] = turn;

      for (int index = 20; index > 0; --index) {
        turns[index] = turns[index - 1];
        halfTurns[index] = halfTurns[index - 1];
      }
    }

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00705510
// FUNCTION: fn_07039
void CHitLineEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || *reinterpret_cast<int*>(particle->unknown_0048) <= 0 || particle->unknown_00E8 <= 0) {
      continue;
    }

    const int segment = particle->unknown_000C;
    vector3d points[2] = {};
    const float radius = particle->unknown_00C0;
    const float* turns = &particle->unknown_0054;
    const unsigned char* halfTurns = &particle->unknown_00A8;
    for (int index = 0; index < 2; ++index) {
      const int count = static_cast<signed char>(halfTurns[segment + index]);
      float distance = 0.0f;
      float factor = 1.0f;
      for (int step = 0; step < count; ++step) {
        distance += radius * 2.0f * factor;
        factor *= 0.7f;
      }

      const float turn = turns[segment + index];
      if (turn > 0.0f) {
        distance += radius * factor * (turn / 90.0f);
      }

      float height = -radius * g_sinTable[static_cast<int>(turn)] * 1.5f;
      float scale = 1.0f;
      for (int step = 0; step < count; ++step) {
        scale *= 0.7f;
      }

      points[index].y = height * scale;
      points[index].x = g_cosTable[particle->unknown_004C] * distance;
      points[index].z = g_sinTable[particle->unknown_004C] * distance;
    }

    const float dz = points[0].z - points[1].z;
    const float dx = points[0].x - points[1].x;
    const float length = SquareRoot(dz * dz + dx * dx);
    const float cosine = dx / length;
    const float sine = dz / length;
    short direction = 0;
    while (true) {
      if (cosine >= g_cosTable[direction]) {
        if (!(sine >= 0.0f)) {
          direction = static_cast<short>(360 - direction);
        }
        break;
      }

      ++direction;
      if (direction >= 180) {
        direction = 180;
        break;
      }
    }

    const int heading = (direction + 90) % 360;
    int corner = 0;
    for (int degree = 0; degree <= 360; degree += 120) {
      const float size = particle->scale;
      const float up = size * g_sinTable[degree];
      const float side = size * g_cosTable[degree];
      const float offsetX = side * g_cosTable[heading];
      const float offsetZ = side * g_sinTable[heading];
      vector3d edge[2] = {};
      edge[0].x = offsetX + points[0].x;
      edge[0].y = up + points[0].y;
      edge[0].z = offsetZ + points[0].z;
      edge[1].x = offsetX + points[1].x;
      edge[1].y = up + points[1].y;
      edge[1].z = offsetZ + points[1].z;
      particle->vertices[1].x = edge[1].x;
      particle->vertices[1].y = edge[1].y;
      particle->vertices[3].x = edge[0].x;
      particle->vertices[3].y = edge[0].y;
      particle->vertices[3].z = edge[0].z;
      particle->vertices[1].z = edge[1].z;
      if (corner > 0) {
        particle->vertices[0].x = this->previous[1].x;
        particle->vertices[0].y = this->previous[1].y;
        particle->vertices[0].z = this->previous[1].z;
        particle->vertices[2].x = this->previous[0].x;
        particle->vertices[2].y = this->previous[0].y;
        particle->vertices[2].z = this->previous[0].z;
        const int alpha = *reinterpret_cast<int*>(particle->unknown_0048) - segment * 5;
        if (alpha > 0) {
          particle->alpha = alpha;
          particle->SetColor(255, 200, static_cast<short>(segment * 7 + 10));
          particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
        }
      }

      ++corner;
      this->previous[0] = edge[0];
      this->previous[1] = edge[1];
    }
  }
}
