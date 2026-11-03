#include <d3d9.h>
#include "Render/DX9/D3DX9Runtime.h"
#include "Render/DX9/DeviceStateDX9.h"
#include "Render/DX9/RendererFogDX9.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/TextureDX9.h"
#include "Render/TextureQuad.h"

#include <bit>

// OFFSET: 0x00567E20
// FUNCTION: fn_02230
void RendererFogDX9::Set(int value, const _D3DCAPS9& caps) {
  const int supported = (caps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX) != 0;
  this->dirty = 1;
  this->supported = supported;
  this->enabled = value;
  g_fogRange = 1;
  g_fogVertexMode = D3DFOG_LINEAR;
}

// OFFSET: 0x005654D0
// FUNCTION: fn_02217
void CRendererDX9::ApplyFog() {
  DeviceStateDX9& state = this->device->state;
  if (this->fog.dirty == 1) {
    const float inverseAspect = 1.0f / this->viewport.aspect;
    const bool supported = (this->device->caps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX) != 0;
    D3DMATRIX identity{};
    identity._11 = identity._22 = identity._33 = identity._44 = 1.0f;
    const float start = g_depthStart * 1490.0f + 10.0f;
    const float end = g_depthEnd * 1490.0f + 10.0f;
    D3DMATRIX projection;
    D3DXMatrixPerspectiveFovLH(&projection, 0.2617994f, inverseAspect, 10.0f, 1500.0f);
    this->nativeDevice->SetTransform(D3DTS_WORLD, &identity);
    this->nativeDevice->SetTransform(D3DTS_PROJECTION, &projection);
    this->nativeDevice->SetTransform(D3DTS_VIEW, &identity);

    state.SetRenderState(D3DRS_FOGCOLOR, g_worldOverlayColor);
    state.SetRenderState(D3DRS_RANGEFOGENABLE, g_fogRange);
    state.SetRenderState(D3DRS_FOGSTART, std::bit_cast<unsigned>(start));
    state.SetRenderState(D3DRS_FOGEND, std::bit_cast<unsigned>(end));
    state.SetRenderState(D3DRS_FOGDENSITY, std::bit_cast<unsigned>(g_fogDensity));

    if (supported) {
      state.SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
      state.SetRenderState(D3DRS_FOGVERTEXMODE, g_fogVertexMode);
    } else {
      this->fog.enabled = 0;
    }

    this->fog.dirty = 0;
  }

  state.SetRenderState(D3DRS_FOGENABLE, this->fog.enabled);
}
