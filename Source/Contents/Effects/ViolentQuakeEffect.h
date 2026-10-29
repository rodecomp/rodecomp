#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_VIOLENTQUAKEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_VIOLENTQUAKEEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CEZ2STRParticle;

class CViolentQuakeEffect : public CEffect {
public:
  CViolentQuakeEffect();

  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ std::vector<CEZ2STRParticle*> emitters;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CViolentQuakeEffect) == 0xD8);
#endif

#endif
