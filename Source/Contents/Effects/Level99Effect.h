#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_LEVEL99EFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_LEVEL99EFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <string>
#include <vector>

class CParticle;

class CLevel99Effect : public CEffect {
public:
  CLevel99Effect();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void Slot08(int red, int green, int blue) override;
  void Slot11(std::string name) override;

  void AddRings();

  /* 00CC */ vector3d unknown_00CC[2] = {};
  /* 00E4 */ std::vector<CParticle*> sparkles;
  /* 00F0 */ std::vector<CParticle*> sprites;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CLevel99Effect, unknown_00CC) == 0xCC);
static_assert(offsetof(CLevel99Effect, sparkles) == 0xE4);
static_assert(offsetof(CLevel99Effect, sprites) == 0xF0);
#endif

#endif
