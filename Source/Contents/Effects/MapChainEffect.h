#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MAPCHAINEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MAPCHAINEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

struct MapChainPoint {
  MapChainPoint() : x(0), y(0), z(0) {
  }

  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ float z;
};

class CMapChainEffect : public CEffect {
public:
  CMapChainEffect();
  void Slot03() override;
  void Slot04() override;
  bool Slot06() override;

  /* 00CC */ MapChainPoint corners[2];
  /* 00E4 */ std::vector<CParticle*> particles;
  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ int unknown_00F4;
  /* 00F8 */ int unknown_00F8;
  /* 00FC */ int unknown_00FC;
  /* 0100 */ int unknown_0100;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMapChainEffect, particles) == 0xE4);
static_assert(sizeof(CMapChainEffect) == 0x104);
#endif

#endif
