#include "Contents/Effects/Skills/WindEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

// INLINED
static EffectPoint TransformPoint(const matrix& transform, const EffectPoint& point) {
  EffectPoint result;
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
// FUNCTION: fn_07539
void CWindEffect::Init() {
  const float step = static_cast<float>(this->arcLength) / 20.0f;
  for (int ring = 0; ring < this->ringCount; ++ring) {
    const int startAngle = std::ranges::max(rand() % 360, 0);
    const int maxAlpha = this->maxAlpha;
    const int alpha = maxAlpha == 0 ? 0 : std::ranges::max(rand() % maxAlpha, 0);
    for (int segment = 0; segment < 21; ++segment) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (!particle) {
        continue;
      }
      particle->alpha = alpha;
      particle->SetColor(this->red, this->green, this->blue);
      particle->scale = this->particleScale;
      particle->animationParameter = 1 < this->ringCount ? this->height / static_cast<float>(this->ringCount - 1) * static_cast<float>(ring) : 0.0f;
      particle->speedParameter = this->angleSpeed;
      particle->angleParameter = this->arcLength;
      if (this->sourceBlend != -1) {
        particle->sourceBlend = this->sourceBlend;
      }
      if (this->destinationBlend != -1) {
        particle->destinationBlend = this->destinationBlend;
      }
      particle->SetTexture(this->textureName);
      particle->angle = static_cast<float>(segment) * step + static_cast<float>(startAngle);
      particle->animationState = 0;
      particle->InitParticleQuad(particle->vertices);
      particle->effectId = this->id;
      particle->columnIndex = segment;
      particle->rowIndex = ring;
      particle->groupIndex = 1;
      this->particles.push_back(particle);
    }
  }
}

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
void CWindEffect::Open() {
  this->Init();
}

// OFFSET: 0x00730860
// FUNCTION: fn_07542
void CWindEffect::ResetParticles() {
  this->Close();
  this->Init();
}

// OFFSET: 0x00730870
// FUNCTION: fn_07543
bool CWindEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00730880
// FUNCTION: fn_07544
void CWindEffect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle == nullptr) {
      continue;
    }

    switch (particle->animationState) {
    case 0: {
      int alpha = particle->alpha + 1;
      if (this->maxAlpha <= alpha) {
        alpha = this->maxAlpha;
        particle->animationState = 1;
      }
      particle->alpha = alpha;
      break;
    }
    case 1: {
      int alpha = particle->alpha - 1;
      if (alpha <= 0) {
        alpha = 0;
        particle->animationState = 0;
      }
      particle->alpha = alpha;
      break;
    }
    }

    float& angle = particle->angle;
    float nextAngle = particle->speedParameter + angle;
    if (nextAngle > 360.0f) {
      nextAngle -= 360.0f;
    }
    angle = nextAngle;

    const float cosine = ClientCos(angle / 180.0f * 3.141592f);
    const float sine = ClientSin(angle / 180.0f * 3.141592f);

    EffectPoint points[4];
    const float scale = particle->scale;
    points[0].x += scale * cosine;
    points[0].y = scale * 0.0f - particle->animationParameter;
    points[0].z += scale * sine;
    points[1].x = (cosine + 0.0f) * this->width + points[0].x;
    points[1].y = this->width + points[0].y;
    points[1].z = (sine + 0.0f) * this->width + points[0].z;

    matrix transform;
    transform.MakeXRotation(this->rotation.x / 180.0f * 3.141592f);
    transform.AppendZRotation(this->rotation.z / 180.0f * 3.141592f);
    transform.AppendTranslate(this->position);

    const EffectPoint bottom = TransformPoint(transform, points[0]);
    const EffectPoint top = TransformPoint(transform, points[1]);
    particle->vertices[1].x = top.x;
    particle->vertices[1].y = top.y;
    particle->vertices[1].z = top.z;
    particle->vertices[3].x = bottom.x;
    particle->vertices[3].y = bottom.y;
    particle->vertices[3].z = bottom.z;

    if (particle->columnIndex != 0) {
      particle->vertices[0].x = this->lastPoints[1].x;
      particle->vertices[0].y = this->lastPoints[1].y;
      particle->vertices[0].z = this->lastPoints[1].z;
      particle->vertices[2].x = this->lastPoints[0].x;
      particle->vertices[2].y = this->lastPoints[0].y;
      particle->vertices[2].z = this->lastPoints[0].z;
    }

    this->lastPoints[0] = bottom;
    this->lastPoints[1] = top;
  }
}

// OFFSET: 0x00731030
// FUNCTION: fn_07545
void CWindEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    const int texPart = particle->columnIndex;
    if (texPart <= 0) {
      continue;
    }

    ParticleQuad quad;
    quad.unused = false;
    quad.depth = true;
    quad.top = 0.0f;
    quad.bottom = 1.0f;
    quad.billboard = true;
    quad.left = static_cast<float>(texPart - 1) / 21.0f;
    quad.right = static_cast<float>(texPart) / 21.0f;
    particle->Render(quad);
  }
}
