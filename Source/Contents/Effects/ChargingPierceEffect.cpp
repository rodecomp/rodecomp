#include "Contents/Effects/ChargingPierceEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/World/World.h"
#include "Render/Textures/Texture.h"
#include "Utility/Math/AffineMatrix.h"

#include <string>

// GLOBAL: gl_24880
static vector3d s_chargingPierceNewOffsets[10] = {
  {-7.5f, -23.45f, 0.0f}, {-3.75f, -23.45f, 0.0f}, {0.0f, -23.45f, 0.0f}, {3.75f, -23.45f, 0.0f}, {7.5f, -23.45f, 0.0f},
  {-7.5f, -20.0f, 0.0f},  {-3.75f, -20.0f, 0.0f},  {0.0f, -20.0f, 0.0f},  {3.75f, -20.0f, 0.0f},  {7.5f, -20.0f, 0.0f},
};

// OFFSET: 0x00703060
// FUNCTION: fn_07017
CChargingPierceEffect::CChargingPierceEffect() : CEffect(-1), offsets{} {
}

// OFFSET: 0x007030F0
// FUNCTION: fn_07018
CChargingPierceNewEffect::CChargingPierceNewEffect() : CEffect(-1) {
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
    particle->unknown_00D0 = s_chargingPierceNewOffsets[index].x;
    particle->unknown_00D4 = s_chargingPierceNewOffsets[index].y;
    particle->unknown_00D8 = s_chargingPierceNewOffsets[index].z;
    particle->m_hold = true;
    particle->m_scale = 0.75f;
    this->emitters.push_back(particle);
  }
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
void CChargingPierceEffect::Slot01() {
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
void CChargingPierceNewEffect::Slot01() {
  this->retained = 1;
}

// OFFSET: 0x00703C50
// FUNCTION: fn_07028
bool CChargingPierceEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00703C60
// FUNCTION: fn_07029
bool CChargingPierceNewEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00703DC0
// FUNCTION: fn_07030
void CChargingPierceEffect::Slot03() {
  for (int index = 0; index < 10; ++index) {
    const vector3d& offset = this->offsets[index];

    if (CParticle* spark = this->sparks[index]) {
      float height = -10.0f;
      if (CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
        height = -10.0f - static_cast<float>(actor->MountType() ? 3 : 0);
      }

      matrix rotation;
      rotation.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
      rotation.AppendYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / 180.0f * 3.141592f);

      const float y = height + offset.y;
      const float x = offset.x + 0.0f;
      const float z = offset.z + 0.0f;
      const float rotatedX = y * rotation.values[3] + x * rotation.values[0] + z * rotation.values[6] + rotation.values[9];
      const float rotatedY = y * rotation.values[4] + x * rotation.values[1] + z * rotation.values[7] + rotation.values[10];
      const float rotatedZ = y * rotation.values[5] + x * rotation.values[2] + z * rotation.values[8] + rotation.values[11];

      matrix scaling;
      scaling.SetScale(spark->scale, spark->scale, 1.0f);
      matrix billboard;
      billboard.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
      billboard.AppendYRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw / 180.0f * 3.141592f);
      matrix translation;
      translation.MakeTranslate(rotatedX + this->position[0], rotatedY + this->position[1], rotatedZ + this->position[2]);
      matrix first;
      first.Compose(scaling, billboard);
      matrix result;
      result.Compose(first, translation);
      spark->transform = result;
    }

    if (CEZ2STRParticle* emitter = this->emitters[index]) {
      if (emitter->life == 0.0f || static_cast<float>(emitter->unknown_00E8) < emitter->life * 60.0f) {
        if (g_modeMgr.GetGameMode()) {
          float height = -13.0f;
          if (CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
            height = -13.0f - static_cast<float>(actor->MountType() ? 3 : 0);
          }

          const float x = this->position[0];
          const float y = this->position[1];
          const float z = this->position[2];
          const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
          const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;
          const float viewZ = y * view[5] + x * view[2] + z * view[8] + view[11];
          const float viewX = y * view[3] + x * view[0] + z * view[6] + view[9] + offset.x;
          const float viewY = y * view[4] + x * view[1] + z * view[7] + view[10] + (height + offset.y);
          emitter->position.x = viewY * inverse[3] + viewX * inverse[0] + viewZ * inverse[6] + inverse[9];
          emitter->position.y = viewX * inverse[1] + viewY * inverse[4] + viewZ * inverse[7] + inverse[10];
          emitter->position.z = viewX * inverse[2] + viewY * inverse[5] + viewZ * inverse[8] + inverse[11];
          emitter->Process(0.0f, 0.0f, 0.0f);
          ++emitter->unknown_00E8;
        }
      } else {
        FreeParticle(emitter);
        this->emitters[index] = nullptr;
        if (!this->sparks[index]) {
          CParticle* spark = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 0x101);
          spark->unknown_0004 = this->id;
          spark->unknown_000C = 1;
          spark->unknown_0010 = 0;
          spark->unknown_0008 = 1;
          spark->SetTexture("effect\\new_chargingpierce\\cha_orb.bmp");
          spark->scale = 1.5f;
          spark->depthOffset = 0.035f;
          InitParticleQuad(spark->vertices);
          this->sparks[index] = spark;
        }
      }
    }
  }
}

// OFFSET: 0x00704360
// FUNCTION: fn_07031
void CChargingPierceNewEffect::Slot03() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    const float x = this->position[0];
    const float y = this->position[1];
    const float z = this->position[2];
    CEZ2STRParticle* particle = *it;

    const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
    const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;
    const float viewX = view[0] * x + view[3] * y + view[6] * z + view[9] + particle->unknown_00D0;
    const float viewY = view[1] * x + view[4] * y + view[7] * z + view[10] + particle->unknown_00D4;
    const float viewZ = view[2] * x + view[5] * y + view[8] * z + view[11] + particle->unknown_00D8;
    particle->position.x = viewY * inverse[3] + viewX * inverse[0] + viewZ * inverse[6] + inverse[9];
    particle->position.y = viewY * inverse[4] + viewX * inverse[1] + viewZ * inverse[7] + inverse[10];
    particle->position.z = viewY * inverse[5] + viewX * inverse[2] + viewZ * inverse[8] + inverse[11];
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x00704A30
// FUNCTION: fn_07032
void CChargingPierceEffect::Slot04() {
  for (int index = 0; index < 10; ++index) {
    CParticle* spark = this->sparks[index];
    if (spark) {
      CTexture* texture = spark->texture;
      const float left = g_textureOffsetX / texture->TextureWidth();
      const float right = (texture->Width() + g_textureOffsetX) / texture->TextureWidth();
      const float top = g_textureOffsetY / texture->TextureHeight();
      const float bottom = (texture->Height() + g_textureOffsetY) / spark->texture->TextureHeight();
      spark->Render(true, bottom, top, left, right, false, true);
    }

    if (this->emitters[index]) {
      this->emitters[index]->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

// OFFSET: 0x00704BB0
// FUNCTION: fn_07033
void CChargingPierceNewEffect::Slot04() {
  for (auto it = this->emitters.begin(); it != this->emitters.end(); ++it) {
    (*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}
