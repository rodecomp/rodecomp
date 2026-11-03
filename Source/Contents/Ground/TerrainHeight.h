#ifndef RAGNAROK_SOURCE_CONTENTS_GROUND_TERRAINHEIGHT_H
#define RAGNAROK_SOURCE_CONTENTS_GROUND_TERRAINHEIGHT_H

#include <cstddef>
#include <cstdint>

struct TerrainHeightCell {
  /* 0000 */ float heights[4];
  /* 0010 */ std::uint32_t type;
};

static_assert(sizeof(TerrainHeightCell) == 20);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(TerrainHeightCell, type) == 0x10);
#endif

#endif
