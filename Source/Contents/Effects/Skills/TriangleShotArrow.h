#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_TRIANGLESHOTARROW_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_TRIANGLESHOTARROW_H

#include "Contents/Effects/Core/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>

class CEZ2STRParticle;

class CTriangleShotArrow : public CEffect {
public:
  CTriangleShotArrow();

  void Open() override;
  void OnProcess() override;
  void Close() override;
  void Render() override;
  bool IsAlive() override;
  void SetDelay(int delay) override;

  /* 00CC */ CEZ2STRParticle* arrow;
  /* 00D0 */ int step;
  /* 00D4 */ int delay;
  /* 00D8 */ float speed;
  /* 00DC */ vector3d current;
  /* 00E8 */ vector3d direction;
  /* 00F4 */ vector3d origin;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CTriangleShotArrow) == 0x100);
static_assert(offsetof(CTriangleShotArrow, origin) == 0xF4);
#endif

#endif
