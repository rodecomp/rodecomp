#include "Client/Application/ErrorDialog.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/DX7/RendererFogDX7.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/DX7/RendererDX7.h"

#include <bit>
#include <cmath>

// OFFSET: 0x00556E50
  // FUNCTION: fn_02039
  __declspec(noinline) float ClientSin(volatile float angle) {
    angle = sinf(angle);
    return angle;
  }

// OFFSET: 0x00556E40
  // FUNCTION: fn_02038
  __declspec(noinline) float ClientCos(volatile float angle) {
    angle = cosf(angle);
    return angle;
  }

// OFFSET: 0x00556E60
// FUNCTION: fn_02040
float SquareRoot(volatile float value) {
  value = sqrtf(value);
  return value;
}

// OFFSET: 0x00548C30
// FUNCTION: fn_01863
HRESULT PerspectiveMatrixDX7(D3DMATRIX& matrix, float fov, float aspect, float nearPlane, float farPlane) {
  if (std::fabs(farPlane - nearPlane) < 0.01f) {
    return E_INVALIDARG;
  }
  if (std::fabs(ClientSin(fov / 2)) < 0.01f) {
    return E_INVALIDARG;
  }

  const float width = aspect * (ClientCos(fov / 2) / ClientSin(fov / 2));
  const float height = 1.0f * (ClientCos(fov / 2) / ClientSin(fov / 2));
  const float depth = farPlane / (farPlane - nearPlane);

  ZeroMemory(&matrix, sizeof(D3DMATRIX));
  matrix._11 = width;
  matrix._22 = height;
  matrix._33 = depth;
  matrix._34 = 1.0f;
  matrix._43 = -depth * nearPlane;

  return S_OK;
}

// INLINED
static D3DDEVICEDESC7 SelectFogCaps(const D3DDEVICEDESC7& empty) {
  if (g_deviceOwnerDX7) {
    return g_deviceOwnerDX7->caps;
  }
  return empty;
}

// OFFSET: 0x0055CF10
// FUNCTION: fn_02094
void CRendererDX7::SetFog(int enabled) {
  D3DDEVICEDESC7 empty = g_emptyCapabilitiesDX7;
  const D3DDEVICEDESC7 caps = SelectFogCaps(empty);
  this->fog.supported = (caps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGVERTEX) != 0;
  this->fog.enabled = enabled;
  this->device->SetRenderState(D3DRENDERSTATE_FOGENABLE, enabled);
  this->fog.dirty = 1;
  g_fogRange = 1;
  g_fogVertexMode = D3DFOG_LINEAR;
}

// OFFSET: 0x0055A500
// FUNCTION: fn_02080
void CRendererDX7::ApplyFog() {
  if (!this->fog.dirty) {
    return;
  }

  if (this->fog.enabled) {
    D3DDEVICEDESC7 empty = g_emptyCapabilitiesDX7;
    D3DDEVICEDESC7 caps;
    if (g_deviceOwnerDX7) {
      caps = g_deviceOwnerDX7->caps;
    } else {
      caps = empty;
    }
    const float start = g_depthStart * 1490.0f + 10.0f;
    const float end = g_depthEnd * 1490.0f + 10.0f;
    const int vertexFog = (caps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGVERTEX) != 0;
    D3DMATRIX identity;
    identity._12 = identity._13 = identity._14 = identity._21 = identity._23 = identity._24 = identity._31 = identity._32 = identity._34 =
      identity._41 = identity._42 = identity._43 = 0.0f;
    identity._11 = identity._22 = identity._33 = identity._44 = 1.0f;
    D3DMATRIX projection;
    PerspectiveMatrixDX7(projection, 0.26179934f, this->viewport.aspect, 10.0f, 1500.0f);
    this->device->SetTransform(D3DTRANSFORMSTATE_WORLD, &identity);
    this->device->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &projection);
    this->device->SetTransform(D3DTRANSFORMSTATE_VIEW, &identity);
    this->device->SetRenderState(D3DRENDERSTATE_FOGCOLOR, g_worldOverlayColor);

    if (g_fogVertexMode == D3DFOG_LINEAR) {
      if (vertexFog) {
        this->device->SetRenderState(D3DRENDERSTATE_FOGSTART, std::bit_cast<unsigned>(start));
        this->device->SetRenderState(D3DRENDERSTATE_FOGEND, std::bit_cast<unsigned>(end));
        this->device->SetRenderState(D3DRENDERSTATE_FOGDENSITY, std::bit_cast<unsigned>(g_fogDensity));
        this->device->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_NONE);
        this->device->SetRenderState(D3DRENDERSTATE_FOGVERTEXMODE, g_fogVertexMode);
      } else {
        this->device->SetRenderState(D3DRENDERSTATE_FOGENABLE, 0);
      }
    } else {
      if (vertexFog) {
        this->device->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_NONE);
        this->device->SetRenderState(D3DRENDERSTATE_FOGVERTEXMODE, g_fogVertexMode);
      } else {
        ShowErrorDialog("Your graphic accelerator does not support fog effect.", 0);
      }

      this->device->SetRenderState(D3DRENDERSTATE_FOGDENSITY, std::bit_cast<unsigned>(g_fogDensity));
    }

    if (vertexFog && g_fogRange) {
      this->device->SetRenderState(D3DRENDERSTATE_RANGEFOGENABLE, 1);
    }
  }

  this->fog.dirty = 0;
}

// GLOBAL: gl_24699
D3DDEVICEDESC7 g_emptyCapabilitiesDX7;
