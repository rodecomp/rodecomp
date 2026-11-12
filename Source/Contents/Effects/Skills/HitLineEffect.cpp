#include "Contents/Effects/Skills/HitLineEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

struct HitLineVector3d : vector3d {
  // INLINED
  HitLineVector3d() {
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
  }
};

// OFFSET: 0x00705040
// FUNCTION: fn_07035
void CHitLineEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00705100
// FUNCTION: fn_07036
void CHitLineEffect::Open() {
  for (int pass = 0; pass < 4; ++pass) {
    for (int line = 0; line < 4; ++line) {
      const int delayRoll = rand() % 26;
      const int delay = -(delayRoll >= 0 ? delayRoll : 0);
      const int brightnessRoll = rand() % 51;
      const int brightness = (brightnessRoll >= 0 ? brightnessRoll : 0) + 125;
      const int angleRoll = rand() % 360;
      const int angle = angleRoll >= 0 ? angleRoll : 0;
      for (int segment = 0; segment < 20; ++segment) {
        CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
        if (particle) {
          particle->SetTexture("effect\\white02.bmp");
          particle->rowIndex = line;
          particle->effectId = this->id;
          particle->columnIndex = segment;
          particle->groupIndex = 4;
          particle->elapsedFrames = delay;
          particle->scale = 0.1f;
          *reinterpret_cast<int*>(particle->effectParameters) = brightness;
          particle->phaseParameter = angle;
          particle->speedParameter = 14.0f;
          for (int index = 0; index < 21; ++index) {
            (&particle->animationState)[index] = 0;
            (&particle->animationParameter)[index] = 0.0f;
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
bool CHitLineEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00705280
// FUNCTION: fn_07038
void CHitLineEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& particlePosition = (*it)->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];
    if (particle && particle->elapsedFrames > 0) {
      int& brightness = *reinterpret_cast<int*>(particle->effectParameters);
      short level = static_cast<short>(brightness);
      if (particle->elapsedFrames > 35) {
        level -= 5;
      }
      brightness = std::max<short>(0, level);

      float* turns = &particle->animationParameter;
      unsigned char* halfTurns = &particle->animationState;
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

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00705510
// FUNCTION: fn_07039
void CHitLineEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle || *reinterpret_cast<int*>(particle->effectParameters) <= 0 || particle->elapsedFrames <= 0) {
      continue;
    }

    const int segment = particle->columnIndex;
    HitLineVector3d points[2];
    const float radius = particle->speedParameter;
    const float* turns = &particle->animationParameter;
    const unsigned char* halfTurns = &particle->animationState;
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
      points[index].x = g_cosTable[particle->phaseParameter] * distance;
      points[index].z = g_sinTable[particle->phaseParameter] * distance;
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
      HitLineVector3d edge[2];
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
        const int alpha = *reinterpret_cast<int*>(particle->effectParameters) - segment * 5;
        if (alpha > 0) {
          particle->alpha = alpha;
          particle->SetColor(255, 200, static_cast<short>(segment * 7 + 10));
          particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
        }
      }

      ++corner;
      this->previous[0].x = edge[0].x;
      this->previous[0].y = edge[0].y;
      this->previous[0].z = edge[0].z;
      this->previous[1].x = edge[1].x;
      this->previous[1].y = edge[1].y;
      this->previous[1].z = edge[1].z;
    }
  }
}
