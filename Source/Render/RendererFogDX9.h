#ifndef RAGNAROK_SOURCE_RENDER_RENDERERFOGDX9_H
#define RAGNAROK_SOURCE_RENDER_RENDERERFOGDX9_H

#include "Render/FogConfiguration.h"

#include <d3d9.h>

class D3DX9Runtime;
struct TextureDX9Context;
struct DeviceStateDX9;

// CRenderer +0x150..+0x15B. dirty is tested for exact equality with one.
struct RendererFogDX9 {
  int enabled;
  int dirty;
  int supported;

  void Set(int enabled, const D3DCAPS9& caps, FogConfiguration& configuration);
  void Apply(
    const TextureDX9Context& context,
    DeviceStateDX9& state,
    D3DX9Runtime& d3dx,
    const float& aspect,
    const FogConfiguration& configuration
  );
};

static_assert(sizeof(RendererFogDX9) == 12);

#endif
