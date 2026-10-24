#ifndef RAGNAROK_SOURCE_RENDER_SPRITEPIXELS_H
#define RAGNAROK_SOURCE_RENDER_SPRITEPIXELS_H

#include <cstdint>
#include <span>

// 36-byte action cell used by frame accessor.
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

// Original image record: dimensions at0/2, RLE byte pointer at+12 on x86.
struct SpriteRleImage {
  std::int16_t width;
  std::int16_t height;
  std::uint32_t unknown_0004;
  const void* unknown_0008;
  const std::uint8_t* runs;
};

#endif
