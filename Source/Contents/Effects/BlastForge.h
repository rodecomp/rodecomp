#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_BLASTFORGE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_BLASTFORGE_H

#include "Contents/Effects/Effect.h"

#include <cstddef>
#include <cstdint>

struct EZ2STREmitter;

class CBlastForge : public CEffect {
public:
  CBlastForge();

  /* 00CC */ EZ2STREmitter* unknown_00CC;
  /* 00D0 */ EZ2STREmitter* unknown_00D0;
  /* 00D4 */ EZ2STREmitter* unknown_00D4;
  /* 00D8 */ EZ2STREmitter* unknown_00D8;
  /* 00DC */ unsigned char unknown_00DC;
  /* 00DD */ unsigned char unknown_00DD[3];
  /* 00E0 */ std::int64_t startTime;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CBlastForge) == 0xE8);
static_assert(offsetof(CBlastForge, unknown_00DC) == 0xDC);
static_assert(offsetof(CBlastForge, startTime) == 0xE0);
#endif

#endif
