#ifndef RAGNAROK_SOURCE_RENDER_TEXTUREALLOCATIONDX7_H
#define RAGNAROK_SOURCE_RENDER_TEXTUREALLOCATIONDX7_H

#include <d3d.h>

// Shared original globals, not per-texture options.
struct TextureAllocationDX7 {
  DWORD descriptorFlags;
  DWORD surfaceCaps;

  void Update(int enabled, const D3DDEVICEDESC7& caps);
};

#endif
