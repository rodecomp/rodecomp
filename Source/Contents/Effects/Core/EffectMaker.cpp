#include "Contents/Effects/Core/EffectMaker.h"
#include "Contents/Actors/ActorEnums.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/World/World.h"
#include "Resource/World/WorldResource.h"

#include <algorithm>
#include <cstdlib>

namespace {
  // INLINED
  inline int RandomBelow(int range) {
    if (range == 0) {
      return 0;
    }

    return std::ranges::max(rand() % range, 0);
  }
}

// OFFSET: 0x00E4A2B0
// FUNCTION: fn_24251
CEffectMaker::CEffectMaker() {
  this->effectId = -1;
  this->spawnInterval = 360;
  this->elapsedFrames = 360;
  this->parameter1 = 0;
  this->parameter2 = 0;
  this->parameter3 = 0;
  this->parameter4 = 0;
}

// OFFSET: 0x00E4BA80
// FUNCTION: fn_24278
void CEffectMaker::Reset() {
  this->effectId = -1;
  this->spawnInterval = 360;
  this->elapsedFrames = 360;
  this->parameter1 = 0;
  this->parameter2 = 0;
  this->parameter3 = 0;
  this->parameter4 = 0;
}

// OFFSET: 0x00E4C210
// FUNCTION: fn_24282
bool CEffectMaker::OnProcess() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CPlayer* player = mode->scene->player;
  if (this->CalcDist(player->render.position.x, player->render.position.z) < 130.0f && this->elapsedFrames >= this->spawnInterval) {
    this->elapsedFrames = 0;
    CRagEffect* effect = this->LaunchEffect(this->effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect != nullptr) {
      effect->SendMsg(nullptr, AM_SETPARAM, 0, reinterpret_cast<int>(&this->parameter1), 0, 0, 0, 0);
    }
  }

  ++this->elapsedFrames;
  return true;
}

// OFFSET: 0x00E4FD60
// FUNCTION: fn_24290
void CEffectMaker::Render(int) {
}

// OFFSET: 0x00E50BB0
// FUNCTION: fn_24297
void CEffectMaker::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int) {
  if (message != 14 || a != 0) {
    this->EffectActor::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  const WorldEffectRecord* record = reinterpret_cast<const WorldEffectRecord*>(b);
  this->effectId = record->id;
  if (this->effectId != 45) {
    this->spawnInterval = static_cast<unsigned>(record->delay);
    this->elapsedFrames = static_cast<unsigned>(record->delay - static_cast<float>(RandomBelow(24)));
  } else {
    const int jitter = RandomBelow(static_cast<int>(record->delay / 10.0f));
    this->spawnInterval = static_cast<unsigned>(static_cast<float>(jitter) + record->delay - record->delay / 20.0f);
    this->elapsedFrames = RandomBelow(static_cast<int>(record->delay + 1.0f));
  }

  this->render.position.x = record->position[0];
  this->render.position.y = record->position[1];
  this->render.position.z = record->position[2];
  this->parameter1 = record->parameters[0];
  this->parameter2 = record->parameters[1];
  this->parameter3 = record->parameters[2];
  this->parameter4 = record->parameters[3];
}
