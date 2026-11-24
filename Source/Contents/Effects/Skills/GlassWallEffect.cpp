#include "Contents/Effects/Skills/GlassWallEffect.h"

#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/Particles/EZ2STREffect.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00B445D0
// FUNCTION: fn_46008
CGlassWall::CGlassWall() : CEffect(EF_GLASSWALL), effect(nullptr) {
  this->effectLifetime = 9999;
}

// OFFSET: 0x00B44980
// FUNCTION: fn_45941
__forceinline void CGlassWall::CloseScreenEffect() {
  if (this->effect) {
    this->effect->Close();
    delete this->effect;
    this->effect = nullptr;
  }
}

// OFFSET: 0x00B44660
// FUNCTION: fn_46009
__declspec(noinline) CGlassWall::~CGlassWall() {
  this->CloseScreenEffect();
}

// OFFSET: 0x00B44860
// FUNCTION: fn_18890
void CGlassWall::AddPanel(float angle, float scale, vector3d offset) {
  const int startFrame = this->effectLifetime - 6;
  CParticle* particle = CreateParticle(&this->positionVector, 1);
  particle->scale = scale;
  particle->speedParameter = 0.25f;
  particle->alpha = 0;
  particle->angle = angle;
  particle->offsetX = offset.x;
  particle->offsetY = offset.y;
  particle->offsetZ = offset.z;
  particle->animationParameter = 40.0f;
  particle->animationParameters[0] = 30.0f;
  particle->animationParameters[1] = static_cast<float>(startFrame);
  particle->animationParameters[2] = 180.0f;
  particle->animationParameters[3] = 50.0f;
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

// OFFSET: 0x00B447A0
// FUNCTION: fn_45940
__forceinline void CGlassWall::OpenScreenEffect() {
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

  this->OpenScreenEffect();
}

// OFFSET: 0x00B44BC0
// FUNCTION: fn_18893
bool CGlassWall::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B44BD0
// FUNCTION: fn_45942
__forceinline void CGlassWall::UpdateScreenEffect() {
  if (this->effect) {
    this->effect->OnProcess();
  }
}

// OFFSET: 0x00B44DD0
// FUNCTION: fn_45943
__forceinline void CGlassWall::UpdatePanel(CParticle* particle) {
  const int maxAlpha = static_cast<int>(particle->animationValues[3]);
  const int previousAlpha = particle->alpha;
  const int frame = this->frame;
  float fade = particle->animationValues[1];
  float speed = particle->speedParameter;
  const float angle = speed + particle->animationValues[0];
  if (frame % static_cast<int>(particle->animationValues[4]) == 0) {
    speed *= g_heightSpeedSign;
  }

  if (frame == static_cast<int>(particle->animationValues[2])) {
    fade = static_cast<float>(-(previousAlpha / (this->effectLifetime - frame)));
  }

  int alpha = static_cast<int>(static_cast<float>(previousAlpha) + fade);
  if (static_cast<float>(alpha) < 0.0f) {
    alpha = 0;
  }

  matrix translation;
  const vector3d position = {particle->position.x + particle->offsetX, particle->position.y + particle->offsetY,
                             particle->position.z + particle->offsetZ};
  translation.MakeTranslate(position.x, position.y, position.z);
  matrix rotation;
  rotation.MakeYRotation(GetRadian(static_cast<int>(particle->angle)));
  matrix result;
  result.Compose(rotation, translation);

  particle->animationValues[0] = angle;
  particle->animationValues[1] = fade;
  particle->speedParameter = speed;
  particle->alpha = alpha <= maxAlpha ? alpha : maxAlpha;
  particle->transform = result;
}

// OFFSET: 0x00B44BE0
// FUNCTION: fn_18894
void CGlassWall::OnProcess() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    this->UpdatePanel(particle);
    ++particle->elapsedFrames;
  }

  this->UpdateScreenEffect();
}

// OFFSET: 0x00B45070
// FUNCTION: fn_45944
__forceinline void CGlassWall::RenderScreenEffect() {
  if (this->effect) {
    this->effect->Render();
  }
}

// OFFSET: 0x00B45080
// FUNCTION: fn_45945
__forceinline void CGlassWall::RenderPanel(CParticle* particle) {
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

// OFFSET: 0x00B44F70
// FUNCTION: fn_18895
void CGlassWall::Render() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->RenderPanel(*it);
  }

  this->RenderScreenEffect();
}
