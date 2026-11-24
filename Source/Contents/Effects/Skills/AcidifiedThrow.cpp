#include "Contents/Effects/Skills/AcidifiedThrow.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/Core/EffectTypes.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Render/Projection/RenderProjection.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

namespace {
// INLINED
__forceinline bool IsParticleActive(const CEZ2STRParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}
}

// OFFSET: 0x006F3540
// FUNCTION: fn_06821
CAcidifiedThrow::CAcidifiedThrow() : CEffect(-1) {
  this->impactPosition.x = 0.0f;
  this->impactPosition.y = 0.0f;
  this->impactPosition.z = 0.0f;
}

// OFFSET: 0x006F35C0
// FUNCTION: fn_46014
__declspec(noinline) CAcidifiedThrow::~CAcidifiedThrow() {
}

// OFFSET: 0x006F3680
// FUNCTION: fn_46015
vector3d CAcidifiedThrow::PointOnCurve(vector3d start, vector3d control, vector3d end, float t) {
  const float oneMinusT = 1.0f - t;
  const vector3d first = start * static_cast<float>(std::pow(static_cast<double>(oneMinusT), 2.0));
  const vector3d third = end * static_cast<float>(std::pow(static_cast<double>(t), 2.0));
  const vector3d second = control * ((t + t) * oneMinusT);
  return vector3d{second.x + third.x + first.x, second.y + third.y + first.y, second.z + third.z + first.z};
}

// OFFSET: 0x006F3750
// FUNCTION: fn_06823
void CAcidifiedThrow::Close() {
  if (this->projectileEmitter) {
    FreeParticle(this->projectileEmitter);
    this->projectileEmitter = nullptr;
  }

  for (__int64 i = 0; i < 3; ++i) {
    if (this->impactEmitters[i]) {
      FreeParticle(this->impactEmitters[i]);
      this->impactEmitters[i] = nullptr;
    }
  }
}

// OFFSET: 0x006F37D0
// FUNCTION: fn_06824
void CAcidifiedThrow::Open() {
  this->projectileEmitter = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  this->impactEmitters[0] = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  this->impactEmitters[1] = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 13));
  this->impactEmitters[2] = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));

  if (!this->projectileEmitter || !this->impactEmitters[0] || !this->impactEmitters[1] || !this->impactEmitters[2]) {
    return;
  }

  switch (this->id) {
  case EF_ACIDIFIED_ZONE_FIRE:
    this->projectileEmitter->Load("acidified_zone_fire\\acidified_zone_fire_c\\acidified_zone_fire_c.str");
    this->impactEmitters[0]->Load("acidified_zone_fire\\acidified_zone_fire_b\\acidified_zone_fire_b.str");
    this->impactEmitters[1]->Load("acidified_zone_fire\\acidified_zone_fire_b_bottom\\acidified_zone_fire_b_bottom.str");
    this->impactEmitters[2]->Load("acidified_zone_fire\\acidified_zone_fire_water\\acidified_zone_fire_water.str");
    break;

  case EF_ACIDIFIED_ZONE_WATER:
    this->projectileEmitter->Load("acidified_zone_water\\acidified_zone_water_c\\acidified_zone_water_c.str");
    this->impactEmitters[0]->Load("acidified_zone_water\\acidified_zone_water_b\\acidified_zone_water_b.str");
    this->impactEmitters[1]->Load("acidified_zone_water\\acidified_zone_water_b_bottom\\acidified_zone_water_b_bottom.str");
    this->impactEmitters[2]->Load("acidified_zone_water\\acidified_zone_water_water\\acidified_zone_water_water.str");
    break;

  case EF_ACIDIFIED_ZONE_WIND:
    this->projectileEmitter->Load("acidified_zone_wind\\acidified_zone_wind_c\\acidified_zone_wind_c.str");
    this->impactEmitters[0]->Load("acidified_zone_wind\\acidified_zone_wind_b\\acidified_zone_wind_b.str");
    this->impactEmitters[1]->Load("acidified_zone_wind\\acidified_zone_wind_b_bottom\\acidified_zone_wind_b_bottom.str");
    this->impactEmitters[2]->Load("acidified_zone_wind\\acidified_zone_wind_water\\acidified_zone_wind_water.str");
    break;

  case EF_ACIDIFIED_ZONE_GROUND:
    this->projectileEmitter->Load("acidified_zone_ground\\acidified_zone_ground_c\\acidified_zone_ground_c.str");
    this->impactEmitters[0]->Load("acidified_zone_ground\\acidified_zone_ground_b\\acidified_zone_ground_b.str");
    this->impactEmitters[1]->Load("acidified_zone_ground\\acidified_zone_ground_b_bottom\\acidified_zone_ground_b_bottom.str");
    this->impactEmitters[2]->Load("acidified_zone_ground\\acidified_zone_ground_water\\acidified_zone_ground_water.str");
    break;

  default:
    break;
  }

  this->projectileEmitter->m_loop = 1;
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
    this->impactPosition = sourceActor->render.position;
  }

  if (this->projectileEmitter) {
    const vector3d actorPos = actor->render.position;
    const matrix& view = g_modeMgr.GetGameMode()->camera->frame.view;

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

    actorViewY -= g_projectionDepthOffset;

    const matrix& inverse = g_modeMgr.GetGameMode()->camera->frame.inverse;
    const vector3d world{
        actorViewX * inverse.values[0] + actorViewY * inverse.values[3] + actorViewZ * inverse.values[6] + inverse.values[9],
        actorViewX * inverse.values[1] + actorViewY * inverse.values[4] + actorViewZ * inverse.values[7] + inverse.values[10],
        actorViewX * inverse.values[2] + actorViewY * inverse.values[5] + actorViewZ * inverse.values[8] + inverse.values[11]};

    this->trajectoryTime -= 0.07f;

    const vector3d delta{this->impactPosition.x - world.x, this->impactPosition.y - world.y, this->impactPosition.z - world.z};
    const float distSq = delta.y * delta.y + delta.x * delta.x + delta.z * delta.z;
    const float length = SquareRoot(distSq);
    const float inverseDist = 1.0f / SquareRoot(distSq);
    const vector3d offset{delta.x * inverseDist * length, delta.y * inverseDist * length, delta.z * inverseDist * length};
    const vector3d rotated{
        offset.x * rotation.values[0] + offset.y * rotation.values[3] + offset.z * rotation.values[6] + rotation.values[9],
        offset.x * rotation.values[1] + offset.y * rotation.values[4] + offset.z * rotation.values[7] + rotation.values[10],
        offset.x * rotation.values[2] + offset.y * rotation.values[5] + offset.z * rotation.values[8] + rotation.values[11]};
    const vector3d control{this->impactPosition.x + rotated.x, this->impactPosition.y + rotated.y, this->impactPosition.z + rotated.z};

    this->projectileEmitter->position = this->PointOnCurve(this->impactPosition, control, world, this->trajectoryTime);

    this->projectileEmitter->Process(0.0f, 0.0f, 0.0f);

    if (!(this->trajectoryTime <= 0.0f)) {
      return;
    }

    FreeParticle(this->projectileEmitter);
    this->projectileEmitter = nullptr;

    CRenderObject* player = g_modeMgr.GetGameMode()->scene->player;
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

  __int64 i = 0;
  __int64 activeCount = 0;
  for (; i < 3; ++i) {
    CEZ2STRParticle* emitter = this->impactEmitters[i];
    if (emitter) {
      if (IsParticleActive(emitter) != true) {
        FreeParticle(emitter);
        this->impactEmitters[i] = nullptr;
      } else {
        vector3d position = this->impactPosition;
        position.y -= 14.0f;
        position.x -= g_projectionDepthNumerator;
        emitter->position = position;
        this->impactEmitters[i]->Process(0.0f, 0.0f, 0.0f);
        ++activeCount;
      }
    }
  }

  if (0 == activeCount) {
    this->active = false;
  }
}

// OFFSET: 0x006F4790
// FUNCTION: fn_06827
void CAcidifiedThrow::Render() {
  if (this->projectileEmitter) {
    this->projectileEmitter->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
    return;
  }

  for (__int64 i = 0; i < 3; ++i) {
    if (this->impactEmitters[i]) {
      this->impactEmitters[i]->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
    }
  }
}
