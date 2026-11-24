#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/DX7/SurfaceDX7.h"
#include "Render/DX7/TextureDX7.h"

#include <bit>
#include <cstring>

namespace {
  // INLINED
  unsigned Component(unsigned pixel, DWORD mask, const DeviceOwnerDX7::Channel& channel) {
    return ((pixel & mask) >> channel.shift) << channel.expansion;
  }
}

// OFFSET: 0x0054A740
// FUNCTION: fn_45916
DWORD DeviceOwnerDX7::UnpackBlue(std::uint16_t pixel) const {
  return Component(pixel, this->blueMask, this->blue);
}

// OFFSET: 0x0054A770
// FUNCTION: fn_45917
DWORD DeviceOwnerDX7::UnpackGreen(std::uint16_t pixel) const {
  return Component(pixel, this->greenMask, this->green);
}

// OFFSET: 0x0054A7A0
// FUNCTION: fn_45918
DWORD DeviceOwnerDX7::UnpackRed(std::uint16_t pixel) const {
  return Component(pixel, this->redMask, this->red);
}

// OFFSET: 0x0054B040
// FUNCTION: fn_01905
bool DeviceOwnerDX7::CalibrateTextureCoordinates() {
  const std::uint32_t pattern[16] = {0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0};
  CSurfaceDX7 readback(*this, this->width, this->height);
  CTextureDX7 texture(*this, 4, 4, TextureFormat::A1R5G5B5);
  texture.Upload(0, 0, 4, 4, pattern, 0, 0);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, 0);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
  this->device->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
  this->device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
  this->device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
  this->device->SetTexture(0, texture.NativeSurface());

  unsigned first = 0;
  const unsigned columns = this->width >> 2;
  const unsigned capacity = columns * (this->height >> 2);
  int bestScore = -1024;
  int best = 32896;
  bool succeeded = true;

  do {
    this->device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1, 0);

    if (FAILED(this->device->BeginScene())) {
      succeeded = false;
      break;
    }

    for (unsigned index = 0; index < capacity; ++index) {
      const int candidate = first + index;
      if (candidate > 65535) {
        break;
      }

      const int left = 4 * (index % columns);
      const int top = 4 * (index / columns);
      const float offsetX = static_cast<float>(candidate % 256) * (1.0f / 256.0f);
      const float offsetY = static_cast<float>(candidate / 256) * (1.0f / 256.0f);
      const float textureWidth = static_cast<float>(texture.TextureWidth());
      const float textureHeight = static_cast<float>(texture.TextureHeight());
      const float u0 = (offsetX + 0.0f) / textureWidth;
      const float v0 = (offsetY + 0.0f) / textureHeight;
      const float u1 = (offsetX + g_effectIconRadius) / textureWidth;
      const float v1 = (offsetY + g_effectIconRadius) / textureHeight;
      constexpr float z = 0.00001f;
      constexpr float rhw = 0.99999f;
      RenderVertex vertices[4] = {
        {static_cast<float>(left), static_cast<float>(top), z, rhw, 0xFFFFFFFF, 0xFFFFFFFF, u0, v0},
        {static_cast<float>(left), static_cast<float>(top + 4), z, rhw, 0xFFFFFFFF, 0xFFFFFFFF, u0, v1},
        {static_cast<float>(left + 4), static_cast<float>(top), z, rhw, 0xFFFFFFFF, 0xFFFFFFFF, u1, v0},
        {static_cast<float>(left + 4), static_cast<float>(top + 4), z, rhw, 0xFFFFFFFF, 0xFFFFFFFF, u1, v1}
      };
      this->device->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, vertices, 4, 0);
    }

    this->device->EndScene();
    readback.m_surface->Blt(nullptr, this->renderTarget, nullptr, DDBLT_WAIT, nullptr);
    DDSURFACEDESC2 locked = {sizeof(DDSURFACEDESC2)};

    if (FAILED(readback.m_surface->Lock(nullptr, &locked, DDLOCK_WAIT, nullptr))) {
      succeeded = false;
      break;
    }

    const unsigned pixelBytes = locked.ddpfPixelFormat.dwRGBBitCount >> 3;

    for (unsigned index = 0; index < capacity; ++index) {
      const int candidate = first + index;
      if (candidate > 65535) {
        break;
      }

      const unsigned x = 4 * (index % columns);
      const unsigned y = 4 * (index / columns);
      int dark = 0;
      int bright = 0;

      for (int sample = 0; sample < 16; ++sample) {
        const auto* source = static_cast<const unsigned char*>(locked.lpSurface) + (y + sample / 4) * locked.lPitch + (x + sample % 4) * pixelBytes;
        unsigned pixel;

        if (pixelBytes == 2) {
          std::uint16_t value;
          std::memcpy(&value, source, 2);
          pixel = value;
        } else {
          std::memcpy(&pixel, source, 4);
        }

        const auto packed = static_cast<std::uint16_t>(pixel);
        const int value = this->UnpackGreen(packed) + this->UnpackBlue(packed) + this->UnpackRed(packed);

        if (pattern[sample] == 0) {
          dark += value;
        } else {
          bright += value;
        }
      }

      if (bright >= 5760 && dark <= 384 && bright - dark > bestScore) {
        bestScore = bright - dark;
        best = candidate;
      }
    }

    readback.m_surface->Unlock(nullptr);
    first += capacity;
  } while (first < 65536);

  if (succeeded) {
    g_textureOffsetX = static_cast<float>(best % 256) * (1.0f / 256.0f);
    g_textureOffsetY = static_cast<float>(best / 256) * (1.0f / 256.0f);
    this->device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
    this->device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
  }

  return succeeded;
}
