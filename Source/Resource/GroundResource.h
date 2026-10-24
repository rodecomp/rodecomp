#ifndef RAGNAROK_SOURCE_RESOURCE_GROUNDRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_GROUNDRESOURCE_H

#include "Resource/Resource.h"

#include <vector>

class ResourceBytes;
struct ResourceByteReader;

struct GroundWater {
  std::uint32_t values[6];
  std::uint32_t unknown_0018[3];
};

struct GroundResourceContext {
  ResourceBytes& file;
};

// RTTI CGndRes. Names and record slices borrow retained
// file bytes. Unload clears selected slices, not dimensions or water records.
class CGndRes : public CRes {
public:
  explicit CGndRes(GroundResourceContext& context);
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
  /* 0130 */ std::uint32_t waterDefault[6];
  /* 0148 */ int waterWidth;
  /* 014C */ int waterHeight;
  /* 0150 */ std::vector<std::uint32_t> waterHeights;
  /* 015C */ std::vector<GroundWater> water;
  /* 0168 */ std::vector<const char*> textureNames;
  /* 0174 */ const unsigned char* lightmaps;
  /* 0178 */ const unsigned char* legacyLightmapIndices;
  /* 017C */ const unsigned char* legacyLightmapBytes;
  /* 0180 */ const unsigned char* tiles;
  /* 0184 */ const unsigned char* cells;

private:
  bool Parse(ResourceByteReader& input);
  void ReadWater(ResourceByteReader& input);
  // Required snapshot: ResourceBytes cannot transfer its borrowed allocation.
  // Replaces the original retaining CFile at +0x188.
  std::vector<unsigned char> m_bytes;
  GroundResourceContext* m_context;
};

static_assert(sizeof(GroundWater) == 36);

#endif
