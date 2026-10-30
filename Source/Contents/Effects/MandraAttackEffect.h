#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_MANDRAATTACKEFFECT_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_MANDRAATTACKEFFECT_H

#include "Contents/Actors/RenderObject.h"

#include <cstddef>

class CMandraAttackEffect : public CRenderObject {
public:
  CMandraAttackEffect();
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;

  /* 0138 */ float startFrame;
  /* 013C */ int unknown_013C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMandraAttackEffect) == 0x140);
static_assert(offsetof(CMandraAttackEffect, startFrame) == 0x138);
#endif

#endif
