#ifndef RAGNAROK_SOURCE_CONTENTS_GROUND_TERRAINHEIGHT_H
#define RAGNAROK_SOURCE_CONTENTS_GROUND_TERRAINHEIGHT_H

#include <cstddef>
#include <cstdint>

struct TerrainHeightCell {
  /* 0000 */ float heights[4];
  /* 0010 */ std::uint32_t type;
};

struct TerrainHeight {
  const int& width;
  const int& height;
  const int& cellSize;
  const TerrainHeightCell* cells;

  float Height(float x, float z) const;
  float PlaneHeight(float x, float z) const;
  const TerrainHeightCell* CellAt(int column, int row) const;
  bool Walkable(int column, int row) const;
  void CellOrigin(int column, int row, float& x, float& z) const;
  void CellCenter(int column, int row, float& x, float& z) const;
  void WorldCell(float x, float z, int& column, int& row) const;

  void WorldSubcell(float x, float z, int& column, int& row, int& subColumn, int& subRow) const;
  void SpatialCell(float x, float z, int& column, int& row) const;
  void HeightBounds(const int* rectangle, float& minimum, float& maximum) const;
  float RayDistance(const float* ray, int column, int row) const;
};

static_assert(sizeof(TerrainHeightCell) == 20);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(TerrainHeightCell, type) == 0x10);
#endif

#endif
