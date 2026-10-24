#include "Render/DeviceOwnerDX7.h"
#include "Render/RenderPrimitive.h"
#include "Render/RendererDX7Operations.h"
#include "Render/TextureBindingDX7.h"
#include "Render/TextureDX7.h"

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

// OFFSET: 0x00544120
// FUNCTION: fn_01813
bool CTextureDX7::CopySurface(IDirectDrawSurface7* source, const RECT* rectangle) {
  return this->m_surface && SUCCEEDED(this->m_surface->Blt(
                              const_cast<RECT*>(rectangle), source, const_cast<RECT*>(rectangle), DDBLT_WAIT, nullptr
                            ));
}

// OFFSET: 0x00813CA0
// FUNCTION: fn_10125
bool RendererDX7Operations::RenderToTexture(
  int width,
  int height,
  const std::list<RenderPrimitive*>& primitives,
  CTexture*& texture,
  TextureDX7Context& context,
  TextureBindingDX7& binding
) {
  if (!texture) {
    texture = new CTextureDX7(context, width, height, 4);

    if (!texture) {
      return false;
    }
  }

  DDSURFACEDESC2 description{};
  description.dwSize = sizeof(description);
  description.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
  description.dwWidth = width;
  description.dwHeight = height;
  description.ddsCaps.dwCaps = 0x10007000;
  description.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
  description.ddpfPixelFormat.dwRGBBitCount = 32;
  description.ddpfPixelFormat.dwRBitMask = 0xFF0000;
  description.ddpfPixelFormat.dwGBitMask = 0xFF00;
  description.ddpfPixelFormat.dwBBitMask = 0xFF;
  description.ddpfPixelFormat.dwRGBAlphaBitMask = 0xFF000000;
  ComPtr<IDirectDrawSurface7> target;

  if (FAILED(this->owner.draw->CreateSurface(&description, target.GetAddressOf(), nullptr))) {
    return false;
  }

  ComPtr<IDirectDrawSurface7> previous;
  DWORD depth = 0;
  DWORD fog = 0;
  auto* device = this->owner.device;

  if (
    device->GetRenderTarget(previous.GetAddressOf()) != S_OK ||
    device->GetRenderState(D3DRENDERSTATE_ZENABLE, &depth) != S_OK ||
    device->GetRenderState(D3DRENDERSTATE_FOGENABLE, &fog) != S_OK
  ) {
    return false;
  }

  if (device->SetRenderTarget(target.Get(), 0) == S_OK) {
    device->SetRenderState(D3DRENDERSTATE_ZENABLE, 0);
    device->SetRenderState(D3DRENDERSTATE_FOGENABLE, 0);
    device->Clear(0, nullptr, D3DCLEAR_TARGET, 0, 0, 0);

    if (device->BeginScene() == S_OK) {
      for (auto* primitive : primitives) {
        auto* selected = static_cast<CTextureDX7*>(primitive->texture);

        binding.Set(device, 0, selected);

        this->DrawPrimitive(primitive);
      }

      device->EndScene();
      RECT rectangle{0, 0, width, height};
      static_cast<CTextureDX7*>(texture)->CopySurface(target.Get(), &rectangle);
    }

    device->SetRenderTarget(previous.Get(), 0);
    device->SetRenderState(D3DRENDERSTATE_ZENABLE, depth);
    device->SetRenderState(D3DRENDERSTATE_FOGENABLE, fog);
  }

  // Original reports success even when target switching or BeginScene failed.
  // It retains texture binding and does not generate output mipmaps here.
  return true;
}
