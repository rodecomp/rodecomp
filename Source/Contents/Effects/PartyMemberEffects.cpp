#include "Contents/Effects/RagEffect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/PartyMemberEffects.h"
#include "Contents/Effects/RagEffectInitialization.h"
#include "Script/LuaScript.h"
#include "Utility/Math/AffineMatrix.h"

namespace {
  // OFFSET: 0x00CADD60
  // FUNCTION: fn_21069
  CRagEffect* CreatePartyEffect(EffectActor& actor, PartyEffectMode& mode) {
    mode.lastEffectId = 160;
    auto* effect = new CRagEffect();
    bool force = false;
    mode.effectScript.Call("IsForceRenderEffect", "d>b", 160, &force);
    effect->state.forceRender = force;

    const float sine = ClientSine(0);
    const float cosine = ClientCosine(0);
    const float x = 0.0f * cosine - 0.0f * sine;
    const float z = 0.0f * cosine + 0.0f * sine;
    InitializeRagEffect(effect->state, effect->render, &actor, actor.render, 160, x, 0, z, mode.options, mode.resources);
    mode.objects.push_back(effect);
    actor.state.effects.push_back(&effect->state);

    return effect;
  }
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
        CreatePartyEffect(*actor, mode);
      }

      return;
    }
  }
}
