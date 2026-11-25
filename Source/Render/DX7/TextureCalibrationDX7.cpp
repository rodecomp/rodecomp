#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/DX7/SurfaceDX7.h"
#include "Render/DX7/TextureDX7.h"
#include "Render/Projection/ScreenGeometry.h"

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
  std::uint32_t pattern[16] = {0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0};
  CSurfaceDX7 readback(*this, this->width, this->height);
  CTextureDX7 texture(*this, 4, 4, TextureFormat::A1R5G5B5);
  texture.Upload(0, 0, 4, 4, pattern, 0, 0);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, 0);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
  this->device->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
  this->device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
  this->device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
  this->device->SetTexture(0, texture.NativeSurface());

  unsigned i = 0, columns = this->width >> 2;
  unsigned rows = this->height >> 2;
  int bestScore = -1024;
  int best = 0x8080;

  while (i < 0x10000) {
    DDSURFACEDESC2 locked;
    unsigned j;
    int pixelBytes;
    this->device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    if (FAILED(this->device->BeginScene())) {
      return false;
    }

    for (j = 0; j < columns * rows; ++j) {
      if (static_cast<int>(i + j) > 0xFFFF) {
        break;
      }

      int x = static_cast<int>(j % columns) * 4;
      int y = static_cast<int>(j / columns) * 4;
      float tu = static_cast<float>(static_cast<int>(i + j) % 256) * 0.00390625f;
      float tv = static_cast<float>(static_cast<int>(i + j) / 256) * 0.00390625f;

      ScreenPoint xy[2] = {ScreenPoint(static_cast<float>(x), static_cast<float>(y)), ScreenPoint(static_cast<float>(x + 4), static_cast<float>(y + 4))};
      ScreenPoint uv[2] = {
        ScreenPoint((tu + 0.0f) / texture.TextureWidth(), (tv + 0.0f) / texture.TextureHeight()),
        ScreenPoint((tu + g_effectIconRadius) / texture.TextureWidth(), (tv + g_effectIconRadius) / texture.TextureHeight())};
      RenderVertex vertices[4] = {
        {xy[0].x, xy[0].y, 0.00001f, 0.99999f, 0xFFFFFFFF, 0xFFFFFFFF, uv[0].x, uv[0].y},
        {xy[0].x, xy[1].y, 0.00001f, 0.99999f, 0xFFFFFFFF, 0xFFFFFFFF, uv[0].x, uv[1].y},
        {xy[1].x, xy[0].y, 0.00001f, 0.99999f, 0xFFFFFFFF, 0xFFFFFFFF, uv[1].x, uv[0].y},
        {xy[1].x, xy[1].y, 0.00001f, 0.99999f, 0xFFFFFFFF, 0xFFFFFFFF, uv[1].x, uv[1].y}
      };

      this->device->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, vertices, 4, 0);
    }

    this->device->EndScene();
    readback.m_surface->Blt(nullptr, this->renderTarget, nullptr, DDBLT_WAIT, nullptr);

    locked.dwSize = sizeof(locked);
    locked.dwFlags = 0;
    ZeroMemory(&locked.dwHeight, sizeof(locked) - 8);
    if (FAILED(readback.m_surface->Lock(nullptr, &locked, DDLOCK_WAIT, nullptr))) {
      return false;
    }

    pixelBytes = locked.ddpfPixelFormat.dwRGBBitCount >> 3;
    for (j = 0; j < columns * rows; ++j) {
      if (static_cast<int>(i + j) > 0xFFFF) {
        break;
      }

      int bright = 0;
      int dark = 0;

      for (int k = 0; k < 16; ++k) {
        int x = static_cast<int>(j % columns) * 4 + k % 4;
        int y = static_cast<int>(j / columns) * 4 + k / 4;
        DWORD pixel;
        if (pixelBytes == 2) {
          pixel = *reinterpret_cast<WORD*>(static_cast<BYTE*>(locked.lpSurface) + y * locked.lPitch + x * 2);
        } else {
          pixel = *reinterpret_cast<DWORD*>(static_cast<BYTE*>(locked.lpSurface) + y * locked.lPitch + x * pixelBytes);
        }

        if (pattern[k]) {
          bright += this->UnpackBlue(static_cast<WORD>(pixel)) + this->UnpackGreen(static_cast<WORD>(pixel)) + this->UnpackRed(static_cast<WORD>(pixel));
        } else {
          dark += this->UnpackBlue(static_cast<WORD>(pixel)) + this->UnpackGreen(static_cast<WORD>(pixel)) + this->UnpackRed(static_cast<WORD>(pixel));
        }
      }

      if (bright >= 0x1680 && dark <= 0x180) {
        int difference = bright - dark;
        if (difference > bestScore) {
          bestScore = difference;
          best = static_cast<int>(i + j);
        }
      }
    }
    readback.m_surface->Unlock(nullptr);

    i += columns * rows;
  }

  g_textureOffsetX = static_cast<float>(best % 256) * 0.00390625f;
  g_textureOffsetY = static_cast<float>(best / 256) * 0.00390625f;
  this->device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
  this->device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
  return true;
}
