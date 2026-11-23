#include "Contents/Effects/Skills/ChargingPierceEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/World/World.h"
#include "Render/Textures/Texture.h"
#include "Utility/Math/AffineMatrix.h"

#include <string>

// GLOBAL: gl_24880
static vector3d s_chargingPierceNewOffsets[10] = {
  {-7.5f, -23.45f, 0.0f}, {-3.75f, -23.45f, 0.0f}, {0.0f, -23.45f, 0.0f}, {3.75f, -23.45f, 0.0f}, {7.5f, -23.45f, 0.0f},
  {-7.5f, -20.0f, 0.0f},  {-3.75f, -20.0f, 0.0f},  {0.0f, -20.0f, 0.0f},  {3.75f, -20.0f, 0.0f},  {7.5f, -20.0f, 0.0f},
};

// INLINED
static void MatrixMult(vector3d& result, const vector3d& v, const float* m) {
  result.x = v.x * m[0] + v.y * m[3] + v.z * m[6] + m[9];
  result.y = v.x * m[1] + v.y * m[4] + v.z * m[7] + m[10];
  result.z = v.x * m[2] + v.y * m[5] + v.z * m[8] + m[11];
}

// INLINED
static __forceinline bool IsParticleActive(const CEZ2STRParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}

// OFFSET: 0x00703060
// FUNCTION: fn_07017
CChargingPierceEffect::CChargingPierceEffect() : CEffect(-1) {
}

// OFFSET: 0x007030F0
// FUNCTION: fn_07018
CChargingPierceNewEffect::CChargingPierceNewEffect() : CEffect(-1) {
}

// OFFSET: 0x007032D0
// FUNCTION: fn_07019
__declspec(noinline) CChargingPierceNewEffect::~CChargingPierceNewEffect() {
}

// OFFSET: 0x00703490
// FUNCTION: fn_07022
void CChargingPierceNewEffect::SetCount(int count) {
  if (count > 10) {
    return;
  }

  if (this->emitters.size() > static_cast<unsigned int>(count)) {
    this->Close();
  }

  for (int index = this->emitters.size(); index < count; ++index) {
    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&s_chargingPierceNewOffsets[index], 5));
    if (!particle) {
      continue;
    }

    particle->Load(std::string("DRAGON_KNIGHT\\DK_CHARGINGPIERCE\\chargingpierce_cast2\\chargingpierce_cast2.str"));
    particle->offset = s_chargingPierceNewOffsets[index];
    particle->m_hold = true;
    particle->m_scale = 0.75f;
    this->emitters.push_back(particle);
  }
}

// OFFSET: 0x007036B0
// FUNCTION: fn_46209
__declspec(noinline) void CChargingPierceEffect::SetCount(std::uint64_t count) {
  if (count > 10) {
    return;
  }

  if (this->ActiveCount() > count) {
    this->Close();
  }

  for (int index = this->ActiveCount(); index < count; ++index) {
    if (this->sparks[index] || this->emitters[index]) {
      continue;
    }

    CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
    particle->Open(std::string("new_chargingpierce\\new_chargingpierce_cast\\new_chargingpierce_cast.str"));
    this->emitters[index] = particle;
  }
}

// OFFSET: 0x00703C70
// FUNCTION: fn_46210
__declspec(noinline) CEZ2STRParticle* CChargingPierceNewEffect::CreateParticle(const vector3d* position) {
  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(position, 5));
  if (particle) {
    particle->Load(std::string("DRAGON_KNIGHT\\DK_CHARGINGPIERCE\\chargingpierce_cast2\\chargingpierce_cast2.str"));
    particle->offsetX = position->x;
    particle->offsetY = position->y;
    particle->offsetZ = position->z;
    particle->m_hold = true;
    particle->m_scale = 0.75f;
    return particle;
  }
  return nullptr;
}

// OFFSET: 0x007038B0
// FUNCTION: fn_07023
void CChargingPierceEffect::Close() {
  for (int index = 0; index < 10; ++index) {
    if (this->sparks[index]) {
      FreeParticle(this->sparks[index]);
      this->sparks[index] = nullptr;
    }

    if (this->emitters[index]) {
      FreeParticle(this->emitters[index]);
      this->emitters[index] = nullptr;
    }
  }
}

// OFFSET: 0x00703900
// FUNCTION: fn_07024
void CChargingPierceNewEffect::Close() {
  for (CEZ2STRParticle* particle : this->emitters) {
    FreeParticle(particle);
  }

  this->emitters.clear();
}

// OFFSET: 0x00703970
// FUNCTION: fn_07025
int CChargingPierceEffect::ActiveCount() {
  int count = 0;
  for (int index = 0; index < 10; ++index) {
    if (this->sparks[index] || this->emitters[index]) {
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x00703A40
// FUNCTION: fn_07026
void CChargingPierceEffect::Open() {
  this->retained = 1;
  for (int index = 0; index < 10; ++index) {
    this->sparks[index] = nullptr;
    this->emitters[index] = nullptr;
  }

  this->offsets[0] = {-5.0f, -2.5f, 0.0f};
  this->offsets[1] = {-2.5f, -2.5f, 0.0f};
  this->offsets[2] = {-0.0f, -2.5f, 0.0f};
  this->offsets[3] = {2.5f, -2.5f, 0.0f};
  this->offsets[4] = {5.0f, -2.5f, 0.0f};
  this->offsets[5] = {-5.0f, 0.0f, 0.0f};
  this->offsets[6] = {-2.5f, 0.0f, 0.0f};
  this->offsets[7] = {0.0f, 0.0f, 0.0f};
  this->offsets[8] = {2.5f, 0.0f, 0.0f};
  this->offsets[9] = {5.0f, 0.0f, 0.0f};
}

// OFFSET: 0x00703C40
// FUNCTION: fn_07027
void CChargingPierceNewEffect::Open() {
  this->retained = 1;
}

// OFFSET: 0x00703C50
// FUNCTION: fn_07028
bool CChargingPierceEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00703C60
// FUNCTION: fn_07029
bool CChargingPierceNewEffect::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x00703DC0
// FUNCTION: fn_07030
void CChargingPierceEffect::OnProcess() {
  for (int index = 0; index < 10; ++index) {
    const vector3d& offset = this->offsets[index];

    if (CParticle* spark = this->sparks[index]) {
      float height = -10.0f;
      if (CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
        height = -10.0f - static_cast<float>(actor->MountType() ? 3 : 0);
      }

      matrix rotation;
      rotation.MakeXRotation(g_modeMgr.GetGameMode()->camera->camera.pitch / 180.0f * 3.141592f);
      rotation.AppendYRotation(g_modeMgr.GetGameMode()->camera->camera.yaw / 180.0f * 3.141592f);

      const vector3d local{offset.x + 0.0f, height + offset.y, offset.z + 0.0f};
      vector3d rotated;
      MatrixMult(rotated, local, rotation.values);

      matrix scaling;
      scaling.SetScale(spark->scale, spark->scale, 1.0f);
      matrix billboard;
      billboard.MakeXRotation(g_modeMgr.GetGameMode()->camera->camera.pitch / 180.0f * 3.141592f);
      billboard.AppendYRotation(g_modeMgr.GetGameMode()->camera->camera.yaw / 180.0f * 3.141592f);
      matrix translation;
      const vector3d moved{rotated.x + this->position[0], rotated.y + this->position[1], rotated.z + this->position[2]};
      translation.MakeTranslate(moved.x, moved.y, moved.z);
      matrix first;
      first.Compose(scaling, billboard);
      matrix result;
      result.Compose(first, translation);
      spark->transform = result;
    }

    if (CEZ2STRParticle* emitter = this->emitters[index]) {
      if (IsParticleActive(emitter) == true) {
        if (g_modeMgr.GetGameMode()) {
          float height = -13.0f;
          if (CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
            height = -13.0f - static_cast<float>(actor->MountType() ? 3 : 0);
          }

          const vector3d origin = this->positionVector;
          const float* view = g_modeMgr.GetGameMode()->camera->frame.view.values;
          const float* inverse = g_modeMgr.GetGameMode()->camera->frame.inverse.values;
          vector3d viewPos;
          MatrixMult(viewPos, origin, view);
          viewPos.x += this->offsets[index].x;
          viewPos.y += height + this->offsets[index].y;
          vector3d worldPos;
          MatrixMult(worldPos, viewPos, inverse);
          emitter->position.x = worldPos.x;
          emitter->position.y = worldPos.y;
          emitter->position.z = worldPos.z;
          emitter->Process(0.0f, 0.0f, 0.0f);
          ++emitter->elapsedFrames;
        }
      } else {
        FreeParticle(emitter);
        this->emitters[index] = nullptr;
        if (!this->sparks[index]) {
          CParticle* spark = CreateParticle(&this->positionVector, 0x101);
          spark->effectId = this->id;
          spark->columnIndex = 1;
          spark->rowIndex = 0;
          spark->groupIndex = 1;
          spark->SetTexture("effect\\new_chargingpierce\\cha_orb.bmp");
          spark->scale = 1.5f;
          spark->depthOffset = 0.035f;
          spark->InitParticleQuad(spark->vertices);
          this->sparks[index] = spark;
        }
      }
    }
  }
}

// OFFSET: 0x00704580
// FUNCTION: fn_46211
__declspec(noinline) void CChargingPierceEffect::UpdateParticle(CEZ2STRParticle* particle, int index) {
  if (!particle) {
    return;
  }

  if (!g_modeMgr.GetGameMode()) {
    return;
  }

  float height = -13.0f;
  if (CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
    height = -13.0f - static_cast<float>(actor->MountType() ? 3 : 0);
  }

  const vector3d origin = this->positionVector;
  const float* view = g_modeMgr.GetGameMode()->camera->frame.view.values;
  const float* inverse = g_modeMgr.GetGameMode()->camera->frame.inverse.values;
  vector3d viewPos;
  MatrixMult(viewPos, origin, view);
  viewPos.x += this->offsets[index].x;
  viewPos.y += height + this->offsets[index].y;
  vector3d worldPos;
  MatrixMult(worldPos, viewPos, inverse);
  particle->position.x = worldPos.x;
  particle->position.y = worldPos.y;
  particle->position.z = worldPos.z;
  particle->Process(0.0f, 0.0f, 0.0f);
  ++particle->elapsedFrames;
}

// OFFSET: 0x00704360
// FUNCTION: fn_07031
void CChargingPierceNewEffect::OnProcess() {
  for (CEZ2STRParticle* particle : this->emitters) {
    const vector3d origin = this->positionVector;

    const matrix& view = g_modeMgr.GetGameMode()->camera->frame.view;
    vector3d viewPos;
    viewPos.x = origin.y * view.values[3] + origin.x * view.values[0] + origin.z * view.values[6] + view.values[9];
    viewPos.y = origin.x * view.values[1] + origin.y * view.values[4] + origin.z * view.values[7] + view.values[10];
    viewPos.z = origin.x * view.values[2] + origin.y * view.values[5] + origin.z * view.values[8] + view.values[11];

    const matrix& inverse = g_modeMgr.GetGameMode()->camera->frame.inverse;
    viewPos.x += particle->offsetX;
    viewPos.y += particle->offsetY;
    viewPos.z += particle->offsetZ;

    vector3d worldPos;
    worldPos.x = viewPos.y * inverse.values[3] + viewPos.x * inverse.values[0] + viewPos.z * inverse.values[6] + inverse.values[9];
    worldPos.y = viewPos.y * inverse.values[4] + viewPos.x * inverse.values[1] + viewPos.z * inverse.values[7] + inverse.values[10];
    worldPos.z = viewPos.y * inverse.values[5] + viewPos.x * inverse.values[2] + viewPos.z * inverse.values[8] + inverse.values[11];
    particle->position.x = worldPos.x;
    particle->position.y = worldPos.y;
    particle->position.z = worldPos.z;
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x00704A30
// FUNCTION: fn_07032
void CChargingPierceEffect::Render() {
  for (int index = 0; index < 10; ++index) {
    CParticle* spark = this->sparks[index];
    if (spark) {
      const float left = g_textureOffsetX / spark->texture->TextureWidth();
      const float right = (spark->texture->Width() + g_textureOffsetX) / spark->texture->TextureWidth();
      const float top = g_textureOffsetY / spark->texture->TextureHeight();
      const float bottom = (spark->texture->Height() + g_textureOffsetY) / spark->texture->TextureHeight();
      spark->Render(true, bottom, top, left, right, false, true);
    }

    if (this->emitters[index]) {
      this->emitters[index]->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
    }
  }
}

// OFFSET: 0x00704BB0
// FUNCTION: fn_07033
void CChargingPierceNewEffect::Render() {
  for (CEZ2STRParticle* particle : this->emitters) {
    particle->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}

// OFFSET: 0x00704BF0
// FUNCTION: fn_46212
__declspec(noinline) void CChargingPierceNewEffect::RenderParticle(CEZ2STRParticle* particle) {
  if (particle) {
    particle->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}
