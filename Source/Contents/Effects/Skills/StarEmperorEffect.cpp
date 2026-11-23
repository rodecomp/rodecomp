#include "Contents/Effects/Skills/StarEmperorEffect.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectPoint.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

// INLINED
static void SetParticleIndices(CParticle& particle, int group, int column, int row) {
  particle.columnIndex = column;
  particle.rowIndex = row;
  particle.groupIndex = group;
}

// OFFSET: 0x0072BBE0
// FUNCTION: fn_07474
CStarEmperorEffect::CStarEmperorEffect() : CEffect(-1), sphere(), rings() {
}

// OFFSET: 0x0072BC80
// FUNCTION: fn_46128
__declspec(noinline) CStarEmperorEffect::~CStarEmperorEffect() {
}

// INLINED
static vector3d TransformPoint(const matrix& transform, const vector3d& point) {
  vector3d result;
  result.x = point.x * transform.values[0] + point.y * transform.values[3] + point.z * transform.values[6] + transform.values[9];
  result.y = point.x * transform.values[1] + point.y * transform.values[4] + point.z * transform.values[7] + transform.values[10];
  result.z = point.x * transform.values[2] + point.y * transform.values[5] + point.z * transform.values[8] + transform.values[11];
  return result;
}

// OFFSET: 0x0072BD80
// FUNCTION: fn_46129
__forceinline void CStarEmperorEffect::BuildRings() {
  for (int i = 0; i < 10; ++i) {
    CParticle* particle = CreateParticle(&this->positionVector, 0x201);
    particle->effectId = this->id;
    SetParticleIndices(*particle, 1, i, 0);
    particle->SetTexture("effect\\star_emperor\\ring_white.tga");
    particle->phaseParameter = 36;
    this->rings.push_back(particle);
  }
}

// OFFSET: 0x0072BE20
// FUNCTION: fn_07476
void CStarEmperorEffect::BuildSphere() {
  for (int row = 0; row < 6; ++row) {
    const float top = static_cast<float>(row) * 0.16666667f;
    const float bottom = static_cast<float>(row + 1) * 0.16666667f;
    const float topPitch = (static_cast<float>(row) * 30.0f - 90.0f) / 180.0f * 3.141592f;
    const float bottomPitch = (static_cast<float>(row + 1) * 30.0f - 90.0f) / 180.0f * 3.141592f;

    for (int column = 0; column < 12; ++column) {
      CParticle* particle = CreateParticle(&this->positionVector, 1);
      particle->SetTexture("effect\\star_emperor\\boom.tga");

      float leftYaw = static_cast<float>(column);
      const float left = leftYaw * 0.083333336f;
      particle->vertices[0].u = left < 0.0f ? 0.0f : (left > 1.0f ? 1.0f : left);
      particle->vertices[0].v = top < 0.0f ? 0.0f : (top > 1.0f ? 1.0f : top);
      float rightYaw = static_cast<float>(column + 1);
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

      const vector3d point = {0.0f, 0.0f, -1.0f};
      for (int i = 0; i < 4; ++i) {
        const vector3d position = TransformPoint(corners[i], point);
        particle->vertices[i].x = position.x;
        particle->vertices[i].y = position.y;
        particle->vertices[i].z = position.z;
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
  this->effectLifetime = this->id != EF_STAR_EMPEROR ? 30 : 60;
  this->BuildSphere();

  this->BuildRings();
}

// OFFSET: 0x0072C540
// FUNCTION: fn_07479
void CStarEmperorEffect::InitEvents() {
  if (this->id == EF_STAR_EMPEROR) {
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
  if (this->id == EF_STAR_EMPEROR) {
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

  if (this->id == EF_STAR_EMPEROR) {
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

  particle->position = vector3d{this->position[0], this->position[1], this->position[2]};
}

// OFFSET: 0x0072C830
// FUNCTION: fn_07482
void CStarEmperorEffect::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->sphere.begin(); it != this->sphere.end(); ++it) {
    this->UpdateSphereParticle(*it);

    ++(*it)->elapsedFrames;
  }

  for (std::vector<CParticle*>::iterator it = this->rings.begin(); it != this->rings.end(); ++it) {
    this->UpdateRing(*it);
    ++(*it)->elapsedFrames;
  }
}

// OFFSET: 0x0072CAA0
// FUNCTION: fn_46130
__forceinline void CStarEmperorEffect::UpdateSphereParticle(CParticle* particle) {
  if (particle) {
    const int frame = particle->elapsedFrames;
    int step = frame + 1;
    if (this->id == EF_STAR_EMPEROR) {
      const int half = this->effectLifetime / 2;
      step = half > frame ? half - frame : frame - half;
    }

    const float acceleration = -(1.15f / static_cast<float>(this->effectLifetime));
    const float size = acceleration * 0.5f * static_cast<float>((step + 1) * step / 2) + static_cast<float>(step) * 1.15f;
    matrix scale;
    scale.SetScale(size, size, size);
    matrix rotation;
    rotation.MakeYRotation(static_cast<float>(step) * 3.0f / 180.0f * 3.141592f);
    matrix result;
    result.Compose(scale, rotation);
    particle->transform = result;

    if (this->id == EF_STAR_EMPEROR) {
      const int quarter = this->effectLifetime / 4;
      particle->alpha = (quarter > step ? step : this->effectLifetime - step) * (255 / quarter);
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
}

// OFFSET: 0x0072CC60
// FUNCTION: fn_46131
__forceinline void CStarEmperorEffect::RenderRing(CParticle* particle) {
  if (particle) {
    particle->Render(false, 0.0f, 1.0f, 0.0f, 0.25f, false, true);
  }
}

// OFFSET: 0x0072CCA0
// FUNCTION: fn_07483
void CStarEmperorEffect::Render() {
  for (auto it = this->sphere.begin(); it != this->sphere.end(); ++it) {
    this->RenderSphereParticle(*it);
  }

  for (auto it = this->rings.begin(); it != this->rings.end(); ++it) {
    this->RenderRing(*it);
  }
}

// OFFSET: 0x0072CD60
// FUNCTION: fn_46132
__forceinline void CStarEmperorEffect::RenderSphereParticle(CParticle* particle) {
  if (particle) {
    particle->Render(false, particle->vertices[0].v, particle->vertices[3].v, particle->vertices[0].u, particle->vertices[3].u, false, true);
  }
}
