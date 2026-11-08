#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"

// OFFSET: 0x00CD6DD0
// FUNCTION: fn_21289
void CGameMode::RefreshActorJobEffects(bool playerOnly) {
  this->scene->player->ResetJobEffects();
  this->scene->player->UpdateMaxLevelAura();
  if (!playerOnly) {
    for (std::list<CRenderObject*>::iterator it = this->scene->actors.begin(); it != this->scene->actors.end(); ++it) {
      CGameActor* actor = static_cast<CGameActor*>(*it);
      if (actor->hasLevelAura != 0) {
        actor->ResetJobEffects();
        actor->UpdateMaxLevelAura();
      }
    }
  }
}
