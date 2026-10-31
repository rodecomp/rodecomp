#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/World/World.h"

// OFFSET: 0x00AC8260
// FUNCTION: fn_17668
CRenderObject* CWorld::GetGameActorByAID(std::uint32_t aid) const {
  for (CRenderObject* actor : this->actors) {
    if (actor->render.accountId == aid) {
      return actor;
    }
  }

  return nullptr;
}

// OFFSET: 0x00AC8470
// FUNCTION: fn_17672
void CWorld::CreatePlayer() {
  CPlayer* player = new CPlayer();
  this->player = player;
  this->objects.push_back(player);
}
