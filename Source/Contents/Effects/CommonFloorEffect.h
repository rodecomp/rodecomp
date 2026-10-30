#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_COMMONFLOOREFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_COMMONFLOOREFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <string>
#include <vector>

class CParticle;

class CCommonFloorEffect : public CEffect {
public:
  CCommonFloorEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void Slot07() override;
  void Slot08(int red, int green, int blue) override;
  void Slot11(std::string name) override;

  /* 00CC */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCommonFloorEffect) == 0xD8);
static_assert(offsetof(CCommonFloorEffect, particles) == 0xCC);
#endif

#endif
