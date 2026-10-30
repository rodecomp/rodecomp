#include "Contents/Effects/StarEmperorEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/MapSkyMgr.h"
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
  for (CParticle* particle : this->sphere) {
    FreeParticle(particle);
  }

  this->sphere.clear();

  for (CParticle* particle : this->rings) {
    FreeParticle(particle);
  }

  this->rings.clear();
}

// OFFSET: 0x0072C490
// FUNCTION: fn_07478
void CStarEmperorEffect::Slot01() {
  this->unknown_002C = this->id == 1203 ? 60 : 30;
  this->BuildSphere();

  for (int i = 0; i < 10; ++i) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 0x201);
    particle->unknown_0004 = this->id;
    particle->unknown_000C = i;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->SetTexture("effect\\star_emperor\\ring_white.tga");
    particle->unknown_004C = 36;
    this->rings.push_back(particle);
  }
}

// OFFSET: 0x0072C540
// FUNCTION: fn_07479
void CStarEmperorEffect::Slot12() {
  if (this->id == 1203) {
    this->AddEvent(0, "effect\\wizard_fire_pillar_a.wav", 1, 0);
    this->AddEvent(0, "effect\\wizard_fire_pillar_b.wav", 31, 0);
  }
}

// OFFSET: 0x0072C580
// FUNCTION: fn_07480
bool CStarEmperorEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x0072C590
// FUNCTION: fn_07481
void CStarEmperorEffect::UpdateRing(CParticle* particle) {
  if (!particle) {
    return;
  }

  const int startAngle = particle->unknown_004C * particle->unknown_000C;
  const int endAngle = (particle->unknown_000C + 1) * particle->unknown_004C;
  int step = particle->unknown_00E8 + 1;
  if (this->id == 1203) {
    const int half = this->unknown_002C / 2;
    step = half > particle->unknown_00E8 ? half - particle->unknown_00E8 : particle->unknown_00E8 - half;
  }

  const float outer = -(1.75f / static_cast<float>(this->unknown_002C)) * g_depthAverageHalf * static_cast<float>((step + 1) * step / 2) +
                      static_cast<float>(step) * 1.75f;
  float inner = 0.0f;
  if (outer > 12.0f) {
    inner = outer - 12.0f;
  }

  vector3d corners[4] = {};
  corners[0].x = g_cosTable[startAngle] * outer;
  corners[0].y = g_sinTable[startAngle] * outer;
  corners[0].z = 0.0f;
  corners[1].x = g_cosTable[endAngle] * outer;
  corners[1].y = g_sinTable[endAngle] * outer;
  corners[1].z = 0.0f;
  corners[2].x = g_cosTable[startAngle] * inner;
  corners[2].y = g_sinTable[startAngle] * inner;
  corners[2].z = 0.0f;
  corners[3].x = g_cosTable[endAngle] * inner;
  corners[3].y = g_sinTable[endAngle] * inner;
  corners[3].z = 0.0f;

  for (int i = 0; i < 4; ++i) {
    particle->vertices[i].x = corners[i].x;
    particle->vertices[i].y = corners[i].y;
    particle->vertices[i].z = corners[i].z;
  }

  matrix rotation;
  rotation.MakeXRotation(1.570796f);
  particle->transform = rotation;

  if (this->id == 1203) {
    const int quarter = this->unknown_002C / 4;
    if (quarter <= step) {
      step = this->unknown_002C - step;
    }
    particle->alpha = 255 / quarter * step;
  } else {
    const int half = this->unknown_002C / 2;
    const int scale = 255 / half;
    if (half > particle->unknown_00E8) {
      particle->alpha = step * scale;
    } else {
      particle->alpha = (this->unknown_002C - step) * scale;
    }
  }

  particle->position.x = this->position[0];
  particle->position.y = this->position[1];
  particle->position.z = this->position[2];
}

// OFFSET: 0x0072C830
// FUNCTION: fn_07482
void CStarEmperorEffect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->sphere.begin(); it != this->sphere.end(); ++it) {
    if (CParticle* particle = *it) {
      const int frame = particle->unknown_00E8;
      int step = frame + 1;
      if (this->id == 1203) {
        const int half = this->unknown_002C / 2;
        step = half > frame ? half - frame : frame - half;
      }

      const float size = -(1.15f / static_cast<float>(this->unknown_002C)) * g_depthAverageHalf * static_cast<float>((step + 1) * step / 2) +
                         static_cast<float>(step) * 1.15f;
      matrix scale;
      scale.SetScale(size, size, size);
      matrix rotation;
      rotation.MakeYRotation(static_cast<float>(step) * 3.0f / 180.0f * 3.141592f);
      matrix result;
      result.Compose(scale, rotation);
      particle->transform = result;

      if (this->id == 1203) {
        const int quarter = this->unknown_002C / 4;
        if (quarter <= step) {
          step = this->unknown_002C - step;
        }
        particle->alpha = 255 / quarter * step;
      } else {
        const int half = this->unknown_002C / 2;
        const int fade = 255 / half;
        if (half > particle->unknown_00E8) {
          particle->alpha = step * fade;
        } else {
          particle->alpha = (this->unknown_002C - step) * fade;
        }
      }

      particle->position.x = this->position[0];
      particle->position.y = this->position[1];
      particle->position.z = this->position[2];
    }

    ++(*it)->unknown_00E8;
  }

  for (std::vector<CParticle*>::iterator it = this->rings.begin(); it != this->rings.end(); ++it) {
    this->UpdateRing(*it);
    ++(*it)->unknown_00E8;
  }
}

// OFFSET: 0x0072CCA0
// FUNCTION: fn_07483
void CStarEmperorEffect::Slot04() {
  for (CParticle* particle : this->sphere) {
    if (particle) {
      particle->Render(false, particle->vertices[0].v, particle->vertices[3].v, particle->vertices[0].u, particle->vertices[3].u, false, true);
    }
  }

  for (CParticle* particle : this->rings) {
    if (particle) {
      particle->Render(false, 0.0f, 1.0f, 0.0f, 0.25f, false, true);
    }
  }
}
