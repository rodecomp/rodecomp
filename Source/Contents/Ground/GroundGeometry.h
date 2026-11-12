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
  /* 0028 */ std::uint32_t cornerColors[4];
};

class GroundLightmapSet {
public:
  ~GroundLightmapSet();
  void Build(CGndRes& resource);
  void Clear();

  /* 0000 */ std::vector<GroundLightmap> lightmaps;
  /* 000C */ std::vector<CTexture*> lightmapTextures;
  /* 0018 */ int lightmapCount;
  /* 001C */ int atlasCount;
};

class GroundBase {
public:
  GroundBase();
  GroundBase(const GroundBase&) = delete;
  GroundBase& operator=(const GroundBase&) = delete;
  virtual ~GroundBase();
  virtual void Reset(int columns, int rows);
  virtual void Build(CGndRes& resource, const float* lightDirection, const float* diffuse, const float* ambient);
  virtual void ClearGeometry();
  virtual void RenderLod(int first);
  virtual void Draw(const float* matrix, const int* rectangle, bool clip);
  virtual void DrawLit(const float* matrix, const int* rectangle, bool clip);
  virtual void CellBounds(int column, int row, float& minimum, float& maximum) const;
  virtual void HeightBounds(const int* rectangle, float& minimum, float& maximum) const;
  virtual void MarkRay(const float* ray, int column, int row, float limit);
  virtual bool IsNewVer();
  virtual void InitializeLighting();
  virtual int Lighting(int column, int row, float& red, float& green, float& blue) const;
  virtual void ImportWater(const CGndRes& resource);
  virtual void DrawGrid(const float* matrix, int column, int row, std::uint32_t color) const;
  virtual void SetGndRiseAmount(float alpha, int x, int y);
  virtual void DrawCellDecal(const float* matrix, int column, int row, std::uint32_t color, CTexture& texture, const float* uv) const;
  virtual void DrawTerrainOverlay(const float* matrix, int column, int row, std::uint32_t color, CTexture* texture) const;
  virtual bool CellVertices(float* vertices, int column, int row) const;
  virtual void UpdateWater();

  void SpatialCell(float x, float z, int& column, int& row) const;
  void SetWater(float level, int type, int animation, int pitch, int phase, float height);

  /* 0004 */ class C3dAttr* terrain;
  /* 0008 */ int width;
  /* 000C */ int height;
  /* 0010 */ float cellSize;
  /* 0014 */ GroundLightmapSet lightmapSet;
  /* 0034 */ std::uint32_t surfaceCount;
  /* 0038 */ float waterLevel;
  /* 003C */ int waterAnimation;
  /* 0040 */ int waterPitch;
  /* 0044 */ int waterPhase;
  /* 0048 */ int waterType;
  /* 004C */ float waterHeight;
  /* 0050 */ std::uint32_t waterStateA;
  /* 0054 */ std::uint32_t waterStateB;
  /* 0058 */ std::uint32_t waterStateC;
  /* 005C */ std::uint32_t waterStateD;
  /* 0060 */ int loaded;
};

class GroundGeometry : public GroundBase {
public:
  GroundGeometry();
  ~GroundGeometry() override;
  void Reset(int columns, int rows) override;
  void Build(CGndRes& resource, const float* lightDirection, const float* diffuse, const float* ambient) override;
  void ClearGeometry() override;
  void RenderLod(int first) override;
  void Draw(const float* matrix, const int* rectangle, bool clip) override;
  void DrawLit(const float* matrix, const int* rectangle, bool clip) override;
  void CellBounds(int column, int row, float& minimum, float& maximum) const override;
  void HeightBounds(const int* rectangle, float& minimum, float& maximum) const override;
  void MarkRay(const float* ray, int column, int row, float limit) override;
  bool IsNewVer() override;
  void InitializeLighting() override;
  int Lighting(int column, int row, float& red, float& green, float& blue) const override;
  void ImportWater(const CGndRes& resource) override;
  void DrawGrid(const float* matrix, int column, int row, std::uint32_t color) const override;
  void DrawCellDecal(const float* matrix, int column, int row, std::uint32_t color, CTexture& texture, const float* uv) const override;
  void DrawTerrainOverlay(const float* matrix, int column, int row, std::uint32_t color, CTexture* texture) const override;
  bool CellVertices(float* vertices, int column, int row) const override;
  void UpdateWater() override;

  bool CellRectangle(int column, int row, float* vertices) const;
  void DrawGridDecal(const float* matrix, int column, int row, std::uint32_t color, CTexture& texture, const float* uv) const;
  void DrawWaterCell(int column, int row, GroundCell& cell, const float* matrix, const float* ambient);
  __declspec(noinline) void DrawLightmapped(const float* matrix, const GroundQuad& quad, bool clip);
  __declspec(noinline) void DrawSide(const float* matrix, const GroundQuad& quad, bool clip);
  void WorldColor(float x, float unusedY, float z, std::uint32_t* output) const;
  void LightmapSample(int column, int row, unsigned char& alpha, unsigned char& red, unsigned char& green, unsigned char& blue) const;
  bool TerrainCellCorners(int column, int row, float* vertices) const;
  float WaterLevel(int column, int row) const;

  /* 0064 */ std::vector<GroundQuad> tiles;
  /* 0070 */ std::vector<std::array<float, 3>> lighting;
  /* 007C */ std::vector<GroundCell> cells;
  /* 0088 */ int waterColumns;
  /* 008C */ int waterRows;
  /* 0090 */ std::vector<float> waterLevels;
  /* 009C */ std::vector<GroundWaterSurface> waterSurfaces;
  /* 00A8 */ bool waterPartitioned;
};

extern float g_groundGridUV[8];

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GroundLightmap) == 56);
static_assert(offsetof(GroundLightmap, texture) == 0x04);
static_assert(offsetof(GroundLightmap, uv) == 0x08);
static_assert(offsetof(GroundLightmap, cornerColors) == 0x28);
static_assert(sizeof(GroundLightmapSet) == 0x20);
static_assert(offsetof(GroundLightmapSet, lightmapCount) == 0x18);
static_assert(offsetof(GroundLightmapSet, atlasCount) == 0x1C);
static_assert(sizeof(GroundBase) == 0x64);
static_assert(offsetof(GroundBase, terrain) == 0x04);
static_assert(offsetof(GroundBase, width) == 0x08);
static_assert(offsetof(GroundBase, height) == 0x0C);
static_assert(offsetof(GroundBase, cellSize) == 0x10);
static_assert(offsetof(GroundBase, lightmapSet) == 0x14);
static_assert(offsetof(GroundBase, waterLevel) == 0x38);
static_assert(offsetof(GroundBase, loaded) == 0x60);
static_assert(sizeof(GroundGeometry) == 0xAC);
static_assert(offsetof(GroundGeometry, tiles) == 0x64);
static_assert(offsetof(GroundGeometry, lighting) == 0x70);
static_assert(offsetof(GroundGeometry, cells) == 0x7C);
static_assert(offsetof(GroundGeometry, waterColumns) == 0x88);
static_assert(offsetof(GroundGeometry, waterRows) == 0x8C);
static_assert(offsetof(GroundGeometry, waterLevels) == 0x90);
static_assert(offsetof(GroundGeometry, waterSurfaces) == 0x9C);
static_assert(offsetof(GroundGeometry, waterPartitioned) == 0xA8);
#endif

#endif
