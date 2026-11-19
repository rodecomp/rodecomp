#include "Contents/Effects/Skills/SoulUnityEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/World/World.h"
#include "Render/Textures/Texture.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x0072B050
// FUNCTION: fn_07465
CSoulUnityEffect::CSoulUnityEffect() : CEffect(1222) {
}

// OFFSET: 0x0072B0E0
// FUNCTION: fn_07466
__declspec(noinline) CSoulUnityEffect::~CSoulUnityEffect() {
}

// OFFSET: 0x0072B230
// FUNCTION: fn_07468
void CSoulUnityEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
}

// OFFSET: 0x0072B2A0
// FUNCTION: fn_07469
void CSoulUnityEffect::Open() {
  CParticle* particle = CreateParticle(&this->positionVector, 0);
  if (particle) {
    particle->SetTexture("effect\\soul_unity\\soul_unity.bmp");
    particle->effectId = this->id;
    particle->columnIndex = 1;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->scale = 2.5f;
    particle->depthOffset = 0.035f;
    particle->InitParticleQuad(particle->vertices);
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x0072B3B0
// FUNCTION: fn_07471
bool CSoulUnityEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x0072B3C0
// FUNCTION: fn_07472
void CSoulUnityEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->UpdateParticle(*it);
  }
}

// OFFSET: 0x0072B680
// FUNCTION: fn_46207
__forceinline void CSoulUnityEffect::UpdateParticle(CParticle* particle) {
  if (!particle) {
    return;
  }

  float height = -15.0f;
  CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
  if (actor && actor->isRiding) {
    height = -20.0f;
  }

  matrix rotation;
  rotation.MakeXRotation(g_modeMgr.GetGameMode()->camera->camera.pitch / 180.0f * 3.141592f);
  rotation.AppendYRotation(g_modeMgr.GetGameMode()->camera->camera.yaw / 180.0f * 3.141592f);

  const float x = 0.0f;
  const float y = height;
  const float z = 0.0f;
  vector3d rotated;
  rotated.x = y * rotation.values[3] + x * rotation.values[0] + z * rotation.values[6] + rotation.values[9];
  rotated.y = y * rotation.values[4] + x * rotation.values[1] + z * rotation.values[7] + rotation.values[10];
  rotated.z = x * rotation.values[2] + y * rotation.values[5] + z * rotation.values[8] + rotation.values[11];

  matrix scaling;
  scaling.SetScale(particle->scale, particle->scale, 1.0f);
  matrix billboard;
  billboard.MakeXRotation(g_modeMgr.GetGameMode()->camera->camera.pitch / 180.0f * 3.141592f);
  billboard.AppendYRotation(g_modeMgr.GetGameMode()->camera->camera.yaw / 180.0f * 3.141592f);
  vector3d world;
  world.x = rotated.x + this->position[0];
  world.y = rotated.y + this->position[1];
  world.z = rotated.z + this->position[2];
  matrix translation;
  translation.MakeTranslate(world.x, world.y, world.z);
  matrix first;
  first.Compose(scaling, billboard);
  matrix result;
  result.Compose(first, translation);
  particle->transform = result;
}

// OFFSET: 0x0072B920
// FUNCTION: fn_07473
void CSoulUnityEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    this->RenderParticle(*it);
  }
}

// OFFSET: 0x0072BA90
// FUNCTION: fn_46208
__forceinline void __stdcall CSoulUnityEffect::RenderParticle(CParticle* particle) {
  if (!particle) {
    return;
  }

  if (!particle->texture) {
    return;
  }

  const float left = g_textureOffsetX / particle->texture->TextureWidth();
  const float width = static_cast<float>(particle->texture->Width());
  const float right = (width + g_textureOffsetX) / particle->texture->TextureWidth();
  const float top = g_textureOffsetY / particle->texture->TextureHeight();
  const float bottom = (particle->texture->Height() + g_textureOffsetY) / particle->texture->TextureHeight();
  particle->Render(true, bottom, top, left, right, false, true);
}
