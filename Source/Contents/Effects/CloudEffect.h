#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CLOUDEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CLOUDEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>

class CParticle;

class CCloud : public CEffect {
public:
  CCloud();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void Slot07() override;
  void Drift();

  /* 00CC */ CParticle* particle;
  /* 00D0 */ int driftX;
  /* 00D4 */ int driftTargetX;
  /* 00D8 */ int driftZ;
  /* 00DC */ int driftTargetZ;
  /* 00E0 */ int phase;
  /* 00E4 */ float baseScale;
  /* 00E8 */ float alpha;
  /* 00EC */ int fadeInEnd;
  /* 00F0 */ int fadeOutStart;
  /* 00F4 */ unsigned char red;
  /* 00F5 */ unsigned char green;
  /* 00F6 */ unsigned char blue;
  /* 00F8 */ float scaleBase;
  /* 00FC */ float scaleRange;
  /* 0100 */ float scaleAmplitude;
  /* 0104 */ float fadeInStep;
  /* 0108 */ float fadeOutStep;
  /* 010C */ int fadeInBase;
  /* 0110 */ int fadeInRange;
  /* 0114 */ int fadeOutBase;
  /* 0118 */ int fadeOutRange;
  /* 011C */ float offsetRangeY;
  /* 0120 */ float offsetRangeX;
  /* 0124 */ float offsetRangeZ;
  /* 0128 */ unsigned char renderFlag;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CCloud, particle) == 0xCC);
static_assert(offsetof(CCloud, red) == 0xF4);
static_assert(offsetof(CCloud, renderFlag) == 0x128);
#endif

#endif
