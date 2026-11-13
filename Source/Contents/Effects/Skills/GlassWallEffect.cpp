#include "Contents/Effects/Skills/GlassWallEffect.h"

#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/Particles/EZ2STREffect.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/World/World.h"
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
void CGlassWall::AddPanel(float angle, float scale, vector3d offset) {
  const int startFrame = this->effectLifetime - 6;
  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 1);
  float* params = &particle->animationParameter;
  particle->scale = scale;
  particle->speedParameter = 0.25f;
  particle->alpha = 0;
  particle->angle = angle;
  particle->offsetX = offset.x;
  particle->offsetY = offset.y;
  particle->offsetZ = offset.z;
  params[0] = 40.0f;
  params[1] = 30.0f;
  params[2] = static_cast<float>(startFrame);
  params[3] = 180.0f;
  params[4] = 50.0f;
  particle->SetTexture("effect\\ring_blue.tga");
  this->particles.push_back(particle);
}

// OFFSET: 0x00B44940
// FUNCTION: fn_18891
void CGlassWall::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B449B0
// FUNCTION: fn_18892
void CGlassWall::Open() {
  CRenderObject* actor = FindRenderObject(this->actor);
  int angle = 180;
  if (actor) {
    angle = static_cast<int>(180.0f - actor->render.angle);
  }
  if (angle < 0) {
    angle += 360;
  }

  const float depth = 2.6f;
  const float scale = 3.0f;

  this->AddPanel(static_cast<float>(angle + 90), scale, vector3d{0.0f, 0.0f, depth});
  this->AddPanel(static_cast<float>(angle + 90), scale, vector3d{0.0f, 0.0f, -depth});
  this->AddPanel(static_cast<float>(angle), depth, vector3d{scale, 0.0f, 0.0f});
  this->AddPanel(static_cast<float>(angle), depth, vector3d{-scale, 0.0f, 0.0f});

  this->effect = new CEZ2STRScreenEffect();
  this->effect->SetActor(this->actor);
  this->effect->SetSourceActor(this->sourceActor);
  {
    CEffect* effect = this->effect;
    effect->position[0] = this->position[0];
    effect->position[1] = this->position[1];
    effect->position[2] = this->position[2];
  }
  this->effect->SetId(this->id);
  this->effect->Open();
}

// OFFSET: 0x00B44BC0
// FUNCTION: fn_18893
bool CGlassWall::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B44BE0
// FUNCTION: fn_18894
void CGlassWall::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    const int frame = this->frame;
    float* params = &particle->animationParameter;
    const int maxAlpha = static_cast<int>(params[3]);
    float speed = particle->speedParameter;
    const float angle = speed + params[0];
    float fade = params[1];
    if (frame % static_cast<int>(params[4]) == 0) {
      speed *= -1.0f;
    }

    if (frame == static_cast<int>(params[2])) {
      fade = static_cast<float>(-(particle->alpha / (this->effectLifetime - frame)));
    }

    int alpha = static_cast<int>(static_cast<float>(particle->alpha) + fade);
    if (static_cast<float>(alpha) < 0.0f) {
      alpha = 0;
    }

    matrix translation;
    translation.MakeTranslate(particle->position.x + particle->offsetX, particle->position.y + particle->offsetY,
                              particle->position.z + particle->offsetZ);
    matrix rotation;
    rotation.MakeYRotation(GetRadian(static_cast<int>(particle->angle)));
    matrix result;
    result.Compose(rotation, translation);

    params[0] = angle;
    particle->alpha = alpha > maxAlpha ? maxAlpha : alpha;
    ++particle->elapsedFrames;
    params[1] = fade;
    particle->speedParameter = speed;
    particle->transform = result;
  }

  if (this->effect) {
    this->effect->OnProcess();
  }
}

// OFFSET: 0x00B44F70
// FUNCTION: fn_18895
void CGlassWall::Render() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    const float width = particle->scale;
    const float height = particle->animationParameter;
    const float top = -height;
    const float back = -width;
    particle->vertices[0].x = 0.0f;
    particle->vertices[0].y = top;
    particle->vertices[0].z = width;
    particle->vertices[1].x = 0.0f;
    particle->vertices[1].y = top;
    particle->vertices[1].z = back;
    particle->vertices[2].x = 0.0f;
    particle->vertices[2].y = 0.0f;
    particle->vertices[2].z = width;
    particle->vertices[3].x = 0.0f;
    particle->vertices[3].y = 0.0f;
    particle->vertices[3].z = back;
    particle->Render(true, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
  }

  if (this->effect) {
    this->effect->Render();
  }
}
