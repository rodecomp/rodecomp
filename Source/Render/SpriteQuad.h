#ifndef RAGNAROK_SOURCE_RENDER_SPRITEQUAD_H
#define RAGNAROK_SOURCE_RENDER_SPRITEQUAD_H

#include <cstddef>
#include <cstdint>

class CRenderSystemBase;
struct RenderPrimitive;
struct SpriteRleImage;

struct SpriteQuad {
  /* 0000 */ float top;
  /* 0004 */ float left;
  /* 0008 */ float bottom;
  /* 000C */ float right;
  /* 0010 */ float bottomRhw;
  /* 0014 */ float topRhw;
  /* 0018 */ std::uint32_t color;
  /* 001C */ float angle;
  /* 0020 */ std::int32_t unknown_0020[3];
  /* 002C */ const SpriteRleImage* image;
};

RenderPrimitive* BuildSpriteQuad(CRenderSystemBase& system, const SpriteQuad& quad, const std::uint32_t* palette, int blend, float offsetX, float offsetY);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(SpriteQuad, angle) == 0x001C);
static_assert(offsetof(SpriteQuad, image) == 0x002C);
#endif

#endif
