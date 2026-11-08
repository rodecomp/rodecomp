#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ACIDIFIEDTHROW_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ACIDIFIEDTHROW_H

#include "Contents/Effects/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>

struct EZ2STREmitter;

class CAcidifiedThrow : public CEffect {
public:
  CAcidifiedThrow();

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ EZ2STREmitter* projectileEmitter;
  /* 00D0 */ EZ2STREmitter* impactEmitters[3];
  /* 00DC */ bool active;
  /* 00DD */ unsigned char unknown_00DD[3];
  /* 00E0 */ float trajectoryTime;
  /* 00E4 */ vector3d impactPosition;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAcidifiedThrow) == 0xF0);
static_assert(offsetof(CAcidifiedThrow, active) == 0xDC);
static_assert(offsetof(CAcidifiedThrow, impactPosition) == 0xE4);
#endif

#endif
