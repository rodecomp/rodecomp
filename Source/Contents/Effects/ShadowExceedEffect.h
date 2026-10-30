#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SHADOWEXCEEDEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SHADOWEXCEEDEFFECT_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <vector>

class CEZ2STRParticle;

class CShadowExceed : public CEffect {
public:
  CShadowExceed();
  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ std::vector<CEZ2STRParticle*> emitters;
  /* 00D8 */ unsigned char unknown_00D8;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CShadowExceed) == 0xDC);
static_assert(offsetof(CShadowExceed, unknown_00D8) == 0xD8);
#endif

#endif
