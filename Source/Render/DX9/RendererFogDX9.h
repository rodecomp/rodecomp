#ifndef RAGNAROK_SOURCE_RENDER_DX9_RENDERERFOGDX9_H
#define RAGNAROK_SOURCE_RENDER_DX9_RENDERERFOGDX9_H

#include <cstddef>
#include <d3d9.h>

struct TextureDX9Context;
struct DeviceStateDX9;

struct RendererFogDX9 {
  /* 0150 */ int enabled;
  /* 0154 */ int dirty;
  /* 0158 */ int supported;

  void Set(int enabled, const D3DCAPS9& caps);
  void Apply(const TextureDX9Context& context, DeviceStateDX9& state, const float& aspect);
};

static_assert(sizeof(RendererFogDX9) == 12);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(RendererFogDX9, enabled) == 0x0150 - 0x0150);
static_assert(offsetof(RendererFogDX9, dirty) == 0x0154 - 0x0150);
static_assert(offsetof(RendererFogDX9, supported) == 0x0158 - 0x0150);
#endif

#endif
