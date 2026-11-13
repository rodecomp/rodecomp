#ifndef RAGNAROK_SOURCE_RENDER_DRAWING_SPRITEGEOMETRY_H
#define RAGNAROK_SOURCE_RENDER_DRAWING_SPRITEGEOMETRY_H

#include <cstddef>
#include <cstdint>

struct SpriteColor {
  /* 0000 */ std::uint8_t blue;
  /* 0001 */ std::uint8_t green;
  /* 0002 */ std::uint8_t red;
  /* 0003 */ std::uint8_t alpha;
};

extern float g_tangent;
extern float g_cameraScaleY;
extern float g_cameraScaleX;
extern float g_heightScale;
extern float g_projectionScale;

static_assert(sizeof(SpriteColor) == 4);

#endif
