#include "Contents/Effects/SoulUnityEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/World/World.h"
#include "Render/Textures/Texture.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x0072B050
// FUNCTION: fn_07465
CSoulUnityEffect::CSoulUnityEffect() : CEffect(1222) {
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
void CSoulUnityEffect::Slot01() {
  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 0);
  if (particle) {
    particle->SetTexture("effect\\soul_unity\\soul_unity.bmp");
    particle->unknown_0004 = this->id;
    particle->unknown_000C = 1;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->scale = 2.5f;
    particle->depthOffset = 0.035f;
    InitParticleQuad(particle->vertices);
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x0072B3B0
// FUNCTION: fn_07471
bool CSoulUnityEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0072B3C0
// FUNCTION: fn_07472
void CSoulUnityEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (!particle) {
      continue;
    }

    float height = -15.0f;
    CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
    if (actor && actor->unknown_02CC) {
      height = -20.0f;
    }

    matrix rotation;
    rotation.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
    rotation.AppendYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / 180.0f * 3.141592f);

    const float x = 0.0f;
    const float y = height;
    const float z = 0.0f;
    const float rotatedX = y * rotation.values[3] + x * rotation.values[0] + z * rotation.values[6] + rotation.values[9];
    const float rotatedY = y * rotation.values[4] + x * rotation.values[1] + z * rotation.values[7] + rotation.values[10];
    const float rotatedZ = x * rotation.values[2] + y * rotation.values[5] + z * rotation.values[8] + rotation.values[11];

    matrix scaling;
    scaling.SetScale(particle->scale, particle->scale, 1.0f);
    matrix billboard;
    billboard.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
    billboard.AppendYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / 180.0f * 3.141592f);
    matrix translation;
    translation.MakeTranslate(rotatedX + this->position[0], rotatedY + this->position[1], rotatedZ + this->position[2]);
    matrix first;
    first.Compose(scaling, billboard);
    matrix result;
    result.Compose(first, translation);
    particle->transform = result;
  }
}

// OFFSET: 0x0072B920
// FUNCTION: fn_07473
void CSoulUnityEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->texture) {
      CTexture* texture = particle->texture;
      const float left = g_textureOffsetX / texture->TextureWidth();
      const float right = (texture->Width() + g_textureOffsetX) / texture->TextureWidth();
      const float top = g_textureOffsetY / texture->TextureHeight();
      const float bottom = (texture->Height() + g_textureOffsetY) / particle->texture->TextureHeight();
      particle->Render(true, bottom, top, left, right, false, true);
    }
  }
}
