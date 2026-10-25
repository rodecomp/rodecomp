#ifndef RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDSPATIAL_H
#define RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDSPATIAL_H

#include <cstdint>
#include <vector>

class CTexture;
struct GroundLightmap;

struct GroundQuad {
  GroundQuad();

  float vertices[4][8];
  std::uint32_t unknown_0080;
  CTexture* texture;
  CTexture* lightmapTexture;
  GroundLightmap* lightmap;
  std::uint32_t color;
  std::uint32_t selected;
  std::uint32_t unknown_0098;
};

struct GroundCell {
  GroundCell();

  /* 0000 */ float heights[4];
  /* 0010 */ float corners[4][3];
  /* 0040 */ GroundQuad* top;
  /* 0044 */ GroundQuad* side1;
  /* 0048 */ GroundQuad* side2;
  /* 004C */ std::uint32_t frame;
  /* 0050 */ unsigned char color[3];
  /* 0053 */ unsigned char unknown_0053;
};

struct GroundSpatial {
  const int& width;
  const int& height;
  const float& cellSize;
  const std::vector<GroundCell>& cells;

  void SpatialCell(float x, float z, int& column, int& row) const;
  void HeightBounds(const int* rectangle, float& minimum, float& maximum) const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GroundQuad) == 156);
static_assert(sizeof(GroundCell) == 84);
#endif

#endif
