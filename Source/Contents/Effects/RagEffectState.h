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
  /* 00004 */ float param[4];
  /* 00014 */ float longitude;
  /* 00018 */ float latitude;
  /* 0001C */ float tlvertX;
  /* 00020 */ float tlvertY;
  /* 00024 */ float tlvertZ;
  /* 00028 */ std::uint32_t defZ;
  /* 0002C */ std::uint32_t isDirection;
  /* 00030 */ int type;
  /* 00034 */ int level;
  /* 00038 */ std::uint32_t flag;
  /* 0003C */ std::uint32_t count;
  /* 00040 */ std::int32_t stateCnt;
  /* 00044 */ int duration;
  /* 00048 */ float deltaPos[3];
  /* 00054 */ float deltaPos2[3];
  /* 00060 */ float deltaPos3[3];
  /* 0006C */ std::list<EffectPrimitiveState*> primList;
  /* 00074 */ unsigned char objSeg[1600];
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
