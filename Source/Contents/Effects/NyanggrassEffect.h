#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_NYANGGRASSEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_NYANGGRASSEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Contents/Effects/Particle.h"
#include "Render/Primitives/LightmappedPrimitive.h"

#include <cstddef>
#include <vector>

class CMultiTexParticle : public CParticle {
public:
  CMultiTexParticle(vector3d position, int flags);
  ~CMultiTexParticle() override;
  void Slot01() override;

  void Reset();
  void Render(int column, int row);

  /* 02BC */ LightmappedCommand command;
  /* 02D8 */ CTexture* texture1;
  /* 02DC */ CTexture* texture2;
  /* 02E0 */ float uv1[4][2];
  /* 0300 */ float uv2[4][2];
  /* 0320 */ float offsetU;
  /* 0324 */ float offsetV;
};

class CNyanggrassEffect : public CEffect {
public:
  CNyanggrassEffect();

  /* 00CC */ std::vector<CParticle*> particles;
  /* 00D8 */ int unknown_00D8;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMultiTexParticle) == 0x328);
static_assert(offsetof(CMultiTexParticle, command) == 0x2BC);
static_assert(offsetof(CMultiTexParticle, texture1) == 0x2D8);
static_assert(offsetof(CMultiTexParticle, texture2) == 0x2DC);
static_assert(offsetof(CMultiTexParticle, uv1) == 0x2E0);
static_assert(offsetof(CMultiTexParticle, uv2) == 0x300);
static_assert(offsetof(CMultiTexParticle, offsetU) == 0x320);
static_assert(offsetof(CMultiTexParticle, offsetV) == 0x324);
static_assert(sizeof(CNyanggrassEffect) == 0xDC);
static_assert(offsetof(CNyanggrassEffect, unknown_00D8) == 0xD8);
#endif

#endif
