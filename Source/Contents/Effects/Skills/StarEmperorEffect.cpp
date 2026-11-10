#include "Contents/Effects/Skills/StarEmperorEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

// OFFSET: 0x0072BBE0
// FUNCTION: fn_07474
CStarEmperorEffect::CStarEmperorEffect() : CEffect(-1), sphere(), rings() {
}

// OFFSET: 0x0072BE20
// FUNCTION: fn_07476
void CStarEmperorEffect::BuildSphere() {
  for (int row = 0; row < 6;) {
    const float top = static_cast<float>(row) * 0.16666667f;
    const float topPitch = (static_cast<float>(row) * 30.0f - 90.0f) / 180.0f * 3.141592f;
    ++row;
    const float bottom = static_cast<float>(row) * 0.16666667f;
    const float bottomPitch = (static_cast<float>(row) * 30.0f - 90.0f) / 180.0f * 3.141592f;

    for (int column = 0; column < 12;) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 1);
      particle->SetTexture("effect\\star_emperor\\boom.tga");

      float leftYaw = static_cast<float>(column);
      const float left = leftYaw * 0.083333336f;
      particle->vertices[0].u = left < 0.0f ? 0.0f : (left > 1.0f ? 1.0f : left);
      particle->vertices[0].v = top < 0.0f ? 0.0f : (top > 1.0f ? 1.0f : top);
      ++column;
      float rightYaw = static_cast<float>(column);
      const float right = rightYaw * 0.083333336f;
      particle->vertices[1].u = right < 0.0f ? 0.0f : (right > 1.0f ? 1.0f : right);
      particle->vertices[1].v = top < 0.0f ? 0.0f : (top > 1.0f ? 1.0f : top);
      particle->vertices[2].u = left < 0.0f ? 0.0f : (left > 1.0f ? 1.0f : left);
      particle->vertices[2].v = bottom < 0.0f ? 0.0f : (bottom > 1.0f ? 1.0f : bottom);
      particle->vertices[3].u = right < 0.0f ? 0.0f : (right > 1.0f ? 1.0f : right);
      particle->vertices[3].v = bottom < 0.0f ? 0.0f : (bottom > 1.0f ? 1.0f : bottom);

      matrix corners[4];
      corners[0].MakeXRotation(topPitch);
      leftYaw = leftYaw * -30.0f / 180.0f * 3.141592f;
      corners[0].AppendYRotation(leftYaw);
      corners[1].MakeXRotation(topPitch);
      rightYaw = rightYaw * -30.0f / 180.0f * 3.141592f;
      corners[1].AppendYRotation(rightYaw);
      corners[2].MakeXRotation(bottomPitch);
      corners[2].AppendYRotation(leftYaw);
      corners[3].MakeXRotation(bottomPitch);
      corners[3].AppendYRotation(rightYaw);

      for (int i = 0; i < 4; ++i) {
        const float* m = corners[i].values;
        particle->vertices[i].x = 0.0f * m[0] + 0.0f * m[3] - 1.0f * m[6] + m[9];
        particle->vertices[i].y = 0.0f * m[1] + 0.0f * m[4] - 1.0f * m[7] + m[10];
        particle->vertices[i].z = 0.0f * m[2] + 0.0f * m[5] - 1.0f * m[8] + m[11];
      }

      this->sphere.push_back(particle);
    }
  }
}

// OFFSET: 0x0072C420
// FUNCTION: fn_07477
void CStarEmperorEffect::Close() {
  for (auto it = this->sphere.begin(); it != this->sphere.end(); ++it) {
    FreeParticle(*it);
  }

  this->sphere.clear();

  for (auto it = this->rings.begin(); it != this->rings.end(); ++it) {
    FreeParticle(*it);
  }

  this->rings.clear();
}

// OFFSET: 0x0072C490
// FUNCTION: fn_07478
void CStarEmperorEffect::Open() {
  this->effectLifetime = this->id != 1203 ? 30 : 60;
  this->BuildSphere();

  for (int i = 0; i < 10; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 0x201);
    particle->effectId = this->id;
    particle->columnIndex = i;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->SetTexture("effect\\star_emperor\\ring_white.tga");
    particle->phaseParameter = 36;
    this->rings.push_back(particle);
  }
}

// OFFSET: 0x0072C540
// FUNCTION: fn_07479
void CStarEmperorEffect::InitEvents() {
  if (this->id == 1203) {
    this->AddEvent(0, "effect\\wizard_fire_pillar_a.wav", 1, 0);
    this->AddEvent(0, "effect\\wizard_fire_pillar_b.wav", 31, 0);
  }
}

// OFFSET: 0x0072C580
// FUNCTION: fn_07480
bool CStarEmperorEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x0072C590
// FUNCTION: fn_07481
void CStarEmperorEffect::UpdateRing(CParticle* particle) {
  if (!particle) {
    return;
  }

  const int column = particle->columnIndex;
  const int startAngle = particle->phaseParameter * column;
  const int endAngle = (column + 1) * particle->phaseParameter;
  int step = particle->elapsedFrames + 1;
  if (this->id == 1203) {
    const int half = this->effectLifetime / 2;
    step = half > particle->elapsedFrames ? half - particle->elapsedFrames : particle->elapsedFrames - half;
  }

  float inner = 0.0f;
  const float acceleration = -(1.75f / static_cast<float>(this->effectLifetime));
  const float outer = acceleration * g_depthAverageHalf * static_cast<float>((step + 1) * step / 2) + static_cast<float>(step) * 1.75f;
  if (outer > 12.0f) {
    inner = outer - 12.0f;
  }

  EffectPoint corners[4];
  const float outerStartX = g_cosTable[startAngle] * outer;
  const float outerStartY = g_sinTable[startAngle] * outer;
  const float outerEndX = g_cosTable[endAngle] * outer;
  const float outerEndY = g_sinTable[endAngle] * outer;
  const float innerStartX = g_cosTable[startAngle] * inner;
  const float innerStartY = g_sinTable[startAngle] * inner;
  const float innerEndX = g_cosTable[endAngle] * inner;
  const float innerEndY = g_sinTable[endAngle] * inner;

  particle->vertices[0].x = outerStartX;
  particle->vertices[0].y = outerStartY;
  particle->vertices[0].z = 0.0f;
  particle->vertices[1].x = outerEndX;
  particle->vertices[1].y = outerEndY;
  particle->vertices[1].z = 0.0f;
  particle->vertices[2].x = innerStartX;
  particle->vertices[2].y = innerStartY;
  particle->vertices[2].z = 0.0f;
  particle->vertices[3].x = innerEndX;
  particle->vertices[3].y = innerEndY;
  particle->vertices[3].z = 0.0f;

  corners[0].x = outerStartX;
  corners[0].y = outerStartY;
  corners[0].z = 0.0f;
  corners[1].x = outerEndX;
  corners[1].y = outerEndY;
  corners[1].z = 0.0f;
  corners[2].x = innerStartX;
  corners[2].y = innerStartY;
  corners[2].z = 0.0f;
  corners[3].x = innerEndX;
  corners[3].y = innerEndY;
  corners[3].z = 0.0f;

  matrix rotation;
  rotation.MakeXRotation(1.570796f);
  particle->transform = rotation;

  if (this->id == 1203) {
    const int quarter = this->effectLifetime / 4;
    if (quarter > step) {
      particle->alpha = step * (255 / quarter);
    } else {
      particle->alpha = (this->effectLifetime - step) * (255 / quarter);
    }
  } else {
    const int half = this->effectLifetime / 2;
    if (half > particle->elapsedFrames) {
      particle->alpha = step * (255 / half);
    } else {
      particle->alpha = (this->effectLifetime - step) * (255 / half);
    }
  }

  particle->position.x = this->position[0];
  particle->position.y = this->position[1];
  particle->position.z = this->position[2];
}

// OFFSET: 0x0072C830
// FUNCTION: fn_07482
void CStarEmperorEffect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->sphere.begin(); it != this->sphere.end(); ++it) {
    if (CParticle* particle = *it) {
      const int frame = particle->elapsedFrames;
      int step = frame + 1;
      if (this->id == 1203) {
        const int half = this->effectLifetime / 2;
        step = half > frame ? half - frame : frame - half;
      }

      const float size = -(1.15f / static_cast<float>(this->effectLifetime)) * g_depthAverageHalf * static_cast<float>((step + 1) * step / 2) +
                         static_cast<float>(step) * 1.15f;
      matrix scale;
      scale.SetScale(size, size, size);
      matrix rotation;
      rotation.MakeYRotation(static_cast<float>(step) * 3.0f / 180.0f * 3.141592f);
      matrix result;
      result.Compose(scale, rotation);
      particle->transform = result;

      if (this->id == 1203) {
        const int quarter = this->effectLifetime / 4;
        if (quarter <= step) {
          step = this->effectLifetime - step;
        }
        particle->alpha = 255 / quarter * step;
      } else {
        const int half = this->effectLifetime / 2;
        const int fade = 255 / half;
        if (half > particle->elapsedFrames) {
          particle->alpha = step * fade;
        } else {
          particle->alpha = (this->effectLifetime - step) * fade;
        }
      }

      particle->position.x = this->position[0];
      particle->position.y = this->position[1];
      particle->position.z = this->position[2];
    }

    ++(*it)->elapsedFrames;
  }

  for (std::vector<CParticle*>::iterator it = this->rings.begin(); it != this->rings.end(); ++it) {
    this->UpdateRing(*it);
    ++(*it)->elapsedFrames;
  }
}

// OFFSET: 0x0072CCA0
// FUNCTION: fn_07483
void CStarEmperorEffect::Render() {
  for (auto it = this->sphere.begin(); it != this->sphere.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      particle->Render(false, particle->vertices[0].v, particle->vertices[3].v, particle->vertices[0].u, particle->vertices[3].u, false, true);
    }
  }

  for (auto it = this->rings.begin(); it != this->rings.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 0.25f, false, true);
    }
  }
}
