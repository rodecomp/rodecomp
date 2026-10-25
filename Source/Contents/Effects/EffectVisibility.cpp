#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectVisibility.h"
#include "Contents/Effects/RagEffectState.h"
#include "Contents/Configuration/ServiceOptions.h"

// OFFSET: 0x00C734B0
// FUNCTION: fn_20327
bool PartyEffectVisible(const RagEffectState& effect, const EffectVisibilityContext& context) {
  if (context.options.Get(8) == 0 && effect.forceRender == 0) {
    return false;
  }

  const EffectActor* actor = effect.parent;

  if (!actor) {
    return false;
  }

  if (actor->render.unknown_00A8 == 0) {
    return true;
  }

  if (effect.unknown_11CA0 == 0) {
    if (actor->render.accountId == context.localAccountId) {
      return true;
    }

    if (context.partyVisible) {
      for (const MessengerMember& member : context.party) {
        if (member.accountId == actor->render.accountId) {
          if (member.offline == 0) {
            return true;
          }

          break;
        }
      }
    }
  }

  return actor->render.visible != 0 && actor->render.enabled != 0;
}
