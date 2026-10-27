#ifndef RAGNAROK_SOURCE_RENDER_TEXTURES_SPRITEPIXELS_H
#define RAGNAROK_SOURCE_RENDER_TEXTURES_SPRITEPIXELS_H

#include <cstddef>
#include <cstdint>
#include <span>

struct SpriteFrameCell {
  /* 0000 */ std::int32_t x;
  /* 0004 */ std::int32_t y;
  /* 0008 */ std::int32_t image;
  /* 000C */ std::int32_t mirrored;
  /* 0010 */ std::uint32_t color;
  /* 0014 */ float scaleX;
  /* 0018 */ float scaleY;
  /* 001C */ std::int32_t unknown_001C;
  /* 0020 */ std::int32_t imageType;
};
static_assert(sizeof(SpriteFrameCell) == 36);

struct SpriteRleImage {
  /* 0000 */ std::int16_t width;
  /* 0002 */ std::int16_t height;
  /* 0004 */ std::uint16_t unknown_0004;
  /* 0006 */ std::uint16_t unknown_0006;
  /* 0008 */ const void* unknown_0008;
  /* 000C */ const std::uint8_t* runs;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(SpriteFrameCell, imageType) == 0x20);
static_assert(sizeof(SpriteRleImage) == 0x10);
static_assert(offsetof(SpriteRleImage, unknown_0004) == 0x04);
static_assert(offsetof(SpriteRleImage, unknown_0006) == 0x06);
static_assert(offsetof(SpriteRleImage, unknown_0008) == 0x08);
static_assert(offsetof(SpriteRleImage, runs) == 0x0C);
#endif

#endif
