#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Actors/GameActor.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Effects/Core/RagEffectState.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/World/World.h"

// INLINED
static inline bool ActorEffectVisible(const EffectActor* actor, bool restricted) {
  if (!actor) {
    return false;
  }

  if (actor->render.isCharacter == 0) {
    return true;
  }

  if (!restricted) {
    if (actor->render.accountId == g_session.m_aid) {
      return true;
    }

    if (g_session.IsPvpMap() && g_session.IsPartyMember(actor->render.accountId, true)) {
      return true;
    }
  }

  return actor->EffectVisible();
}

// OFFSET: 0x00C734B0
// FUNCTION: fn_20327
bool CRagEffect::PartyEffectVisible() {
  if (g_serviceOptions.Get(TT_EFFECT_ON_OFF) == 0 && this->state.forceRender == 0) {
    return false;
  }

  if (this->state.type == EF_LINELINK) {
    CWorld* scene = g_modeMgr.GetGameMode()->scene;
    const EffectActor* target = static_cast<const EffectActor*>(scene->GetGameActorByAidIncludeMe(static_cast<std::uint32_t>(this->state.deltaPos.y)));

    if (!ActorEffectVisible(target, false)) {
      return false;
    }
  }

  return ActorEffectVisible(this->state.parent, this->state.visibilityFlags != 0);
}
