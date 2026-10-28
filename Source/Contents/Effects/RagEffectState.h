#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECTSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECTSTATE_H

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
  /* 00054 */ std::uint32_t unknown_018C[6];
  /* 0006C */ std::list<EffectPrimitiveState*> related;
  /* 00074 */ unsigned char unknown_01AC[1600];
  /* 006B4 */ CRes* resource;
  /* 006B8 */ void* resourceData;
  /* 006BC */ unsigned char unknown_07F4[14848];
  /* 040BC */ unsigned char vertices[55296];
  /* 118BC */ std::uint32_t unknown_119F4;
  /* 118C0 */ unsigned char unknown_119F8[132];
  /* 11944 */ unsigned char unknown_11A7C[512];
  /* 11B44 */ std::uint32_t unknown_11C7C;
  /* 11B48 */ std::uint32_t unknown_11C80;
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
#endif

#endif
