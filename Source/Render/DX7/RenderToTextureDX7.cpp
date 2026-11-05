#include "Render/DX7/RenderSystemDX7.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/RendererDX7.h"
#include "Render/Textures/TextureCache.h"
#include "Render/DX7/TextureDX7.h"

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

// OFFSET: 0x00544120
// FUNCTION: fn_01813
bool CTextureDX7::CopySurface(IDirectDrawSurface7* source, const RECT* rectangle) {
  return this->m_surface && SUCCEEDED(this->m_surface->Blt(const_cast<RECT*>(rectangle), source, const_cast<RECT*>(rectangle), DDBLT_WAIT, nullptr));
}

// OFFSET: 0x00813CA0
// FUNCTION: fn_10125
bool CRendererDX7::RenderToTexture(
  int width, int height, const std::list<RenderPrimitive*>& primitives, CTexture*& texture
) {
  if (!texture) {
    texture = CreateTexture(width, height, TextureFormat::A8R8G8B8, false, false);

    if (!texture) {
      return false;
    }
  }

  DDSURFACEDESC2 description{};
  description.dwSize = sizeof(description);
  description.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
  description.dwWidth = width;
  description.dwHeight = height;
  description.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_3DDEVICE | DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM;
  description.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
  description.ddpfPixelFormat.dwRGBBitCount = 32;
  description.ddpfPixelFormat.dwRBitMask = 0xFF0000;
  description.ddpfPixelFormat.dwGBitMask = 0xFF00;
  description.ddpfPixelFormat.dwBBitMask = 0xFF;
  description.ddpfPixelFormat.dwRGBAlphaBitMask = 0xFF000000;
  ComPtr<IDirectDrawSurface7> target;

  if (FAILED((g_deviceOwnerDX7 ? g_deviceOwnerDX7->draw : nullptr)->CreateSurface(&description, target.GetAddressOf(), nullptr))) {
    return false;
  }

  ComPtr<IDirectDrawSurface7> previous;
  if ((g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr)->GetRenderTarget(previous.GetAddressOf()) != S_OK) {
    return false;
  }

  DWORD depth = 0;
  if ((g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr)->GetRenderState(D3DRENDERSTATE_ZENABLE, &depth) != S_OK) {
    return false;
  }

  DWORD fog = 0;
  if ((g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr)->GetRenderState(D3DRENDERSTATE_FOGENABLE, &fog) != S_OK) {
    return false;
  }

  if ((g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr)->SetRenderTarget(target.Get(), 0) == S_OK) {
    (g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr)->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
    (g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr)->SetRenderState(D3DRENDERSTATE_FOGENABLE, 0);
    (g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr)->Clear(0, nullptr, D3DCLEAR_TARGET, 0, 0, 0);

    if ((g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr)->BeginScene() == S_OK) {
      for (auto* primitive : primitives) {
        auto* selected = static_cast<CTextureDX7*>(primitive->texture);

        this->SetTexture(0, selected);
        primitive->Draw(reinterpret_cast<int>(g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr));
      }

      (g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr)->EndScene();
      RECT rectangle{0, 0, width, height};
      static_cast<CTextureDX7*>(texture)->CopySurface(target.Get(), &rectangle);
    }

    (g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr)->SetRenderTarget(previous.Get(), 0);
    (g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr)->SetRenderState(D3DRENDERSTATE_ZENABLE, depth);
    (g_deviceOwnerDX7 ? g_deviceOwnerDX7->device : nullptr)->SetRenderState(D3DRENDERSTATE_FOGENABLE, fog);
  }

  return true;
}
