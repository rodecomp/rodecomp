#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_HEALEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_HEALEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CHealEffect : public CEffect {
public:
  CHealEffect();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;
  void Slot12() override;

  void AddSparks(int kind);
  void AddColumn(int kind);
  void AddCone(int kind);
  void SpawnParticles();
  void sub_B67F90(CParticle* particle);
  void sub_B69E30(vector3d* points);

  /* 00CC */ vector3d columnEdge[2];
  /* 00E4 */ vector3d coneEdge[2];
  /* 00FC */ int columnLifetime;
  /* 0100 */ int coneLifetime;
  /* 0104 */ int spawnTick;
  /* 0108 */ std::vector<CParticle*> columnParticles;
  /* 0114 */ std::vector<CParticle*> coneParticles;
  /* 0120 */ std::vector<CParticle*> sparkParticles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CHealEffect) == 0x12C);
static_assert(offsetof(CHealEffect, columnEdge) == 0xCC);
static_assert(offsetof(CHealEffect, coneEdge) == 0xE4);
static_assert(offsetof(CHealEffect, columnLifetime) == 0xFC);
static_assert(offsetof(CHealEffect, spawnTick) == 0x104);
static_assert(offsetof(CHealEffect, columnParticles) == 0x108);
static_assert(offsetof(CHealEffect, coneParticles) == 0x114);
static_assert(offsetof(CHealEffect, sparkParticles) == 0x120);
#endif

#endif
