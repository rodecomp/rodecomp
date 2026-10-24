#include "Render/D3DX9Runtime.h"
#include "Render/DeviceStateDX9.h"
#include "Render/RendererFogDX9.h"
#include "Render/TextureDX9.h"

#include <bit>

// OFFSET: 0x00567E20
// FUNCTION: fn_02230
void RendererFogDX9::Set(int value, const D3DCAPS9& caps, FogConfiguration& configuration) {
  const int supported = (caps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX) != 0;
  this->dirty = 1;
  this->supported = supported;
  this->enabled = value;
  configuration.vertexMode = D3DFOG_LINEAR;
  configuration.range = 1;
}

// OFFSET: 0x005654D0
// FUNCTION: fn_02217
void RendererFogDX9::Apply(
  const TextureDX9Context& context,
  DeviceStateDX9& state,
  D3DX9Runtime& d3dx,
  const float& aspect,
  const FogConfiguration& configuration
) {
  if (this->dirty == 1) {
    const float inverseAspect = 1.0f / aspect;
    const bool supported = (context.caps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX) != 0;
    D3DMATRIX identity{};
    identity._11 = identity._22 = identity._33 = identity._44 = 1.0f;
    const float start = configuration.start * 1490.0f + 10.0f;
    const float end = configuration.end * 1490.0f + 10.0f;
    D3DMATRIX projection;
    d3dx.perspectiveFovLH(&projection, 0.2617994f, inverseAspect, 10.0f, 1500.0f);
    context.device->SetTransform(D3DTS_WORLD, &identity);
    context.device->SetTransform(D3DTS_PROJECTION, &projection);
    context.device->SetTransform(D3DTS_VIEW, &identity);
    state.SetRenderState(D3DRS_FOGCOLOR, configuration.color);
    state.SetRenderState(D3DRS_RANGEFOGENABLE, configuration.range);
    state.SetRenderState(D3DRS_FOGSTART, std::bit_cast<unsigned>(start));
    state.SetRenderState(D3DRS_FOGEND, std::bit_cast<unsigned>(end));
    state.SetRenderState(D3DRS_FOGDENSITY, std::bit_cast<unsigned>(configuration.density));

    if (supported) {
      state.SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
      state.SetRenderState(D3DRS_FOGVERTEXMODE, configuration.vertexMode);
    } else {
      this->enabled = 0;
    }

    this->dirty = 0;
  }

  state.SetRenderState(D3DRS_FOGENABLE, this->enabled);
}
