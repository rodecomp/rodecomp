#include "Contents/Effects/WindEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

// INLINED
static vector3d TransformPoint(const matrix& transform, const vector3d& point) {
  vector3d result;
  result.x = point.y * transform.values[3] + point.x * transform.values[0] + point.z * transform.values[6] + transform.values[9];
  result.y = point.y * transform.values[4] + point.x * transform.values[1] + point.z * transform.values[7] + transform.values[10];
  result.z = point.y * transform.values[5] + point.x * transform.values[2] + point.z * transform.values[8] + transform.values[11];
  return result;
}

// OFFSET: 0x00730210
// FUNCTION: fn_07537
CWindEffect::CWindEffect() : CEffect(2343) {
}

// OFFSET: 0x00730510

// OFFSET: 0x00730810
// FUNCTION: fn_07540
void CWindEffect::Close() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00730850
// FUNCTION: fn_07541
void CWindEffect::Slot01() {
  this->Init();
}

// OFFSET: 0x00730860
// FUNCTION: fn_07542
void CWindEffect::Slot07() {
  this->Close();
  this->Init();
}

// OFFSET: 0x00730870
// FUNCTION: fn_07543
bool CWindEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00730880
// FUNCTION: fn_07544
void CWindEffect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle == nullptr) {
      continue;
    }

    switch (particle->unknown_00A8) {
    case 0: {
      int alpha = particle->alpha + 1;
      if (alpha >= this->maxAlpha) {
        alpha = this->maxAlpha;
        particle->unknown_00A8 = 1;
      }
      particle->alpha = alpha;
      break;
    }
    case 1: {
      int alpha = particle->alpha - 1;
      if (alpha <= 0) {
        alpha = 0;
        particle->unknown_00A8 = 0;
      }
      particle->alpha = alpha;
      break;
    }
    }

    float& angle = *reinterpret_cast<float*>(&particle->unknown_00C4);
    float nextAngle = particle->unknown_00C0 + angle;
    if (nextAngle > 360.0f) {
      nextAngle -= 360.0f;
    }
    angle = nextAngle;

    const float cosine = static_cast<float>(std::cos(static_cast<double>(angle / 180.0f * 3.141592f)));
    const float sine = static_cast<float>(std::sin(static_cast<double>(angle / 180.0f * 3.141592f)));

    vector3d points[4] = {};
    const float scale = particle->scale;
    points[0].x += scale * cosine;
    points[0].y = scale * 0.0f - particle->unknown_0054;
    points[0].z += scale * sine;
    points[1].x = (cosine + 0.0f) * this->width + points[0].x;
    points[1].y = this->width + points[0].y;
    points[1].z = (sine + 0.0f) * this->width + points[0].z;

    matrix transform;
    transform.MakeXRotation(this->rotation.x / 180.0f * 3.141592f);
    transform.AppendZRotation(this->rotation.z / 180.0f * 3.141592f);
    transform.AppendTranslate(this->position);

    const vector3d bottom = TransformPoint(transform, points[0]);
    const vector3d top = TransformPoint(transform, points[1]);
    particle->vertices[1].x = top.x;
    particle->vertices[1].y = top.y;
    particle->vertices[1].z = top.z;
    particle->vertices[3].x = bottom.x;
    particle->vertices[3].y = bottom.y;
    particle->vertices[3].z = bottom.z;

    if (particle->unknown_000C != 0) {
      particle->vertices[0].x = this->lastTop.x;
      particle->vertices[0].y = this->lastTop.y;
      particle->vertices[0].z = this->lastTop.z;
      particle->vertices[2].x = this->lastBottom.x;
      particle->vertices[2].y = this->lastBottom.y;
      particle->vertices[2].z = this->lastBottom.z;
    }

    this->lastBottom = bottom;
    this->lastTop = top;
  }
}

// OFFSET: 0x00731030
// FUNCTION: fn_07545
void CWindEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    const int texPart = particle->unknown_000C;
    if (texPart <= 0) {
      continue;
    }

    const ParticleQuad quad = {true, 0.0f, 1.0f, static_cast<float>(texPart - 1) / 21.0f, static_cast<float>(texPart) / 21.0f, false, true};
    particle->Render(quad);
  }
}
