#ifndef RAGNAROK_SOURCE_RESOURCE_SKINPALETTE_H
#define RAGNAROK_SOURCE_RESOURCE_SKINPALETTE_H

#include <cstdint>

class ResourceMgr;

struct SkinPaletteCell {
  /* 0000 */ int column;
  /* 0004 */ int row;
};

std::uint32_t SkinPaletteColor(ResourceMgr& resources, const SkinPaletteCell& cell, bool resolveSkin);
void SkinPaletteChannels(ResourceMgr& resources, int x, int y, unsigned& red, unsigned& green, unsigned& blue);

#endif
