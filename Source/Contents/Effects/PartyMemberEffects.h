#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTYMEMBEREFFECTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTYMEMBEREFFECTS_H

#include <cstdint>

class CGameMode;

struct PartyMemberEffects {
  CGameMode* mode;

  bool Enabled() const;
  void EnsurePartyEffect(std::uint32_t accountId);
};

#endif
