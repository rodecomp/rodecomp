#ifndef RAGNAROK_SOURCE_RENDER_SPRITEGEOMETRY_H
#define RAGNAROK_SOURCE_RENDER_SPRITEGEOMETRY_H

#include <cstddef>
#include <cstdint>

struct SpriteColor {
  /* 0000 */ std::uint8_t blue;
  /* 0001 */ std::uint8_t green;
  /* 0002 */ std::uint8_t red;
  /* 0003 */ std::uint8_t alpha;
};

struct SpriteCellQuad {
  /* 0000 */ float top;
  /* 0004 */ float left;
  /* 0008 */ float bottom;
  /* 000C */ float right;
  /* 0010 */ float bottomDepth;
  /* 0014 */ float topDepth;
  /* 0018 */ SpriteColor color;
  /* 001C */ float angle;
};

extern float g_tangent;
extern float g_cameraScaleY;
extern float g_cameraScaleX;
extern float g_heightScale;

static_assert(sizeof(SpriteColor) == 4);
static_assert(sizeof(SpriteCellQuad) == 0x20);
static_assert(offsetof(SpriteCellQuad, color) == 0x18);

#endif
