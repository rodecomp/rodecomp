#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_RAGEFFECTSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_CORE_RAGEFFECTSTATE_H

#include "Resource/Effects/EZeffectResource.h"

#include <cstddef>
#include <list>
#include "Utility/Math/Vector3d.h"

class CRes;
struct EffectPrimitiveState;
struct CEffectPrim;
struct EffectActor;

struct RagEffectSegment {
  RagEffectSegment() {
    this->header[0] = 0;
    this->header[1] = 0;
    this->header[2] = 0;
    this->header[3] = 0;
    this->header[4] = 0;
    this->header[5] = 0;
  }

  /* 0000 */ std::uint32_t header[6];
  /* 0018 */ unsigned char unknown_0018[56];
};

class CTexture;

struct RagEffectFaceVertex {
  __declspec(noinline) RagEffectFaceVertex() {
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
    this->rhw = 0.0f;
    this->diffuse = 0;
    this->specular = 0xFF000000u;
    this->u = 0.0f;
    this->v = 0.0f;
  }

  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ float z;
  /* 000C */ float rhw;
  /* 0010 */ std::uint32_t diffuse;
  /* 0014 */ std::uint32_t specular;
  /* 0018 */ float u;
  /* 001C */ float v;
};

struct RagEffectFace {
  RagEffectFace() {
  }

  /* 0000 */ RagEffectFaceVertex verts[3];
  /* 0060 */ CTexture* tex;
  /* 0064 */ unsigned long srcAlphaMode;
  /* 0068 */ unsigned long destAlphaMode;
};

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
  /* 00048 */ vector3d deltaPos;
  /* 00054 */ vector3d deltaPos2;
  /* 00060 */ vector3d deltaPos3;
  /* 0006C */ std::list<CEffectPrim*> primList;
  /* 00074 */ RagEffectSegment objSeg[20];
  /* 006B4 */ CRes* resource;
  /* 006B8 */ EZeffectData* resourceData;
  /* 006BC */ EZeffectTransform transforms[128];
  /* 040BC */ RagEffectFace vertices[512];
  /* 118BC */ int currentLayer;
  /* 118C0 */ int endLayer;
  /* 118C4 */ bool layerDrawn[128];
  /* 11944 */ int currentKey[128];
  /* 11B44 */ int loopCount;
  /* 11B48 */ int loopLimit;
  union {
    /* 11B4C */ EffectPrimitiveState* currentPrimitive;
    /* 11B4C */ CEffectPrim* lastPrim;
  };
  /* 11B50 */ std::int32_t colorChannels[4];
  /* 11B60 */ unsigned char forceRender;
  /* 11B61 */ unsigned char padding_11B61[3];
  /* 11B64 */ std::uint32_t tag;
  /* 11B68 */ unsigned char visibilityFlags;
  /* 11B69 */ unsigned char padding_11B69[7];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RagEffectSegment) == 0x50);
static_assert(sizeof(RagEffectFace) == 0x6C);
static_assert(sizeof(RagEffectState) == 0x11B70);
static_assert(offsetof(RagEffectState, transforms) == 0x6BC);
static_assert(offsetof(RagEffectState, currentLayer) == 0x118BC);
static_assert(offsetof(RagEffectState, currentKey) == 0x11944);
static_assert(offsetof(RagEffectState, loopLimit) == 0x11B48);
#endif

#endif
