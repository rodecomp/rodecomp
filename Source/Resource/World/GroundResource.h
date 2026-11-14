#ifndef RAGNAROK_SOURCE_RESOURCE_WORLD_GROUNDRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_WORLD_GROUNDRESOURCE_H

#include "File/File.h"
#include "Resource/Resource.h"

#include <cstdint>
#include <vector>

struct GroundCellRecord {
  /* 0000 */ float heights[4];
  /* 0010 */ int tiles[3];
};

struct GroundTileRecord {
  /* 0000 */ float u[4];
  /* 0010 */ float v[4];
  /* 0020 */ std::int16_t texture;
  /* 0022 */ std::uint16_t lightmap;
  /* 0024 */ std::uint32_t color;
};

struct GroundWaterParameters {
  /* 0000 */ float level;
  /* 0004 */ int type;
  /* 0008 */ float height;
  /* 000C */ float speed;
  /* 0010 */ float pitch;
  /* 0014 */ int animation;
};

struct GroundWater {
  /* 0000 */ GroundWaterParameters parameters;
  /* 0018 */ std::uint32_t reserved[3];
};

struct GroundLegacyLightmap {
  /* 0000 */ unsigned char bits[40];

  void Decode(unsigned char* output) const;
};

class CGndRes : public CRes {
public:
  CGndRes();
  CGndRes(const char* extension, const char* prefix);
  ~CGndRes() override;
  CGndRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;
  void Lightmap(int index, unsigned char* output) const;

  /* 0110 */ int loaded;
  /* 0114 */ unsigned char major;
  /* 0115 */ unsigned char minor;
  /* 0116 */ unsigned char padding_0116[2];
  /* 0118 */ int width;
  /* 011C */ int height;
  /* 0120 */ float cellSize;
  /* 0124 */ int textureCount;
  /* 0128 */ int tileCount;
  /* 012C */ int lightmapCount;
  /* 0130 */ GroundWaterParameters waterDefault;
  /* 0148 */ int waterWidth;
  /* 014C */ int waterHeight;
  /* 0150 */ std::vector<float> waterHeights;
  /* 015C */ std::vector<GroundWater> water;
  /* 0168 */ std::vector<const char*> textureNames;
  /* 0174 */ const unsigned char* lightmaps;
  /* 0178 */ const std::uint32_t* legacyLightmapIndices;
  /* 017C */ const GroundLegacyLightmap* legacyLightmaps;
  /* 0180 */ const GroundTileRecord* tiles;
  /* 0184 */ const GroundCellRecord* cells;

private:
  bool Parse();
  void ReadWater();

  /* 0188 */ CFile m_file;
};

static_assert(sizeof(GroundWaterParameters) == 24);
static_assert(sizeof(GroundWater) == 36);
static_assert(sizeof(GroundCellRecord) == 28);
static_assert(offsetof(GroundCellRecord, tiles) == 0x10);
static_assert(sizeof(GroundTileRecord) == 40);
static_assert(offsetof(GroundTileRecord, texture) == 0x20);
static_assert(offsetof(GroundTileRecord, color) == 0x24);
static_assert(sizeof(GroundLegacyLightmap) == 40);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGndRes) == 0x01B4);
static_assert(offsetof(CGndRes, tiles) == 0x180);
static_assert(offsetof(CGndRes, cells) == 0x184);
#endif

#endif
