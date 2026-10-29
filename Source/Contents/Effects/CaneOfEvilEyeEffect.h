#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CANEOFEVILEYEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CANEOFEVILEYEEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/Particle.h"

#include <cstddef>
#include <cstdint>
#include <vector>

class CCaneOfEvilEyeEffect : public CEffect {
public:
  CCaneOfEvilEyeEffect();

  void Slot04() override;
  bool Slot06() override;

  void RemoveNullEmitters();

  /* 00CC */ std::uint32_t startTime;
  /* 00D0 */ std::vector<CEZ2STRParticle*> unknown_00D0;
  /* 00DC */ std::vector<CEZ2STRParticle*> unknown_00DC;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCaneOfEvilEyeEffect) == 0xE8);
static_assert(offsetof(CCaneOfEvilEyeEffect, unknown_00D0) == 0xD0);
static_assert(offsetof(CCaneOfEvilEyeEffect, unknown_00DC) == 0xDC);
#endif

#endif
