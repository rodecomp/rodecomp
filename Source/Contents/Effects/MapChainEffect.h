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
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;
  void ResetParticles() override;

  void CreateChainParticles(int param);

  /* 00CC */ MapChainPoint corners[2];
  /* 00E4 */ std::vector<CParticle*> particles;
  /* 00F0 */ int chainModeA;
  /* 00F4 */ int chainModeB;
  /* 00F8 */ int chainModeC;
  /* 00FC */ int chainModeD;
  /* 0100 */ int chainModeE;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMapChainEffect, particles) == 0xE4);
static_assert(sizeof(CMapChainEffect) == 0x104);
#endif

#endif
