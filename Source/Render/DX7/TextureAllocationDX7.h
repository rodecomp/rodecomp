#ifndef RAGNAROK_SOURCE_RENDER_DX7_TEXTUREALLOCATIONDX7_H
#define RAGNAROK_SOURCE_RENDER_DX7_TEXTUREALLOCATIONDX7_H

#include <d3d.h>

struct TextureAllocationDX7 {
  DWORD descriptorFlags;
  DWORD surfaceCaps;

  void Update(int enabled, const D3DDEVICEDESC7& caps);
};

#endif
