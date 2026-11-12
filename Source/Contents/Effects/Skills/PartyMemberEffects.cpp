#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Skills/PartyMemberEffects.h"
#include "Client/Application/LaunchInfo.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/GroundScope.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Script/LuaScript.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

template __declspec(noinline) std::_Tree_find_result<std::map<int, std::int64_t>::_Mybase::_Nodeptr> std::map<int, std::int64_t>::_Mybase::_Find_lower_bound<int>(const int&) const;

namespace {
  struct CRollingCutterRagEffect : CRagEffect {
    __declspec(noinline) CRollingCutterRagEffect();

    bool OnProcess() override;
    int GetDuration() override;

    /* 11CA8 */ int duration;
    /* 11CAC */ int elapsed;
  };

  struct CWhirlwindEffect : CRagEffect {
    CWhirlwindEffect() : duration(250), first(0), second(0) {
    }

    /* 11CA8 */ int duration;
    /* 11CAC */ int padding_11CAC;
    /* 11CB0 */ int first;
    /* 11CB4 */ int second;
  };

  void RecordLaunch(int effectId) {
    std::uint32_t recent[8];
    recent[0] = effectId;
    std::memcpy(recent + 1, g_launchInfo, sizeof(recent) - sizeof(recent[0]));
    std::memcpy(g_launchInfo, recent, sizeof(recent));
  }
}

// OFFSET: 0x00611ED0
// FUNCTION: fn_04336
CRollingCutterRagEffect::CRollingCutterRagEffect() : duration(60), elapsed(0) {
}

// OFFSET: 0x00612000
// FUNCTION: fn_04338
int CRollingCutterRagEffect::GetDuration() {
  return this->duration;
}

// OFFSET: 0x00612010
// FUNCTION: fn_04339
std::uint32_t CRagEffect::GetStateFlags() {
  return 0;
}

// OFFSET: 0x00612500
// FUNCTION: fn_04341
bool CRollingCutterRagEffect::OnProcess() {
  if (!this->state.parent) {
    return false;
  }

  if (g_updateTick > 0 && this->state.parent->render.effectPhaseBytes[0] == 1) {
    this->state.parent->render.flags = 0;
    this->state.parent->render.effectPhaseBytes[0] = 0;
  }

  const vector3d& parentPosition = this->state.parent->render.position;
  this->render.position.x = parentPosition.x;
  this->render.position.y = parentPosition.y;
  this->render.position.z = parentPosition.z;

  if (this->elapsed == 0) {
    const vector3d& player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->render.position;
    const vector3d soundPosition = {this->render.position.x - player.x, -100.0f, this->render.position.z - player.z};
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", soundPosition.x, soundPosition.y, soundPosition.z, 250, 40, 1.0f, 0);
  }

  this->state.parent->render.angle += 12.0f;
  if (this->state.parent->render.angle >= 360.0f) {
    this->state.parent->render.angle -= 360.0f;
  }

  ++this->elapsed;
  if (this->elapsed >= this->GetDuration()) {
    CGameActor* actor = dynamic_cast<CGameActor*>(static_cast<CRenderObject*>(this->state.parent));
    if (actor) {
      actor->render.instant.Values()[1] = static_cast<int>(actor->GetInstant(1)) - 1;
      if (static_cast<int>(actor->GetInstant(1)) <= 0) {
        actor->render.angle = static_cast<int>(actor->GetInstant(0)) / 1000.0f;
      }
    }

    return false;
  }

  return true;
}

// OFFSET: 0x00CADD60
// FUNCTION: fn_21069
CRagEffect* EffectActor::LaunchEffect(int effectId, vector3d deltaPos, float rotation) {
  CRagEffect* effect;

  if (effectId == 127) {
    this->state.effectLaunched = 1;
  } else if (effectId < 0 || effectId > 60000) {
    return nullptr;
  }

  RecordLaunch(effectId);

  switch (effectId) {
  case 837:
    effect = new CRollingCutterRagEffect();
    break;
  case 2444:
    effect = new CWhirlwindEffect();
    break;
  default:
    effect = new CRagEffect();
    break;
  }

  if (!effect) {
    return nullptr;
  }

  bool force = false;
  g_session.m_hotkeyScript->Call("IsForceRenderEffect", "d>b", effectId, &force);
  effect->state.forceRender = force;

  const int degrees = static_cast<int>(rotation);
  const float sine = ClientSine(degrees);
  const float cosine = ClientCosine(degrees);
  const float x = deltaPos.x * cosine - deltaPos.z * sine;
  const float z = deltaPos.z * cosine + deltaPos.x * sine;
  deltaPos.x = x;
  deltaPos.z = z;

  if (effectId == 166 && this->render.isCharacter == 0) {
    return nullptr;
  }

  if (!this) {
    return nullptr;
  }

  effect->Init(this, effectId, deltaPos);
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->objects.push_back(effect);
  this->state.effects.push_back(effect);
  return effect;
}

// OFFSET: 0x00CAE0F0
// FUNCTION: fn_21070
CRagEffect* EffectActor::LaunchEffect(int effectId, int duration) {
  CRagEffect* effect = this->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  if (effect) {
    effect->SendMsg(nullptr, AM_SET_DURATION, 0, duration, 0, 0, 0, 0);
  }

  return effect;
}

// OFFSET: 0x00CAE160
// FUNCTION: fn_21071
CRagEffect* EffectActor::LaunchTaggedEffect(int effectId, std::uint32_t tag) {
  CRagEffect* effect = this->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  if (effect) {
    effect->state.tag = tag;
  }

  return effect;
}

// INLINED
bool PartyMemberEffects::Enabled() const {
  return this->mode && (this->mode->scene->isPKZone != 0 || this->mode->scene->isEventPVPMode != 0);
}

// INLINED
void PartyMemberEffects::EnsurePartyEffect(std::uint32_t accountId) {
  EffectActor* actor = static_cast<EffectActor*>(this->mode->scene->GetGameActorByAID(accountId));
  if (actor && !actor->HasId(160)) {
    actor->LaunchEffect(160, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

