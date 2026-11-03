#ifndef RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDGEOMETRY_H
#define RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDGEOMETRY_H

#include "Contents/Ground/GroundSpatial.h"
#include "Contents/Ground/GroundWaterState.h"

#include <cstddef>
#include <array>

class CGndRes;
class TextureCache;

struct GroundLightmap {
  /* 0000 */ unsigned char color[4];
  /* 0004 */ CTexture* texture;
  /* 0008 */ float uv[4][2];
  /* 0028 */ std::uint32_t unknown_0028[4];
};

class GroundGeometry {
public:
  GroundGeometry();
  ~GroundGeometry();
  GroundGeometry(const GroundGeometry&) = delete;
  GroundGeometry& operator=(const GroundGeometry&) = delete;
  void Build(CGndRes& resource, const float* lightDirection, const float* diffuse, const float* ambient);
  void ClearGeometry();
  void Reset(int columns, int rows);
  void SpatialCell(float x, float z, int& column, int& row) const;
  void HeightBounds(const int* rectangle, float& minimum, float& maximum) const;
  void Draw(const float* matrix, const int* rectangle, bool clip);
  void CellBounds(int column, int row, float& minimum, float& maximum) const;
  bool OverlayHeights(const class C3dAttr& terrain, float* vertices, int column, int row) const;
  bool CellVertices(const class C3dAttr& terrain, float* vertices, int column, int row) const;
  void MarkRay(const float* ray, int column, int row, float limit);
  bool CellRectangle(const class C3dAttr& terrain, int column, int row, float* vertices) const;
  void DrawCellDecal(
    const class C3dAttr& terrain, const float* matrix, int column, int row, std::uint32_t color, CTexture& texture, const float* uv
  ) const;
  void DrawGridDecal(
    const class C3dAttr& terrain, const float* matrix, int column, int row, std::uint32_t color, CTexture& texture, const float* uv
  ) const;
  void DrawGrid(const class C3dAttr& terrain, const float* matrix, int column, int row, std::uint32_t color) const;
  void DrawTerrainOverlay(const class C3dAttr& terrain, const float* matrix, int column, int row, std::uint32_t color, CTexture* texture) const;
  void DrawWaterCell(int column, int row, GroundCell& cell, const float* matrix, const float* ambient);
  void DrawLit(const float* matrix, const int* rectangle, bool clip);
  __declspec(noinline) void DrawLightmapped(const float* matrix, const struct GroundQuad& quad, bool clip);
  __declspec(noinline) void DrawSide(const float* matrix, const struct GroundQuad& quad, bool clip);
  void WorldColor(float x, float unusedY, float z, std::uint32_t* output) const;
  void Lighting(int column, int row, float& red, float& green, float& blue) const;
  void Slot01(int first, int second);
  void Slot02(int first, int second, int third, int fourth);
  void Slot04(int first);
  void Ground15Slot04(int first);
  void Slot05(int first, int second, int third);
  void Slot06(int first, int second, int third);
  void Slot07(int first, int second, int third, int fourth) const;
  void Slot08(int first, int second, int third) const;
  void Slot09(int first, int second, int third, int fourth);
  bool Slot10();
  bool Ground15Slot10();
  void Slot11();
  int Slot12(int first, int second, int third, int fourth, int fifth) const;
  void Slot14(int first, int second, int third, int fourth);
  void Slot16(int first, int second, int third, int fourth, int fifth, int sixth);
  void Slot19();
  void LightmapSample(int column, int row, unsigned char& alpha, unsigned char& red, unsigned char& green, unsigned char& blue) const;
  bool TerrainCellCorners(const class C3dAttr& terrain, int column, int row, float* vertices) const;
  void InitializeLighting(const class C3dAttr& terrain);

  int width;
  int height;
  float cellSize;
  int loaded;
  std::vector<GroundLightmap> lightmaps;
  std::vector<CTexture*> lightmapTextures;
  std::vector<GroundQuad> tiles;
  std::vector<std::array<float, 3>> lighting;
  std::vector<GroundCell> cells;
  std::uint32_t unknown_0058;
  std::uint32_t unknown_005C;
  GroundWaterState water;

private:
  void BuildLightmaps(CGndRes& resource);
  void ClearLightmaps();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GroundLightmap) == 56);
static_assert(offsetof(GroundLightmap, texture) == 0x04);
static_assert(offsetof(GroundLightmap, uv) == 0x08);
static_assert(offsetof(GroundLightmap, unknown_0028) == 0x28);
#endif

#endif
