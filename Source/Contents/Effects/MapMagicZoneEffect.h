#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MAPMAGICZONEEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MAPMAGICZONEEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CMapMagicZoneEffect : public CEffect {
public:
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;
  __declspec(noinline) void UpdateSpinner(CParticle* particle);

  /* 00CC */ vector3d previousInner;
  /* 00D8 */ vector3d previousOuter;
  /* 00E4 */ std::vector<CParticle*> spinners;
  /* 00F0 */ std::vector<CParticle*> petals;
  /* 00FC */ std::vector<CParticle*> ribbon;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMapMagicZoneEffect, previousInner) == 0xCC);
static_assert(offsetof(CMapMagicZoneEffect, previousOuter) == 0xD8);
static_assert(offsetof(CMapMagicZoneEffect, spinners) == 0xE4);
static_assert(offsetof(CMapMagicZoneEffect, petals) == 0xF0);
static_assert(offsetof(CMapMagicZoneEffect, ribbon) == 0xFC);
static_assert(sizeof(CMapMagicZoneEffect) == 0x108);
#endif

#endif
