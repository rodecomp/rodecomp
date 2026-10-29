#include "Contents/Effects/RagEffect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/PartyMemberEffects.h"
#include "Client/Application/LaunchInfo.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/World/World.h"
#include "Script/LuaScript.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

namespace {
  struct CRollingCutterRagEffect : CRagEffect {
    CRollingCutterRagEffect() : duration(60), elapsed(0) {
    }

    /* 11CA8 */ int duration;
    /* 11CAC */ int elapsed;
  };

  struct CWhirlwindEffect : CRagEffect {
    CWhirlwindEffect() : duration(250), first(0), second(0) {
    }

    /* 11CA8 */ int duration;
    /* 11CAC */ int unknown_11CAC;
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

// OFFSET: 0x00CADD60
// FUNCTION: fn_21069
CRagEffect* EffectActor::LaunchEffect(int effectId, vector3d deltaPos, float rotation) {
  CRagEffect* effect;

  if (effectId == 127) {
    this->state.unknown_0140 = 1;
    RecordLaunch(effectId);
    effect = new CRagEffect();
  } else {
    if (effectId < 0 || effectId > 60000) {
      return nullptr;
    }

    RecordLaunch(effectId);

    if (effectId == 837) {
      effect = new CRollingCutterRagEffect();
    } else if (effectId == 2444) {
      effect = new CWhirlwindEffect();
    } else {
      effect = new CRagEffect();
    }
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

  if (effectId == 166 && this->render.unknown_00A8 == 0) {
    return nullptr;
  }

  if (!this) {
    return nullptr;
  }

  effect->Init(this, effectId, deltaPos);
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->objects.push_back(effect);
  this->state.effects.push_back(&effect->state);
  return effect;
}

// OFFSET: 0x00CAE0F0
// FUNCTION: fn_21070
CRagEffect* EffectActor::LaunchEffect(int effectId, int duration) {
  CRagEffect* effect = this->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  if (effect) {
    effect->SendMsg(nullptr, 80, 0, duration, 0, 0, 0, 0);
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
  const auto* mode = this->activeMode;
  return mode && (mode->playerOption48 != 0 || mode->playerOption54 != 0);
}

// INLINED
void PartyMemberEffects::EnsurePartyEffect(std::uint32_t accountId) {
  PartyEffectMode& mode = *this->activeMode;

  for (EffectActor* actor : mode.actors) {
    if (actor->render.accountId == accountId) {
      if (!actor->state.Attachments().HasId(160)) {
        actor->LaunchEffect(160, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }

      return;
    }
  }
}
