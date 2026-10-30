#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_TALISMANMARKEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_TALISMANMARKEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CEZ2STRParticle;

class CTalismanMarkEffect : public CEffect {
public:
  /* 00CC */ std::vector<CEZ2STRParticle*> particles;

  CTalismanMarkEffect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  void PlayEnd();
  void UpdateParticle(CEZ2STRParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CTalismanMarkEffect, particles) == 0xCC);
static_assert(sizeof(CTalismanMarkEffect) == 0xD8);
#endif

#endif
