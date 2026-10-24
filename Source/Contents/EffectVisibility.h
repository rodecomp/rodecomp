#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTVISIBILITY_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTVISIBILITY_H

#include "Contents/MessengerMember.h"

#include <list>

struct RagEffectState;
struct ServiceOptions;

struct EffectVisibilityContext {
  const ServiceOptions& options;
  const std::uint32_t& localAccountId;
  const bool& partyVisible;
  const std::list<MessengerMember>& party;
};

// Id-160 path. Other ids may consult additional actors.
bool PartyEffectVisible(const RagEffectState& effect, const EffectVisibilityContext& context);

#endif
