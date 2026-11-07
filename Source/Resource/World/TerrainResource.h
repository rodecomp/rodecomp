#ifndef RAGNAROK_SOURCE_RESOURCE_WORLD_TERRAINRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_WORLD_TERRAINRESOURCE_H

#include "Contents/Ground/TerrainHeight.h"
#include "Resource/Resource.h"

#include <vector>

class C3dAttr : public CRes {
public:
  C3dAttr();
  C3dAttr(const char* extension, const char* prefix);
  ~C3dAttr() override;
  C3dAttr* Create() override;
  bool Load(const char* name) override;
  void Unload() override;
  float Height(float x, float z) const;
  float PlaneHeight(float x, float z) const;
  const TerrainHeightCell* CellAt(int column, int row) const;
  bool Walkable(int column, int row) const;
  std::uint32_t CellType(float x, float z) const;
  void SpatialCell(float x, float z, int& column, int& row) const;
  void HeightBounds(const int* rectangle, float& minimum, float& maximum) const;
  float RayDistance(const float* ray, int column, int row) const;

  /* 0110 */ int width;
  /* 0114 */ int height;
  /* 0118 */ int cellSize;

private:
  struct Cell : TerrainHeightCell {
    // INLINED
    Cell() noexcept {
    }
  };
  /* 011C */ std::vector<Cell> m_cells;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(C3dAttr) == 0x0128);
#endif

#endif
