#ifndef RAGNAROK_SOURCE_RESOURCE_PALETTERESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_PALETTERESOURCE_H

#include "Resource/Resource.h"

#include <cstdint>

class CPaletteRes : public CRes {
public:
  CPaletteRes();
  ~CPaletteRes() override;
  CPaletteRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;

  /* 0110 */ std::uint8_t sourcePalette[1024];
  /* 0510 */ std::uint32_t palette[256];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CPaletteRes) == 0x910);
#endif

#endif
