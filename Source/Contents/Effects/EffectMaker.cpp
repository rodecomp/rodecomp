#include "Contents/Effects/EffectMaker.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/World/World.h"
#include "Resource/World/WorldResource.h"

#include <cstdlib>

namespace {
  // INLINED
  inline int RandomBelow(int range) {
    int value = 0;
    if (range != 0) {
      const int remainder = rand() % range;
      if (remainder >= 0) {
        value = remainder;
      }
    }

    return value;
  }
}

// OFFSET: 0x00E4A2B0
// FUNCTION: fn_24251
CEffectMaker::CEffectMaker() {
  this->unknown_0160 = -1;
  this->unknown_015C = 360;
  this->unknown_0158 = 360;
  this->unknown_0164 = 0;
  this->unknown_0168 = 0;
  this->unknown_016C = 0;
  this->unknown_0170 = 0;
}

// OFFSET: 0x00E4BA80
// FUNCTION: fn_24278
void CEffectMaker::Reset() {
  this->unknown_0160 = -1;
  this->unknown_015C = 360;
  this->unknown_0158 = 360;
  this->unknown_0164 = 0;
  this->unknown_0168 = 0;
  this->unknown_016C = 0;
  this->unknown_0170 = 0;
}

// OFFSET: 0x00E4C210
// FUNCTION: fn_24282
bool CEffectMaker::OnProcess() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CPlayer* player = mode->scene->player;
  if (this->CalcDist(player->render.position.x, player->render.position.z) < 130.0f && this->unknown_0158 >= this->unknown_015C) {
    this->unknown_0158 = 0;
    CRagEffect* effect = this->LaunchEffect(this->unknown_0160, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect != nullptr) {
      effect->SendMsg(nullptr, 46, 0, reinterpret_cast<int>(&this->unknown_0164), 0, 0, 0, 0);
    }
  }

  ++this->unknown_0158;
  return true;
}

// OFFSET: 0x00E4FD60
// FUNCTION: fn_24290
void CEffectMaker::Render(int) {
}

// OFFSET: 0x00E50BB0
// FUNCTION: fn_24297
void CEffectMaker::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int) {
  if ((static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message)) != 14) {
    this->EffectActor::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  const WorldEffectRecord* record = reinterpret_cast<const WorldEffectRecord*>(b);
  this->unknown_0160 = record->id;
  if (record->id != 45) {
    this->unknown_015C = static_cast<unsigned>(record->delay);
    this->unknown_0158 = static_cast<unsigned>(record->delay - static_cast<float>(RandomBelow(24)));
  } else {
    const int jitter = RandomBelow(static_cast<int>(record->delay / 10.0f));
    this->unknown_015C = static_cast<unsigned>(static_cast<float>(jitter) + record->delay - record->delay / 20.0f);
    this->unknown_0158 = RandomBelow(static_cast<int>(record->delay + 1.0f));
  }

  this->render.position.x = record->position[0];
  this->render.position.y = record->position[1];
  this->render.position.z = record->position[2];
  this->unknown_0164 = record->parameters[0];
  this->unknown_0168 = record->parameters[1];
  this->unknown_016C = record->parameters[2];
  this->unknown_0170 = record->parameters[3];
}
