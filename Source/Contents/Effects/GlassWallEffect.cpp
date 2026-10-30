#include "Contents/Effects/GlassWallEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

CGlassWall::~CGlassWall() {
  if (this->effect) {
    this->effect->Close();
    delete this->effect;
    this->effect = nullptr;
  }
}

// OFFSET: 0x00B44860
// FUNCTION: fn_18890
void CGlassWall::AddPanel(float angle, float scale, float x, float y, float z) {
  const int lastFrame = this->unknown_002C;
  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 1);
  float* params = &particle->unknown_0054;
  particle->scale = scale;
  particle->unknown_00C0 = 0.25f;
  particle->alpha = 0;
  *reinterpret_cast<float*>(&particle->unknown_00C4) = angle;
  particle->unknown_00D0 = x;
  particle->unknown_00D4 = y;
  particle->unknown_00D8 = z;
  params[0] = 40.0f;
  params[1] = 30.0f;
  params[2] = static_cast<float>(lastFrame - 6);
  params[3] = 180.0f;
  params[4] = 50.0f;
  particle->SetTexture("effect\\ring_blue.tga");
  this->particles.push_back(particle);
}

// OFFSET: 0x00B44940
// FUNCTION: fn_18891
void CGlassWall::Close() {
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B44BC0
// FUNCTION: fn_18893
bool CGlassWall::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B44BE0
// FUNCTION: fn_18894
void CGlassWall::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    const int frame = this->frame;
    float* params = &particle->unknown_0054;
    const int maxAlpha = static_cast<int>(params[3]);
    float speed = particle->unknown_00C0;
    const float angle = speed + params[0];
    float fade = params[1];
    if (frame % static_cast<int>(params[4]) == 0) {
      speed *= -1.0f;
    }

    if (frame == static_cast<int>(params[2])) {
      fade = static_cast<float>(-(particle->alpha / (this->unknown_002C - frame)));
    }

    int alpha = static_cast<int>(static_cast<float>(particle->alpha) + fade);
    if (static_cast<float>(alpha) < 0.0f) {
      alpha = 0;
    }

    matrix translation;
    translation.MakeTranslate(particle->position.x + particle->unknown_00D0, particle->position.y + particle->unknown_00D4,
                              particle->position.z + particle->unknown_00D8);
    matrix rotation;
    rotation.MakeYRotation(GetRadian(static_cast<int>(*reinterpret_cast<float*>(&particle->unknown_00C4))));
    matrix result;
    result.Compose(rotation, translation);

    params[0] = angle;
    particle->alpha = alpha > maxAlpha ? maxAlpha : alpha;
    ++particle->unknown_00E8;
    params[1] = fade;
    particle->unknown_00C0 = speed;
    particle->transform = result;
  }

  if (this->effect) {
    this->effect->Slot03();
  }
}

// OFFSET: 0x00B44F70
// FUNCTION: fn_18895
void CGlassWall::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    const float width = particle->scale;
    const float height = particle->unknown_0054;
    particle->vertices[0].x = 0.0f;
    particle->vertices[0].y = -height;
    particle->vertices[0].z = width;
    particle->vertices[1].x = 0.0f;
    particle->vertices[1].y = -height;
    particle->vertices[1].z = -width;
    particle->vertices[2].x = 0.0f;
    particle->vertices[2].y = 0.0f;
    particle->vertices[2].z = width;
    particle->vertices[3].x = 0.0f;
    particle->vertices[3].y = 0.0f;
    particle->vertices[3].z = -width;
    particle->Render(true, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }

  if (this->effect) {
    this->effect->Slot04();
  }
}
