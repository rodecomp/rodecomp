#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_CARTCANNON_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_CARTCANNON_H

#include "Contents/Effects/Core/Effect.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>

class CCartCannon : public CEffect {
public:
  CCartCannon();

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  void SetDelay(int delay) override;

  /* 00CC */ CEZ2STRParticle* particle;
  /* 00D0 */ int currentStep;
  /* 00D4 */ int delay;
  /* 00D8 */ float speed;
  /* 00DC */ vector3d currentPosition;
  /* 00E8 */ vector3d stepDelta;
  /* 00F4 */ vector3d startPosition;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCartCannon) == 0x100);
static_assert(offsetof(CCartCannon, particle) == 0xCC);
static_assert(offsetof(CCartCannon, currentStep) == 0xD0);
static_assert(offsetof(CCartCannon, delay) == 0xD4);
static_assert(offsetof(CCartCannon, speed) == 0xD8);
static_assert(offsetof(CCartCannon, currentPosition) == 0xDC);
static_assert(offsetof(CCartCannon, stepDelta) == 0xE8);
static_assert(offsetof(CCartCannon, startPosition) == 0xF4);
#endif

#endif
