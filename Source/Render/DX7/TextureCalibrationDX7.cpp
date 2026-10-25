#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/DX7/SurfaceDX7.h"
#include "Render/DX7/TextureDX7.h"

#include <bit>
#include <cstring>

namespace {
  unsigned Component(unsigned pixel, DWORD mask, const DeviceOwnerDX7::Channel& channel) {
    return ((pixel & mask) >> (channel.shift & 31)) << (channel.expansion & 31);
  }
}

// OFFSET: 0x0054B040
// FUNCTION: fn_01905
bool DeviceOwnerDX7::CalibrateTextureCoordinates(TextureDX7Context& context) {
  const std::uint32_t pattern[16] = {0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0};
  CSurfaceDX7 readback(*this, this->width, this->height);
  CTextureDX7 texture(context, 4, 4, TextureFormat::A1R5G5B5);
  texture.Upload(0, 0, 4, 4, pattern, 0, 0);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, 0);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
  this->device->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
  this->device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
  this->device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
  this->device->SetTexture(0, texture.NativeSurface());

  const unsigned columns = this->width >> 2;
  const unsigned capacity = columns * (this->height >> 2);
  int bestScore = -1024;
  unsigned best = 32896;
  unsigned first = 0;

  do {
    this->device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1, 0);

    if (FAILED(this->device->BeginScene())) {
      return false;
    }

    for (unsigned index = 0; index < capacity && first + index <= 65535; ++index) {
      const unsigned candidate = first + index;
      const float x = static_cast<float>(4 * (index % columns));
      const float y = static_cast<float>(4 * (index / columns));
      const float offsetX = static_cast<float>(candidate % 256) * (1.0f / 256.0f);
      const float offsetY = static_cast<float>(candidate / 256) * (1.0f / 256.0f);
      const float u0 = (offsetX + 0.0f) / static_cast<float>(texture.TextureWidth());
      const float v0 = (offsetY + 0.0f) / static_cast<float>(texture.TextureHeight());
      const float u1 = (offsetX + 4.0f) / static_cast<float>(texture.TextureWidth());
      const float v1 = (offsetY + 4.0f) / static_cast<float>(texture.TextureHeight());
      constexpr float z = 0.00001f;
      constexpr float rhw = 0.99999f;
      RenderVertex vertices[4] = {
        {x, y, z, rhw, 0xFFFFFFFF, 0xFFFFFFFF, u0, v0},
        {x, y + 4, z, rhw, 0xFFFFFFFF, 0xFFFFFFFF, u0, v1},
        {x + 4, y, z, rhw, 0xFFFFFFFF, 0xFFFFFFFF, u1, v0},
        {x + 4, y + 4, z, rhw, 0xFFFFFFFF, 0xFFFFFFFF, u1, v1}
      };
      this->device->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, vertices, 4, 0);
    }

    this->device->EndScene();
    readback.m_surface->Blt(nullptr, this->renderTarget, nullptr, DDBLT_WAIT, nullptr);
    DDSURFACEDESC2 locked{};
    locked.dwSize = sizeof(locked);

    if (FAILED(readback.m_surface->Lock(nullptr, &locked, DDLOCK_WAIT, nullptr))) {
      return false;
    }

    const unsigned pixelBytes = locked.ddpfPixelFormat.dwRGBBitCount >> 3;

    for (unsigned index = 0; index < capacity && first + index <= 65535; ++index) {
      const unsigned x = 4 * (index % columns);
      const unsigned y = 4 * (index / columns);
      unsigned dark = 0;
      unsigned bright = 0;

      for (unsigned sample = 0; sample < 16; ++sample) {
        const auto* source = static_cast<const unsigned char*>(locked.lpSurface) + (y + sample / 4) * locked.lPitch + (x + sample % 4) * pixelBytes;
        unsigned pixel;

        if (pixelBytes == 2) {
          std::uint16_t value;
          std::memcpy(&value, source, 2);
          pixel = value;
        } else {
          std::memcpy(&pixel, source, 4);
        }

        pixel = static_cast<std::uint16_t>(pixel);
        const unsigned value =
          Component(pixel, this->greenMask, this->green) + Component(pixel, this->blueMask, this->blue) + Component(pixel, this->redMask, this->red);

        if (pattern[sample] == 0) {
          dark += value;
        } else {
          bright += value;
        }
      }

      const int lightScore = std::bit_cast<int>(bright);
      const int darkScore = std::bit_cast<int>(dark);
      const int difference = std::bit_cast<int>(bright - dark);

      if (lightScore >= 5760 && darkScore <= 384 && difference > bestScore) {
        bestScore = difference;
        best = first + index;
      }
    }

    readback.m_surface->Unlock(nullptr);
    first += capacity;
  } while (first < 65536);

  g_textureOffsetX = static_cast<float>(best % 256) * (1.0f / 256.0f);
  g_textureOffsetY = static_cast<float>(best / 256) * (1.0f / 256.0f);
  this->device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
  this->device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);

  return true;
}
