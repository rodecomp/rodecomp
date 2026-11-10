#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_FORTUNEKISSEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_FORTUNEKISSEFFECT_H

#include "Contents/Effects/Core/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CFortuneKiss : public CEffect {
public:
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ unsigned char padding_00CC[0x38];
  /* 0104 */ int angle;
  /* 0108 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CFortuneKiss) == 0x114);
static_assert(offsetof(CFortuneKiss, particles) == 0x108);
#endif

#endif
