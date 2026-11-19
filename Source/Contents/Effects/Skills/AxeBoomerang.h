#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_AXEBOOMERANG_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_SKILLS_AXEBOOMERANG_H

#include "Contents/Effects/Core/Effect.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>

struct CEZ2STRParticle;

class CAxeBoomerang : public CEffect {
public:
  CAxeBoomerang();
  ~CAxeBoomerang() override;

  void Open() override;
  void OnProcess() override;
  void Render() override;
  void Close() override;
  bool IsAlive() override;

  /* 00CC */ CEZ2STRParticle* emitter;
  /* 00D0 */ unsigned char alive;
  /* 00D1 */ unsigned char returning;
  /* 00D2 */ unsigned char padding_00D2[2];
  /* 00D4 */ int processCount;
  /* 00D8 */ float trajectoryTime;
  /* 00DC */ vector3d particlePosition;
  /* 00E8 */ vector3d launchPosition;

private:
  vector3d EvaluateTrajectory(vector3d start, vector3d control, vector3d end, float t);
  void UpdateParticle(CEZ2STRParticle* emitter);
  void RenderParticle(CEZ2STRParticle* particle);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CAxeBoomerang) == 0xF4);
static_assert(offsetof(CAxeBoomerang, alive) == 0xD0);
static_assert(offsetof(CAxeBoomerang, launchPosition) == 0xE8);
#endif

#endif
