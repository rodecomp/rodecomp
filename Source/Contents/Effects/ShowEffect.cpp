#include "Contents/Effects/ShowEffect.h"
#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
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
}

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

  for (const auto& entry : this->effects) {
    if (target) {
      target->SendMsg(nullptr, AM_DELETE_TAGGED_EFFECT, 0, entry.first, entry.first >> 31, entry.second, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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

namespace {
  // INLINED
  bool HasEffect(const std::map<int, int>& effects, int tag) {
    return effects.find(tag) != effects.end();
  }
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

  target->SendMsg(target, AM_DELETE_TAGGED_EFFECT, 0, static_cast<int>(static_cast<std::int64_t>(tag)),
    static_cast<int>(static_cast<std::int64_t>(tag) >> 32), this->effects[tag], 0, 0, 0, 0, 0, 0, 0, 0, 0);
  this->effects.erase(tag);
}
