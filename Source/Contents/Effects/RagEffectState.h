#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECTSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECTSTATE_H

#include "Resource/EZeffectResource.h"

#include <cstddef>
#include <list>

class CRes;
struct EffectPrimitiveState;
struct EffectActor;

struct RagEffectState {
  RagEffectState();
  ~RagEffectState();

  /* 00000 */ EffectActor* parent;
  /* 00004 */ std::uint32_t unknown_013C[9];
  /* 00028 */ std::uint32_t unknown_0160;
  /* 0002C */ std::uint32_t unknown_0164;
  /* 00030 */ int id;
  /* 00034 */ int unknown_016C;
  /* 00038 */ std::uint32_t unknown_0170;
  /* 0003C */ std::uint32_t unknown_0174;
  /* 00040 */ std::int32_t unknown_0178;
  /* 00044 */ int lifetime;
  /* 00048 */ float offset[3];
  /* 00054 */ float unknown_018C[6];
  /* 0006C */ std::list<EffectPrimitiveState*> related;
  /* 00074 */ unsigned char unknown_01AC[1600];
  /* 006B4 */ CRes* resource;
  /* 006B8 */ void* resourceData;
  /* 006BC */ EZeffectTransform transforms[128];
  /* 040BC */ unsigned char vertices[55296];
  /* 118BC */ int currentLayer;
  /* 118C0 */ int endLayer;
  /* 118C4 */ bool layerDrawn[128];
  /* 11944 */ int currentKey[128];
  /* 11B44 */ int loopCount;
  /* 11B48 */ int loopLimit;
  /* 11B4C */ EffectPrimitiveState* unknown_11C84;
  /* 11B50 */ std::int32_t unknown_11C88[4];
  /* 11B60 */ unsigned char forceRender;
  /* 11B61 */ unsigned char unknown_11C99[3];
  /* 11B64 */ std::uint32_t tag;
  /* 11B68 */ unsigned char unknown_11CA0;
  /* 11B69 */ unsigned char unknown_11CA1[7];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RagEffectState) == 0x11B70);
static_assert(offsetof(RagEffectState, transforms) == 0x6BC);
static_assert(offsetof(RagEffectState, currentLayer) == 0x118BC);
static_assert(offsetof(RagEffectState, currentKey) == 0x11944);
static_assert(offsetof(RagEffectState, loopLimit) == 0x11B48);
#endif

#endif
