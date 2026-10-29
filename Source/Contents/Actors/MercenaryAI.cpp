#include "Contents/Actors/MercenaryAI.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/World/World.h"

extern "C" {
#include <lua.h>
}

// OFFSET: 0x00D9E3F0
// FUNCTION: fn_22327
CMercenaryAI::CMercenaryAI(int kind) : state(nullptr), actionStarted(0), actionDuration(0), kind(kind), lastTick(0) {
}

// OFFSET: 0x00D9E430
// FUNCTION: fn_22328
CMercenaryAI::~CMercenaryAI() {
  if (this->state) {
    lua_close(this->state);
    this->state = nullptr;
  }
}

// OFFSET: 0x00D9E500
// FUNCTION: fn_22330
int ClampTargetAttackRange(std::uint32_t aid, int distance) {
  CGameActor* actor = static_cast<CGameActor*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(aid));
  if (actor) {
    CGameActor* target = static_cast<CGameActor*>(
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(&actor->unknown_0318[5]))
    );
    if (target) {
      return distance < target->Slot42() ? target->Slot42() : distance;
    }
  }
  return distance;
}

// OFFSET: 0x00D9E570
// FUNCTION: fn_22331
void CMercenaryAI::Shutdown() {
  if (this->state) {
    lua_close(this->state);
    this->state = nullptr;
  }
}
