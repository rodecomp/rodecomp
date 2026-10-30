#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ACIDIFIEDTHROW_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ACIDIFIEDTHROW_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>

struct EZ2STREmitter;

class CAcidifiedThrow : public CEffect {
public:
  CAcidifiedThrow();

  void Slot01() override;
  void Slot03() override;
  void Slot04() override;
  void Close() override;
  bool Slot06() override;

  /* 00CC */ EZ2STREmitter* unknown_00CC;
  /* 00D0 */ EZ2STREmitter* unknown_00D0[3];
  /* 00DC */ bool active;
  /* 00DD */ unsigned char unknown_00DD[3];
  /* 00E0 */ float unknown_00E0;
  /* 00E4 */ vector3d unknown_00E4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAcidifiedThrow) == 0xF0);
static_assert(offsetof(CAcidifiedThrow, active) == 0xDC);
static_assert(offsetof(CAcidifiedThrow, unknown_00E4) == 0xE4);
#endif

#endif
