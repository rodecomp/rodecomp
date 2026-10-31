#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_ARROWEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_ARROWEFFECT_H

#include "Contents/Actors/RenderObject.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>

class CArrowEffect : public CRenderObject {
public:
  CArrowEffect();

  bool OnProcess() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;

  void Reset();

  /* 0138 */ vector3d startPos;
  /* 0144 */ vector3d velocity;
  /* 0150 */ vector3d targetPos;
  /* 015C */ float speedScale;
  /* 0160 */ float startFrame;
  /* 0164 */ std::uint32_t targetAid;
  /* 0168 */ int frameCount;
  /* 016C */ int unknown_016C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CArrowEffect) == 0x170);
static_assert(offsetof(CArrowEffect, startPos) == 0x138);
static_assert(offsetof(CArrowEffect, speedScale) == 0x15C);
static_assert(offsetof(CArrowEffect, frameCount) == 0x168);
#endif

#endif
