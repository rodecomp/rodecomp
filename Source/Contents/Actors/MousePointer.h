#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_MOUSEPOINTER_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_MOUSEPOINTER_H

#include "Contents/Actors/RenderObject.h"

#include <cstddef>

class CMousePointer : public CRenderObject {
public:
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;

  /* 0138 */ int m_remainingFrames;
  /* 013C */ int unknown_013C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMousePointer) == 0x140);
static_assert(offsetof(CMousePointer, m_remainingFrames) == 0x138);
#endif

#endif
