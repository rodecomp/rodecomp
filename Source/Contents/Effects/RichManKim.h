#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_RICHMANKIM_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_RICHMANKIM_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CParticle;

class CRichManKim : public CEffect {
public:
  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ unsigned char unknown_00CC[0x38];
  /* 0104 */ int angleIndex;
  /* 0108 */ std::vector<CParticle*> particles;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRichManKim) == 0x114);
static_assert(offsetof(CRichManKim, angleIndex) == 0x104);
static_assert(offsetof(CRichManKim, particles) == 0x108);
#endif

#endif
