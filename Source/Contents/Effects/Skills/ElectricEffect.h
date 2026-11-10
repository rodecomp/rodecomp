#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_ELECTRICEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_ELECTRICEFFECT_H

#include "Contents/Effects/Core/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <vector>

class CParticle;

class CElectricEffect : public CEffect {
public:
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  void AddBolt(int tilt);

  /* 00CC */ short angle;
  /* 00CE */ unsigned char padding_00CE[2];
  /* 00D0 */ vector3d path[2];
  /* 00E8 */ vector3d lastEdge[2];
  /* 0100 */ int tick;
  /* 0104 */ std::vector<CParticle*> bolts;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CElectricEffect, bolts) == 0x104);
#endif

#endif
