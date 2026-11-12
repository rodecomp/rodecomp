#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_MOUSEPOINTER_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_MOUSEPOINTER_H

#include "Contents/Actors/RenderObject.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/SpriteResource.h"

#include <cstddef>

class CMousePointer : public CRenderObject {
public:
  CMousePointer();
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) override;

  /* 0138 */ int m_remainingFrames;
  /* 013C */ int padding_013C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMousePointer) == 0x140);
static_assert(offsetof(CMousePointer, m_remainingFrames) == 0x138);
#endif

#endif
