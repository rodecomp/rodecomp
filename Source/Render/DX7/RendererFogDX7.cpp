#include "Client/Application/ErrorDialog.h"
#include "Render/TextureQuad.h"
#include "Render/DX7/RendererFogDX7.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/RendererDX7.h"

#include <bit>
#include <cmath>

namespace {
  // OFFSET: 0x00556E50
  // FUNCTION: fn_02039
  float Sine(float angle) {
    return static_cast<float>(std::sin(static_cast<double>(angle)));
  }

  // OFFSET: 0x00556E40
  // FUNCTION: fn_02038
  float Cosine(float angle) {
    return static_cast<float>(std::cos(static_cast<double>(angle)));
  }
}

// OFFSET: 0x00556E60
// FUNCTION: fn_02040
float SquareRoot(float value) {
  return std::sqrt(value);
}

// OFFSET: 0x00548C30
// FUNCTION: fn_01863
HRESULT PerspectiveMatrixDX7(D3DMATRIX& matrix, float fov, float aspect, float nearPlane, float farPlane) {
  const float difference = farPlane - nearPlane;
  constexpr float epsilon = 0.01f;

  if (epsilon > std::fabs(difference)) {
    return E_INVALIDARG;
  }

  const float half = fov * 0.5f;

  if (epsilon > std::fabs(Sine(half))) {
    return E_INVALIDARG;
  }

  const float cosineX = Cosine(half);
  const float sineX = Sine(half);
  const float scaleX = static_cast<float>((static_cast<double>(cosineX) / sineX) * aspect);
  const float cosineY = Cosine(half);
  const float sineY = Sine(half);
  const float scaleY = static_cast<float>(static_cast<double>(cosineY) / sineY);
  const float depth = farPlane / difference;

  matrix._12 = matrix._13 = matrix._14 = 0;
  matrix._21 = matrix._23 = matrix._24 = 0;
  matrix._31 = matrix._32 = 0;
  matrix._41 = matrix._42 = matrix._44 = 0;
  matrix._34 = 1;
  matrix._33 = depth;
  const float translation = -depth * nearPlane;
  matrix._11 = scaleX;
  matrix._22 = scaleY;
  matrix._43 = translation;

  return S_OK;
}

// OFFSET: 0x0055CF10
// FUNCTION: fn_02094
void CRendererDX7::SetFog(int enabled) {
  const D3DDEVICEDESC7 empty = g_emptyCapabilitiesDX7;
  const D3DDEVICEDESC7 caps = g_deviceOwnerDX7 ? g_deviceOwnerDX7->caps : empty;
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
    const D3DDEVICEDESC7 caps = g_deviceOwnerDX7 ? g_deviceOwnerDX7->caps : empty;
    const float start = g_depthStart * 1490.0f + 10.0f;
    const float end = g_depthEnd * 1490.0f + 10.0f;
    const bool vertexFog = (caps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGVERTEX) != 0;
    D3DMATRIX identity{};
    identity._11 = identity._22 = identity._33 = identity._44 = 1;
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
