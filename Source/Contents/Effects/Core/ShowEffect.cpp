#include "Contents/Effects/Core/ShowEffect.h"
#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/World/World.h"

namespace {
  // INLINED
  CRenderObject* FindActor(std::uint32_t actorId) {
    if (!g_modeMgr.GetGameMode()) {
      return nullptr;
    }

    CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
    if (!world) {
      return nullptr;
    }

    return world->GetGameActorByAID(actorId);
  }

  // INLINED
  bool HasEffect(const std::map<int, int>& effects, int tag) {
    return effects.find(tag) != effects.end();
  }
}

// OFFSET: 0x00729400
// FUNCTION: fn_07441
CShowEffect::CShowEffect() : CEffect(0x947), effects() {
}

// OFFSET: 0x00729810
// FUNCTION: fn_07446
void CShowEffect::Add(int tag, int effectId) {
  const std::uint32_t actorId = this->actor;
  if (HasEffect(this->effects, tag) || g_effectMgr->Find(actorId, effectId)) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  if (!effects->Spawn(actorId, effectId, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, tag)) {
    if (!g_modeMgr.GetGameMode()) {
      return;
    }

    CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
    if (actorId == g_session.m_aid) {
      EffectActor* player = world->player;
      if (!player || !player->LaunchTaggedEffect(effectId, tag)) {
        return;
      }
    } else {
      EffectActor* target = static_cast<EffectActor*>(world->GetGameActorByAID(actorId));
      if (!target || !target->LaunchTaggedEffect(effectId, tag)) {
        return;
      }
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
  CRenderObject* target = FindActor(actorId);

  for (auto it = this->effects.begin(); it != this->effects.end(); ++it) {
    if (target) {
      target->SendMsg(nullptr, AM_DELETE_TAGGED_EFFECT, it->first, static_cast<std::uint32_t>(it->second), 0, 0, 0, 0);
    } else if (g_effectMgr->HasAllocator(it->second)) {
      g_effectMgr->Retire(actorId, it->first, it->second);
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
  if (!HasEffect(this->effects, tag)) {
    return;
  }

  CRenderObject* target = FindActor(this->actor);
  if (!target) {
    return;
  }

  target->SendMsg(target, AM_DELETE_TAGGED_EFFECT, tag, static_cast<std::uint32_t>(this->effects[tag]), 0, 0, 0, 0);
  this->effects.erase(tag);
}
