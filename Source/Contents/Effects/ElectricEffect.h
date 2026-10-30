#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ELECTRICEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ELECTRICEFFECT_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CElectricEffect : public CEffect {
public:
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  void AddBolt(int tilt);

  /* 00CC */ short angle;
  /* 00CE */ unsigned char unknown_00CE[2];
  /* 00D0 */ vector3d path[2];
  /* 00E8 */ vector3d lastEdge[2];
  /* 0100 */ int tick;
  /* 0104 */ std::vector<CParticle*> bolts;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CElectricEffect, bolts) == 0x104);
#endif

#endif
