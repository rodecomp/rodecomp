#include "Contents/Effects/AcidifiedThrow.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectEmitter.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Contents/World/WorldObjects.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// OFFSET: 0x006F3540
// FUNCTION: fn_06821
CAcidifiedThrow::CAcidifiedThrow() : CEffect(-1) {
  this->unknown_00E4.x = 0.0f;
  this->unknown_00E4.y = 0.0f;
  this->unknown_00E4.z = 0.0f;
}

// OFFSET: 0x006F3750
// FUNCTION: fn_06823
void CAcidifiedThrow::Close() {
  if (this->unknown_00CC) {
    FreeParticle(reinterpret_cast<CParticle*>(this->unknown_00CC));
    this->unknown_00CC = nullptr;
  }

  for (__int64 i = 0; i < 3; ++i) {
    if (this->unknown_00D0[i]) {
      FreeParticle(reinterpret_cast<CParticle*>(this->unknown_00D0[i]));
      this->unknown_00D0[i] = nullptr;
    }
  }
}

// OFFSET: 0x006F37D0
// FUNCTION: fn_06824
void CAcidifiedThrow::Slot01() {
  this->unknown_00CC = reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  this->unknown_00D0[0] = reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  this->unknown_00D0[1] = reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  this->unknown_00D0[2] = reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));

  if (!this->unknown_00CC || !this->unknown_00D0[0] || !this->unknown_00D0[1] || !this->unknown_00D0[2]) {
    return;
  }

  switch (this->id) {
  case 0x89E:
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00CC)->Load("acidified_zone_fire\\acidified_zone_fire_c\\acidified_zone_fire_c.str");
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[0])->Load("acidified_zone_fire\\acidified_zone_fire_b\\acidified_zone_fire_b.str");
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[1])->Load("acidified_zone_fire\\acidified_zone_fire_b_bottom\\acidified_zone_fire_b_bottom.str");
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[2])->Load("acidified_zone_fire\\acidified_zone_fire_water\\acidified_zone_fire_water.str");
    break;

  case 0x8A6:
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00CC)->Load("acidified_zone_water\\acidified_zone_water_c\\acidified_zone_water_c.str");
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[0])->Load("acidified_zone_water\\acidified_zone_water_b\\acidified_zone_water_b.str");
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[1])->Load("acidified_zone_water\\acidified_zone_water_b_bottom\\acidified_zone_water_b_bottom.str");
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[2])->Load("acidified_zone_water\\acidified_zone_water_water\\acidified_zone_water_water.str");
    break;

  case 0x8AE:
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00CC)->Load("acidified_zone_wind\\acidified_zone_wind_c\\acidified_zone_wind_c.str");
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[0])->Load("acidified_zone_wind\\acidified_zone_wind_b\\acidified_zone_wind_b.str");
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[1])->Load("acidified_zone_wind\\acidified_zone_wind_b_bottom\\acidified_zone_wind_b_bottom.str");
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[2])->Load("acidified_zone_wind\\acidified_zone_wind_water\\acidified_zone_wind_water.str");
    break;

  case 0x8B6:
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00CC)->Load("acidified_zone_ground\\acidified_zone_ground_c\\acidified_zone_ground_c.str");
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[0])->Load("acidified_zone_ground\\acidified_zone_ground_b\\acidified_zone_ground_b.str");
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[1])->Load("acidified_zone_ground\\acidified_zone_ground_b_bottom\\acidified_zone_ground_b_bottom.str");
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[2])->Load("acidified_zone_ground\\acidified_zone_ground_water\\acidified_zone_ground_water.str");
    break;

  default:
    break;
  }

  this->unknown_00CC->looping = 1;
  this->unknown_00E4.x = 0.0f;
  this->unknown_00E4.y = 0.0f;
  this->unknown_00E4.z = 0.0f;
  this->unknown_00E0 = 1.0f;
  this->active = true;
}

// OFFSET: 0x006F3F90
// FUNCTION: fn_06825
bool CAcidifiedThrow::Slot06() {
  return this->active;
}

// OFFSET: 0x006F3FA0
// FUNCTION: fn_06826
void CAcidifiedThrow::Slot03() {
  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor) {
    this->active = false;
    return;
  }

  CRenderObject* sourceActor = FindRenderObject(this->sourceActor);
  if (sourceActor) {
    this->unknown_00E4 = sourceActor->render.position;
  }

  if (this->unknown_00CC) {
    const vector3d actorPos = actor->render.position;
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const matrix& view = mode->camera->frame.view;

    float actorViewX = actorPos.x * view.values[0] + actorPos.y * view.values[3] + actorPos.z * view.values[6] + view.values[9];
    float actorViewY = actorPos.x * view.values[1] + actorPos.y * view.values[4] + actorPos.z * view.values[7] + view.values[10];
    float actorViewZ = actorPos.x * view.values[2] + actorPos.y * view.values[5] + actorPos.z * view.values[8] + view.values[11];

    float targetViewX = this->unknown_00E4.x * view.values[0] + this->unknown_00E4.y * view.values[3] + this->unknown_00E4.z * view.values[6] + view.values[9];

    matrix rotation;
    if (targetViewX >= actorViewX) {
      rotation.MakeYRotation(4.1887894f);
    } else {
      rotation.MakeYRotation(2.0943947f);
    }

    actorViewY -= 10.0f;

    const matrix& inverse = mode->camera->frame.inverse;
    float worldX = actorViewX * inverse.values[0] + actorViewY * inverse.values[3] + actorViewZ * inverse.values[6] + inverse.values[9];
    float worldY = actorViewX * inverse.values[1] + actorViewY * inverse.values[4] + actorViewZ * inverse.values[7] + inverse.values[10];
    float worldZ = actorViewX * inverse.values[2] + actorViewY * inverse.values[5] + actorViewZ * inverse.values[8] + inverse.values[11];

    this->unknown_00E0 -= 0.07f;

    const float dx = this->unknown_00E4.x - worldX;
    const float dy = this->unknown_00E4.y - worldY;
    const float dz = this->unknown_00E4.z - worldZ;
    const float distSq = dy * dy + dx * dx + dz * dz;
    const float length = SquareRoot(distSq);
    const float inverseDist = 1.0f / SquareRoot(distSq);

    vector3d offset;
    offset.x = dx * inverseDist * length;
    offset.y = dy * inverseDist * length;
    offset.z = dz * inverseDist * length;

    float rotX = offset.x * rotation.values[0] + offset.y * rotation.values[3] + offset.z * rotation.values[6] + rotation.values[9];
    float rotY = offset.x * rotation.values[1] + offset.y * rotation.values[4] + offset.z * rotation.values[7] + rotation.values[10];
    float rotZ = offset.x * rotation.values[2] + offset.y * rotation.values[5] + offset.z * rotation.values[8] + rotation.values[11];

    float p1X = this->unknown_00E4.x + rotX;
    float p1Y = this->unknown_00E4.y + rotY;
    float p1Z = this->unknown_00E4.z + rotZ;

    float t = this->unknown_00E0;
    float oneMinusT = 1.0f - t;
    float term0 = static_cast<float>(std::pow(static_cast<double>(oneMinusT), 2.0));
    float term1 = (t + t) * oneMinusT;
    float term2 = static_cast<float>(std::pow(static_cast<double>(t), 2.0));

    EZ2STREmitter* emitter = this->unknown_00CC;
    emitter->position.x = term0 * this->unknown_00E4.x + term1 * p1X + term2 * worldX;
    emitter->position.y = term0 * this->unknown_00E4.y + term1 * p1Y + term2 * worldY;
    emitter->position.z = term0 * this->unknown_00E4.z + term1 * p1Z + term2 * worldZ;

    reinterpret_cast<CEZ2STRParticle*>(emitter)->Process(0.0f, 0.0f, 0.0f);

    if (this->unknown_00E0 > 0.0f) {
      return;
    }

    FreeParticle(reinterpret_cast<CParticle*>(this->unknown_00CC));
    this->unknown_00CC = nullptr;

    CRenderObject* player = reinterpret_cast<CRenderObject*>(mode->scene->player);
    float soundX = player->render.position.x - this->unknown_00E4.x;
    float soundZ = player->render.position.z - this->unknown_00E4.z;

    g_soundMgr->PlayWave("effect\\BO_ACIDIFIED_ZONE.wav", soundX, 0.0f, soundZ, 250, 40, 1.0f, 0);

    switch (this->id) {
    case 0x89E:
      g_soundMgr->PlayWave("effect\\BO_ACIDIFIED_ZONE_FIRE.wav", soundX, 0.0f, soundZ, 250, 40, 1.0f, 0);
      return;
    case 0x8A6:
      g_soundMgr->PlayWave("effect\\BO_ACIDIFIED_ZONE_WATER.wav", soundX, 0.0f, soundZ, 250, 40, 1.0f, 0);
      return;
    case 0x8AE:
      g_soundMgr->PlayWave("effect\\BO_ACIDIFIED_ZONE_WIND.wav", soundX, 0.0f, soundZ, 250, 40, 1.0f, 0);
      return;
    case 0x8B6:
      g_soundMgr->PlayWave("effect\\BO_ACIDIFIED_ZONE_GROUND.wav", soundX, 0.0f, soundZ, 250, 40, 1.0f, 0);
      return;
    default:
      return;
    }
  }

  __int64 activeCount = 0;
  for (__int64 i = 0; i < 3; ++i) {
    EZ2STREmitter* emitter = this->unknown_00D0[i];
    if (emitter) {
      if (emitter->lifetime == 0.0f || static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f) {
        emitter->position.x = this->unknown_00E4.x - 1.0f;
        emitter->position.y = this->unknown_00E4.y - 14.0f;
        emitter->position.z = this->unknown_00E4.z;
        reinterpret_cast<CEZ2STRParticle*>(emitter)->Process(0.0f, 0.0f, 0.0f);
        ++activeCount;
      } else {
        FreeParticle(reinterpret_cast<CParticle*>(emitter));
        this->unknown_00D0[i] = nullptr;
      }
    }
  }

  if (activeCount == 0) {
    this->active = false;
  }
}

// OFFSET: 0x006F4790
// FUNCTION: fn_06827
void CAcidifiedThrow::Slot04() {
  if (this->unknown_00CC) {
    reinterpret_cast<CEZ2STRParticle*>(this->unknown_00CC)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    return;
  }

  for (__int64 i = 0; i < 3; ++i) {
    if (this->unknown_00D0[i]) {
      reinterpret_cast<CEZ2STRParticle*>(this->unknown_00D0[i])->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
