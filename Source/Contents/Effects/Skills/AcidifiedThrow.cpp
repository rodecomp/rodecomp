#include "Contents/Effects/Skills/AcidifiedThrow.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/Core/EffectTypes.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particles/EffectEmitter.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// OFFSET: 0x006F3540
// FUNCTION: fn_06821
CAcidifiedThrow::CAcidifiedThrow() : CEffect(-1) {
  this->impactPosition.x = 0.0f;
  this->impactPosition.y = 0.0f;
  this->impactPosition.z = 0.0f;
}

// OFFSET: 0x006F3750
// FUNCTION: fn_06823
void CAcidifiedThrow::Close() {
  if (this->projectileEmitter) {
    FreeParticle(reinterpret_cast<CParticle*>(this->projectileEmitter));
    this->projectileEmitter = nullptr;
  }

  for (__int64 i = 0; i < 3; ++i) {
    if (this->impactEmitters[i]) {
      FreeParticle(reinterpret_cast<CParticle*>(this->impactEmitters[i]));
      this->impactEmitters[i] = nullptr;
    }
  }
}

// OFFSET: 0x006F37D0
// FUNCTION: fn_06824
void CAcidifiedThrow::Open() {
  this->projectileEmitter = reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  this->impactEmitters[0] = reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  this->impactEmitters[1] = reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 13));
  this->impactEmitters[2] = reinterpret_cast<EZ2STREmitter*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));

  if (!this->projectileEmitter || !this->impactEmitters[0] || !this->impactEmitters[1] || !this->impactEmitters[2]) {
    return;
  }

  switch (this->id) {
  case EF_ACIDIFIED_ZONE_FIRE:
    reinterpret_cast<CEZ2STRParticle*>(this->projectileEmitter)->Load("acidified_zone_fire\\acidified_zone_fire_c\\acidified_zone_fire_c.str");
    reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[0])->Load("acidified_zone_fire\\acidified_zone_fire_b\\acidified_zone_fire_b.str");
    reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[1])->Load("acidified_zone_fire\\acidified_zone_fire_b_bottom\\acidified_zone_fire_b_bottom.str");
    reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[2])->Load("acidified_zone_fire\\acidified_zone_fire_water\\acidified_zone_fire_water.str");
    break;

  case EF_ACIDIFIED_ZONE_WATER:
    reinterpret_cast<CEZ2STRParticle*>(this->projectileEmitter)->Load("acidified_zone_water\\acidified_zone_water_c\\acidified_zone_water_c.str");
    reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[0])->Load("acidified_zone_water\\acidified_zone_water_b\\acidified_zone_water_b.str");
    reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[1])->Load("acidified_zone_water\\acidified_zone_water_b_bottom\\acidified_zone_water_b_bottom.str");
    reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[2])->Load("acidified_zone_water\\acidified_zone_water_water\\acidified_zone_water_water.str");
    break;

  case EF_ACIDIFIED_ZONE_WIND:
    reinterpret_cast<CEZ2STRParticle*>(this->projectileEmitter)->Load("acidified_zone_wind\\acidified_zone_wind_c\\acidified_zone_wind_c.str");
    reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[0])->Load("acidified_zone_wind\\acidified_zone_wind_b\\acidified_zone_wind_b.str");
    reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[1])->Load("acidified_zone_wind\\acidified_zone_wind_b_bottom\\acidified_zone_wind_b_bottom.str");
    reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[2])->Load("acidified_zone_wind\\acidified_zone_wind_water\\acidified_zone_wind_water.str");
    break;

  case EF_ACIDIFIED_ZONE_GROUND:
    reinterpret_cast<CEZ2STRParticle*>(this->projectileEmitter)->Load("acidified_zone_ground\\acidified_zone_ground_c\\acidified_zone_ground_c.str");
    reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[0])->Load("acidified_zone_ground\\acidified_zone_ground_b\\acidified_zone_ground_b.str");
    reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[1])->Load("acidified_zone_ground\\acidified_zone_ground_b_bottom\\acidified_zone_ground_b_bottom.str");
    reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[2])->Load("acidified_zone_ground\\acidified_zone_ground_water\\acidified_zone_ground_water.str");
    break;

  default:
    break;
  }

  this->projectileEmitter->looping = 1;
  this->impactPosition.x = 0.0f;
  this->impactPosition.y = 0.0f;
  this->impactPosition.z = 0.0f;
  this->trajectoryTime = 1.0f;
  this->active = true;
}

// OFFSET: 0x006F3F90
// FUNCTION: fn_06825
bool CAcidifiedThrow::IsAlive() {
  return this->active;
}

// OFFSET: 0x006F3FA0
// FUNCTION: fn_06826
void CAcidifiedThrow::OnProcess() {
  CRenderObject* actor = FindRenderObject(this->actor);
  if (!actor) {
    this->active = false;
    return;
  }

  CRenderObject* sourceActor = FindRenderObject(this->sourceActor);
  if (sourceActor) {
    this->impactPosition.x = sourceActor->render.position.x;
    this->impactPosition.y = sourceActor->render.position.y;
    this->impactPosition.z = sourceActor->render.position.z;
  }

  if (this->projectileEmitter) {
    const vector3d actorPos = actor->render.position;
    const matrix& view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;

    float actorViewX = actorPos.x * view.values[0] + actorPos.y * view.values[3] + actorPos.z * view.values[6] + view.values[9];
    float actorViewY = actorPos.x * view.values[1] + actorPos.y * view.values[4] + actorPos.z * view.values[7] + view.values[10];
    float actorViewZ = actorPos.x * view.values[2] + actorPos.y * view.values[5] + actorPos.z * view.values[8] + view.values[11];

    float targetViewX = this->impactPosition.x * view.values[0] + this->impactPosition.y * view.values[3] + this->impactPosition.z * view.values[6] + view.values[9];

    matrix rotation;
    if (targetViewX >= actorViewX) {
      rotation.MakeYRotation(4.1887894f);
    } else {
      rotation.MakeYRotation(2.0943947f);
    }

    actorViewY -= 10.0f;

    const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;
    float worldX = actorViewX * inverse.values[0] + actorViewY * inverse.values[3] + actorViewZ * inverse.values[6] + inverse.values[9];
    float worldY = actorViewX * inverse.values[1] + actorViewY * inverse.values[4] + actorViewZ * inverse.values[7] + inverse.values[10];
    float worldZ = actorViewX * inverse.values[2] + actorViewY * inverse.values[5] + actorViewZ * inverse.values[8] + inverse.values[11];

    this->trajectoryTime -= 0.07f;

    const float dx = this->impactPosition.x - worldX;
    const float dy = this->impactPosition.y - worldY;
    const float dz = this->impactPosition.z - worldZ;
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

    float p1X = this->impactPosition.x + rotX;
    float p1Y = this->impactPosition.y + rotY;
    float p1Z = this->impactPosition.z + rotZ;

    float t = this->trajectoryTime;
    const vector3d start = this->impactPosition;
    float oneMinusT = 1.0f - t;
    float term0 = static_cast<float>(std::pow(static_cast<double>(oneMinusT), 2.0));
    float term1 = (t + t) * oneMinusT;
    float term2 = static_cast<float>(std::pow(static_cast<double>(t), 2.0));

    EZ2STREmitter* emitter = this->projectileEmitter;
    emitter->position.x = term1 * p1X + term2 * worldX + term0 * start.x;
    emitter->position.y = term1 * p1Y + term2 * worldY + term0 * start.y;
    emitter->position.z = term1 * p1Z + term2 * worldZ + term0 * start.z;

    reinterpret_cast<CEZ2STRParticle*>(emitter)->Process(0.0f, 0.0f, 0.0f);

    if (!(this->trajectoryTime <= 0.0f)) {
      return;
    }

    FreeParticle(reinterpret_cast<CParticle*>(this->projectileEmitter));
    this->projectileEmitter = nullptr;

    CRenderObject* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
    float soundX = player->render.position.x - this->impactPosition.x;
    float soundZ = player->render.position.z - this->impactPosition.z;

    g_soundMgr->PlayWave("effect\\BO_ACIDIFIED_ZONE.wav", soundX, 0.0f, soundZ, 250, 40, 1.0f, 0);

    switch (this->id) {
    case EF_ACIDIFIED_ZONE_FIRE:
      g_soundMgr->PlayWave("effect\\BO_ACIDIFIED_ZONE_FIRE.wav", soundX, 0.0f, soundZ, 250, 40, 1.0f, 0);
      return;
    case EF_ACIDIFIED_ZONE_WATER:
      g_soundMgr->PlayWave("effect\\BO_ACIDIFIED_ZONE_WATER.wav", soundX, 0.0f, soundZ, 250, 40, 1.0f, 0);
      return;
    case EF_ACIDIFIED_ZONE_WIND:
      g_soundMgr->PlayWave("effect\\BO_ACIDIFIED_ZONE_WIND.wav", soundX, 0.0f, soundZ, 250, 40, 1.0f, 0);
      return;
    case EF_ACIDIFIED_ZONE_GROUND:
      g_soundMgr->PlayWave("effect\\BO_ACIDIFIED_ZONE_GROUND.wav", soundX, 0.0f, soundZ, 250, 40, 1.0f, 0);
      return;
    default:
      return;
    }
  }

  __int64 activeCount = 0;
  for (__int64 i = 0; i < 3; ++i) {
    EZ2STREmitter* emitter = this->impactEmitters[i];
    if (emitter) {
      if (0.0f == emitter->lifetime || static_cast<float>(emitter->elapsed) < emitter->lifetime * 60.0f) {
        emitter->position.x = this->impactPosition.x - 1.0f;
        emitter->position.y = this->impactPosition.y - 14.0f;
        emitter->position.z = this->impactPosition.z;
        reinterpret_cast<CEZ2STRParticle*>(emitter)->Process(0.0f, 0.0f, 0.0f);
        ++activeCount;
      } else {
        FreeParticle(reinterpret_cast<CParticle*>(emitter));
        this->impactEmitters[i] = nullptr;
      }
    }
  }

  if (activeCount == 0) {
    this->active = false;
  }
}

// OFFSET: 0x006F4790
// FUNCTION: fn_06827
void CAcidifiedThrow::Render() {
  if (this->projectileEmitter) {
    reinterpret_cast<CEZ2STRParticle*>(this->projectileEmitter)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    return;
  }

  for (__int64 i = 0; i < 3; ++i) {
    if (this->impactEmitters[i]) {
      reinterpret_cast<CEZ2STRParticle*>(this->impactEmitters[i])->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}
