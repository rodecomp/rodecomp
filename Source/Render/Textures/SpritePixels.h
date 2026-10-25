#ifndef RAGNAROK_SOURCE_RENDER_TEXTURES_SPRITEPIXELS_H
#define RAGNAROK_SOURCE_RENDER_TEXTURES_SPRITEPIXELS_H

#include <cstdint>
#include <span>

struct SpriteFrameCell {
  std::int32_t x;
  std::int32_t y;
  std::int32_t image;
  std::int32_t mirrored;
  std::uint32_t color;
  float scaleX;
  float scaleY;
  std::int32_t unknown_001C;
  std::int32_t imageType;
};
static_assert(sizeof(SpriteFrameCell) == 36);

struct SpriteRleImage {
  std::int16_t width;
  std::int16_t height;
  std::uint32_t unknown_0004;
  const void* unknown_0008;
  const std::uint8_t* runs;
};

#endif
