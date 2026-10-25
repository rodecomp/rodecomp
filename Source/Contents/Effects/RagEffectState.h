#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECTSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECTSTATE_H

#include "Contents/Actors/RenderObjectState.h"

#include <list>

class CRes;
struct EffectPrimitiveState;
struct EffectActor;

struct RagEffectState {
  RagEffectState();
  ~RagEffectState();

  /* 00138 */ EffectActor* parent;
  /* 0013C */ std::uint32_t unknown_013C[9];
  /* 00160 */ std::uint32_t unknown_0160;
  /* 00164 */ std::uint32_t unknown_0164;
  /* 00168 */ int id;
  /* 0016C */ int unknown_016C;
  /* 00170 */ std::uint32_t unknown_0170;
  /* 00174 */ std::uint32_t unknown_0174;
  /* 00178 */ std::int32_t unknown_0178;
  /* 0017C */ int lifetime;
  /* 00180 */ float offset[3];
  /* 0018C */ std::uint32_t unknown_018C[6];
  /* 001A4 */ std::list<EffectPrimitiveState*> related;
  /* 001AC */ unsigned char unknown_01AC[1600];
  /* 007EC */ CRes* resource;
  /* 007F0 */ void* resourceData;
  /* 007F4 */ unsigned char unknown_07F4[14848];
  /* 041F4 */ unsigned char vertices[55296];
  /* 119F4 */ std::uint32_t unknown_119F4;
  /* 119F8 */ unsigned char unknown_119F8[132];
  /* 11A7C */ unsigned char unknown_11A7C[512];
  /* 11C7C */ std::uint32_t unknown_11C7C;
  /* 11C80 */ std::uint32_t unknown_11C80;
  /* 11C84 */ EffectPrimitiveState* unknown_11C84;
  /* 11C88 */ std::int32_t unknown_11C88[4];
  /* 11C98 */ unsigned char forceRender;
  /* 11C99 */ unsigned char unknown_11C99[3];
  /* 11C9C */ std::uint32_t tag;
  /* 11CA0 */ unsigned char unknown_11CA0;
  /* 11CA1 */ unsigned char unknown_11CA1[7];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RagEffectState) == 0x11B70);
#endif

#endif
