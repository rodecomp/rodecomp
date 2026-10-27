#ifndef RAGNAROK_SOURCE_RESOURCE_WORLD_GROUNDRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_WORLD_GROUNDRESOURCE_H

#include "File/File.h"
#include "Resource/Resource.h"

#include <vector>

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
  /* 0018 */ std::uint32_t unknown_0018[3];
};

struct GroundLegacyLightmap {
  /* 0000 */ unsigned char bits[40];

  void Decode(unsigned char* output) const;
};

class CGndRes : public CRes {
public:
  CGndRes();
  ~CGndRes() override;
  CGndRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;
  void Lightmap(int index, unsigned char* output) const;

  /* 0110 */ int loaded;
  /* 0114 */ unsigned char major;
  /* 0115 */ unsigned char minor;
  /* 0116 */ unsigned char unknown_0116[2];
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
  /* 0180 */ const unsigned char* tiles;
  /* 0184 */ const unsigned char* cells;

private:
  bool Parse();
  void ReadWater();

  /* 0188 */ CFile m_file;
};

static_assert(sizeof(GroundWaterParameters) == 24);
static_assert(sizeof(GroundWater) == 36);
static_assert(sizeof(GroundLegacyLightmap) == 40);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGndRes) == 0x01B4);
#endif

#endif
