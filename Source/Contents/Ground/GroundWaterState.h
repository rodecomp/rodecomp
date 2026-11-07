#ifndef RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDWATERSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDWATERSTATE_H

#include <cstddef>

class CTexture;

struct GroundWaterSurface {
  /* 0000 */ float level;
  /* 0004 */ int type;
  /* 0008 */ float height;
  /* 000C */ float speed;
  /* 0010 */ float pitch;
  /* 0014 */ int animation;
  /* 0018 */ int frame;
  /* 001C */ float phase;
  /* 0020 */ CTexture* texture;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GroundWaterSurface) == 36);
static_assert(offsetof(GroundWaterSurface, texture) == 0x20);
#endif

#endif
