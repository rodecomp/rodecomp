#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_AXEBOOMERANG_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_AXEBOOMERANG_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>

struct EZ2STREmitter;

class CAxeBoomerang : public CEffect {
public:
  CAxeBoomerang();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ EZ2STREmitter* unknown_00CC;
  /* 00D0 */ unsigned char unknown_00D0;
  /* 00D1 */ unsigned char unknown_00D1;
  /* 00D2 */ unsigned char unknown_00D2[2];
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ float unknown_00D8;
  /* 00DC */ vector3d unknown_00DC;
  /* 00E8 */ vector3d unknown_00E8;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAxeBoomerang) == 0xF4);
static_assert(offsetof(CAxeBoomerang, unknown_00D0) == 0xD0);
static_assert(offsetof(CAxeBoomerang, unknown_00E8) == 0xE8);
#endif

#endif
