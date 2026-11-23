#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/World/World.h"
#include "UI/Gauges/UIPcGage.h"

// OFFSET: 0x00AC8260
// FUNCTION: fn_17668
CGameActor* CWorld::GetGameActorByAID(std::uint32_t aid) const {
  for (auto it = this->actors.begin(); it != this->actors.end(); ++it) {
    if ((*it)->render.accountId == aid) {
      return *it;
    }
  }

  return nullptr;
}

// OFFSET: 0x00AC8290
// FUNCTION: fn_46298
ActorHealth CWorld::GetActorHealth(std::uint32_t aid) const {
  int maximum;
  int current;
  if (aid == g_session.m_aid) {
    maximum = g_session.maxHp;
    current = g_session.hp;
  } else {
    const CPc* actor = static_cast<const CPc*>(this->GetGameActorByAID(aid));
    if (!actor) {
      return {0, 0, false};
    }
    const UIPcGage* gauge = actor->gaugeWindow;
    if (gauge) {
      maximum = gauge->maximum;
      current = gauge->current;
    } else {
      maximum = 0;
      current = 0;
    }
  }
  return {maximum, current, true};
}

#pragma inline_depth(0)
// OFFSET: 0x00AC8470
// FUNCTION: fn_17672
void CWorld::CreatePlayer() {
  CPlayer* player = new CPlayer();
  this->player = player;
  this->objects.push_back(player);
}
#pragma inline_depth()
