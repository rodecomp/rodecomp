#ifndef RAGNAROK_SOURCE_CONTENTS_TERRAINHEIGHT_H
#define RAGNAROK_SOURCE_CONTENTS_TERRAINHEIGHT_H

#include <cstdint>

struct TerrainHeightCell {
  float heights[4];
  std::uint32_t type;
};

// Borrowed C3dAttr fields +0x110..+0x11C, reached through the active mode.
// The query neither allocates nor changes cells.
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
  // Writes column, row, then signed sixteenth-cell remainders in that order.
  // Outputs may alias: each fractional coordinate reloads the stored cell.
  void WorldSubcell(float x, float z, int& column, int& row, int& subColumn, int& subRow) const;
  void SpatialCell(float x, float z, int& column, int& row) const;
  void HeightBounds(const int* rectangle, float& minimum, float& maximum) const;
  float RayDistance(const float* ray, int column, int row) const;
};

static_assert(sizeof(TerrainHeightCell) == 20);

#endif
