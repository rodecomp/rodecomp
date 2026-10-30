#include "Contents/Effects/ShowEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/World/World.h"

// OFFSET: 0x00729400
// FUNCTION: fn_07441
CShowEffect::CShowEffect() : CEffect(0x947), effects() {
}

// OFFSET: 0x00729810
// FUNCTION: fn_07446
void CShowEffect::Add(int tag, int effectId) {
  const std::uint32_t actorId = this->actor;
  if (this->effects.find(tag) != this->effects.end() || g_effectMgr->Find(actorId, effectId)) {
    return;
  }

  if (!g_effectMgr->Spawn(actorId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, tag)) {
    if (!g_modeMgr.GetGameMode()) {
      return;
    }

    CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
    EffectActor* target;
    if (actorId == g_session.m_aid) {
      target = world->player;
    } else {
      target = static_cast<EffectActor*>(world->GetGameActorByAID(actorId));
    }

    if (!target || !target->LaunchTaggedEffect(effectId, tag)) {
      return;
    }
  }

  this->effects[tag] = effectId;
}

// OFFSET: 0x00729910
// FUNCTION: fn_07447
void CShowEffect::Close() {
  if (this->effects.empty()) {
    return;
  }

  const std::uint32_t actorId = this->actor;
  CRenderObject* target = nullptr;
  if (g_modeMgr.GetGameMode()) {
    CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
    if (world) {
      target = world->GetGameActorByAID(actorId);
    }
  }

  for (const auto& entry : this->effects) {
    if (target) {
      target->SendMsg(nullptr, 0x9F, 0, entry.first, entry.second, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    } else if (g_effectMgr->HasAllocator(entry.second)) {
      g_effectMgr->Retire(actorId, entry.first, entry.second);
    }
  }

  this->effects.clear();
}

// OFFSET: 0x00729A30
// FUNCTION: fn_07448
unsigned CShowEffect::Count() const {
  return this->effects.size();
}

// OFFSET: 0x00729A90
// FUNCTION: fn_07449
void CShowEffect::Remove(int tag) {
  if (this->effects.find(tag) == this->effects.end() || !g_modeMgr.GetGameMode()) {
    return;
  }

  CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  if (!world) {
    return;
  }

  CRenderObject* target = world->GetGameActorByAID(this->actor);
  if (!target) {
    return;
  }

  target->SendMsg(target, 0x9F, 0, tag, this->effects[tag], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->effects.erase(tag);
}
