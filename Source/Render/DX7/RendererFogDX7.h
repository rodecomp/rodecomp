#ifndef RAGNAROK_SOURCE_RENDER_DX7_RENDERERFOGDX7_H
#define RAGNAROK_SOURCE_RENDER_DX7_RENDERERFOGDX7_H

#include "Render/FogConfiguration.h"

#include <d3d.h>

class CRenderSystemBase;

HRESULT PerspectiveMatrixDX7(D3DMATRIX& matrix, float fov, float aspect, float nearPlane, float farPlane);

struct RendererFogDX7 {
  /* 0150 */ int enabled;
  /* 0154 */ int dirty;
  /* 0158 */ int supported;

  void Set(int value, IDirect3DDevice7* device, const D3DDEVICEDESC7& caps, FogConfiguration& configuration);
  void Apply(IDirect3DDevice7* device, const D3DDEVICEDESC7& caps, float aspect, const FogConfiguration& configuration, CRenderSystemBase* renderSystem);
};

static_assert(sizeof(RendererFogDX7) == 12);

#endif
