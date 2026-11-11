#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/D3DX9Runtime.h"
#include "Render/DX9/SurfaceDX9.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/DX9/TextureDX9.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <wrl/client.h>

namespace {
  // OFFSET: 0x00573170
  // FUNCTION: fn_02487
  std::uint16_t PackArgb4444(std::uint32_t color) {
    return ((color >> 16) & 0xF000) | ((color >> 12) & 0xF00) | ((color >> 8) & 0xF0) | ((color >> 4) & 0xF);
  }

  // OFFSET: 0x005731B0
  // FUNCTION: fn_02488
  std::uint16_t Color565(std::uint32_t color) {
    return ((color >> 8) & 0xF800) | ((color >> 5) & 0x7E0) | ((color >> 3) & 0x1F);
  }

  // INLINED
  int FormatBitsPerPixel(D3DFORMAT format) {
    switch (format) {
    case D3DFMT_R5G6B5:
    case D3DFMT_X1R5G5B5:
    case D3DFMT_A1R5G5B5:
    case D3DFMT_A4R4G4B4:
      return 16;
    case D3DFMT_A8R8G8B8:
    case D3DFMT_X8R8G8B8:
      return 32;
    default:
      return 0;
    }
  }

  // INLINED
  std::uint16_t Convert16(std::uint32_t color, D3DFORMAT format) {
    switch (format) {
    case D3DFMT_R5G6B5:
      return Color565(color);
    case D3DFMT_A1R5G5B5:
      return ((color >> 16) & 0x8000) | ((color & 255) >> 3) | ((color >> 6) & 0x3E0) | ((color >> 9) & 0x7C00);
    case D3DFMT_A4R4G4B4:
      return PackArgb4444(color);
    default:
      return 0;
    }
  }
}

// OFFSET: 0x00572500
// FUNCTION: fn_02474
CTextureDX9::CTextureDX9(DeviceOwnerDX9& owner)
    : m_owner(&owner), m_renderTargetUsage(false), m_dynamic(false), m_allowDynamic(false), m_renderTarget(false), m_texture(nullptr), m_surface(nullptr),
      m_description{} {
  this->m_textureWidth = this->m_width = 1;
  this->m_textureHeight = this->m_height = 1;
  this->m_format = TextureFormat::A1R5G5B5;
}

// OFFSET: 0x005725C0
// FUNCTION: fn_02475
CTextureDX9::CTextureDX9(DeviceOwnerDX9& owner, int width, int height, int format, bool dynamic, bool renderTarget) : CTextureDX9(owner) {
  this->m_allowDynamic = dynamic;
  this->m_renderTarget = renderTarget;
  this->Create(width, height, format);
  this->SetImageSize(width, height);
}

// OFFSET: 0x00572650
// FUNCTION: fn_02476
CTextureDX9::CTextureDX9(DeviceOwnerDX9& owner, IDirect3DTexture9* texture) : CTextureDX9(owner) {
  this->m_texture = texture;
  if (SUCCEEDED(texture->GetSurfaceLevel(0, &this->m_surface)) && SUCCEEDED(this->m_surface->GetDesc(&this->m_description))) {
    this->m_renderTargetUsage = (this->m_description.Usage & D3DUSAGE_RENDERTARGET) != 0;
    this->m_dynamic = (this->m_description.Usage & D3DUSAGE_AUTOGENMIPMAP) != 0;
    this->m_textureWidth = this->m_description.Width;
    this->m_textureHeight = this->m_description.Height;

    TextureFormat format;
    switch (this->m_description.Format) {
    case D3DFMT_A1R5G5B5:
      format = TextureFormat::A1R5G5B5;
      break;
    case D3DFMT_A4R4G4B4:
      format = TextureFormat::A4R4G4B4;
      break;
    case D3DFMT_R5G6B5:
      format = TextureFormat::R5G6B5;
      break;
    case D3DFMT_X1R5G5B5:
      format = TextureFormat::X1R5G5B5;
      break;
    case D3DFMT_A8R8G8B8:
      format = TextureFormat::A8R8G8B8;
      break;
    default:
      format = TextureFormat::UnknownTextureFormat;
      break;
    }
    this->m_format = format;

    this->m_height = this->m_width = this->m_description.Width;
  }
}

// INLINED
CTextureDX9::~CTextureDX9() {
  if (this->m_surface) {
    this->m_surface->Release();
    this->m_surface = nullptr;
  }

  if (this->m_texture) {
    this->m_texture->Release();
    this->m_texture = nullptr;
  }
}

// OFFSET: 0x00572AC0
// FUNCTION: fn_02480
bool CTextureDX9::Create(int width, int height, int format) {
  if (this->m_surface) {
    this->m_surface->Release();
    this->m_surface = nullptr;
  }

  if (this->m_texture) {
    this->m_texture->Release();
    this->m_texture = nullptr;
  }

  IDirect3DDevice9Ex* device = this->m_owner->device;
  D3DFORMAT nativeFormat = D3DFMT_UNKNOWN;

  switch (format) {
  case TextureFormat::A1R5G5B5:
    nativeFormat = D3DFMT_A1R5G5B5;
    break;
  case TextureFormat::A4R4G4B4:
    nativeFormat = D3DFMT_A4R4G4B4;
    break;
  case TextureFormat::R5G6B5:
    nativeFormat = D3DFMT_R5G6B5;
    break;
  case TextureFormat::X1R5G5B5:
    nativeFormat = D3DFMT_X1R5G5B5;
    break;
  case TextureFormat::A8R8G8B8:
    nativeFormat = D3DFMT_A8R8G8B8;
    break;
  }

  const bool renderTarget = this->m_renderTarget;
  DWORD usage = renderTarget ? 0 : D3DUSAGE_DYNAMIC;
  UINT size[2];
  const UINT* adjusted = this->m_owner->AdjustTextureDimensions(size, width, height);
  this->m_textureWidth = adjusted[0];
  this->m_textureHeight = adjusted[1];
  this->m_format = format;
  UINT levels = 1;

  if (this->m_allowDynamic && this->m_owner->dynamicTextures) {
    this->m_dynamic = this->m_owner->CanAutoGenerateMipmaps(nativeFormat);
    levels = 0;
    if (this->m_dynamic) {
      usage = renderTarget ? D3DUSAGE_AUTOGENMIPMAP : D3DUSAGE_DYNAMIC | D3DUSAGE_AUTOGENMIPMAP;
    }
  } else {
    this->m_dynamic = false;
  }

  if (FAILED(device->CreateTexture(this->m_textureWidth, this->m_textureHeight, levels, usage, nativeFormat, D3DPOOL_DEFAULT, &this->m_texture, nullptr)) ||
      FAILED(this->m_texture->GetSurfaceLevel(0, &this->m_surface)) || FAILED(this->m_surface->GetDesc(&this->m_description))) {
    return false;
  }

  if (!this->m_renderTarget) {
    for (UINT level = 0; level < this->m_texture->GetLevelCount(); ++level) {
      Microsoft::WRL::ComPtr<IDirect3DSurface9> surface;
      if (FAILED(this->m_texture->GetSurfaceLevel(level, surface.GetAddressOf()))) {
        break;
      }

      this->ClearSurface(surface.Get(), 0);
    }
  }

  return true;
}

// OFFSET: 0x00572D30
// FUNCTION: fn_02481
bool CTextureDX9::Resize(int width, int height) {
  return this->Create(width, height, this->m_format);
}

// OFFSET: 0x00573050
// FUNCTION: fn_02485
SurfaceLock CTextureDX9::Lock() {
  SurfaceLock output{};
  D3DLOCKED_RECT locked;
  if (this->m_surface && SUCCEEDED(this->m_surface->LockRect(&locked, nullptr, 0))) {
    output.pixels = locked.pBits;
    output.pitch = locked.Pitch;
    output.width = this->m_description.Width;
    output.height = this->m_description.Height;
    output.success = true;
    output.bitsPerPixel = FormatBitsPerPixel(this->m_description.Format);
  }

  return output;
}

// OFFSET: 0x00572D50
// FUNCTION: fn_02482
void CTextureDX9::Filter() {
  if (!this->m_dynamic && !this->m_renderTarget && this->m_texture && static_cast<int>(this->m_texture->GetLevelCount()) > 1) {
    D3DXFilterTexture(this->m_texture, nullptr, UINT(-1), DWORD(-1));
  }
}

// OFFSET: 0x005731E0
// FUNCTION: fn_02489
void CTextureDX9::Unlock() {
  if (this->m_surface && SUCCEEDED(this->m_surface->UnlockRect())) {
    this->Filter();
  }
}

// OFFSET: 0x00572D90
// FUNCTION: fn_02483
bool CTextureDX9::GetDC(HDC* output) {
  return this->m_surface && SUCCEEDED(this->m_surface->GetDC(output));
}

// OFFSET: 0x00573110
// FUNCTION: fn_02486
void CTextureDX9::ReleaseDC(HDC dc) {
  if (this->m_surface) {
    this->m_surface->ReleaseDC(dc);
    this->Filter();
  }
}

// OFFSET: 0x005737A0
// FUNCTION: fn_02492
void CTextureDX9::Update(const RECT*) {
  this->Filter();
}

// OFFSET: 0x00572940
// FUNCTION: fn_02478
void CTextureDX9::Fill(const RECT* rectangle, std::uint32_t color) {
  if (this->m_surface) {
    const RECT pixel{0, 0, 1, 1};
    if (SUCCEEDED(D3DXLoadSurfaceFromMemory(this->m_surface, nullptr, rectangle, &color, D3DFMT_A8R8G8B8, 4, nullptr, &pixel, 2, 0))) {
      this->Filter();
    }
  }
}

// OFFSET: 0x00572DC0
// FUNCTION: fn_02484
void CTextureDX9::ClearSurface(IDirect3DSurface9* surface, std::uint32_t color) {
  D3DSURFACE_DESC description;
  D3DLOCKED_RECT locked;
  if (FAILED(surface->GetDesc(&description)) || FAILED(surface->LockRect(&locked, nullptr, 0))) {
    return;
  }

  const UINT width = description.Width;
  const UINT height = description.Height;
  switch (this->m_description.Format) {
  case D3DFMT_A1R5G5B5: {
    const std::uint16_t value = PackColorA1R5G5B5(color);
    for (UINT y = 0; y < height; ++y) {
      auto* row = reinterpret_cast<std::uint16_t*>(static_cast<unsigned char*>(locked.pBits) + y * locked.Pitch);
      for (UINT x = 0; x < width; ++x) {
        row[x] = value;
      }
    }
    break;
  }
  case D3DFMT_A4R4G4B4: {
    const std::uint16_t value = PackArgb4444(color);
    for (UINT y = 0; y < height; ++y) {
      auto* row = reinterpret_cast<std::uint16_t*>(static_cast<unsigned char*>(locked.pBits) + y * locked.Pitch);
      for (UINT x = 0; x < width; ++x) {
        row[x] = value;
      }
    }
    break;
  }
  case D3DFMT_R5G6B5: {
    const std::uint16_t value = Color565(color);
    for (UINT y = 0; y < height; ++y) {
      auto* row = reinterpret_cast<std::uint16_t*>(static_cast<unsigned char*>(locked.pBits) + y * locked.Pitch);
      for (UINT x = 0; x < width; ++x) {
        row[x] = value;
      }
    }
    break;
  }
  case D3DFMT_A8R8G8B8:
    for (UINT y = 0; y < height; ++y) {
      auto* row = reinterpret_cast<std::uint32_t*>(static_cast<unsigned char*>(locked.pBits) + y * locked.Pitch);
      for (UINT x = 0; x < width; ++x) {
        row[x] = color;
      }
    }
    break;
  }

  surface->UnlockRect();
}

// OFFSET: 0x00573240
// FUNCTION: fn_02490
bool CTextureDX9::UploadSurface(
  IDirect3DSurface9* surface, int x, int y, int width, int height, const std::uint32_t* pixels, std::uint8_t blackTransparent, int sourceWidth
) {
  D3DSURFACE_DESC description;
  if (FAILED(surface->GetDesc(&description))) {
    return false;
  }

  const int stride = sourceWidth ? sourceWidth : width;
  int stepX = 1, stepY = 1;
  const int extentX = x + width;
  const int extentY = y + height;

  while (static_cast<UINT>(extentX / stepX) > this->m_width) {
    ++stepX;
  }

  while (static_cast<UINT>(extentY / stepY) > this->m_height) {
    ++stepY;
  }

  const int w = width / stepX, h = height / stepY;
  if (!w || !h) {
    return false;
  }

  const RECT rectangle{x / stepX, y / stepY, x / stepX + w, y / stepY + h};
  D3DLOCKED_RECT locked;
  if (FAILED(surface->LockRect(&locked, &rectangle, 0))) {
    return false;
  }

  switch (description.Format) {
  case D3DFMT_A1R5G5B5:
    for (int row = 0; row < h; ++row) {
      auto* output = static_cast<unsigned char*>(locked.pBits) + row * locked.Pitch;
      const std::uint32_t* input = pixels + row * stepY * stride;

      for (int column = 0; column < w; ++column) {
        std::uint16_t converted = Convert16(input[column * stepX], D3DFMT_A1R5G5B5);
        const std::uint16_t rgb = converted & 0x7FFF;
        if (rgb == 0x7C1F) {
          converted = 0;
        } else if (blackTransparent && !rgb) {
          converted = 0;
        }

        std::memcpy(output + column * sizeof(converted), &converted, sizeof(converted));
      }
    }
    break;
  case D3DFMT_A4R4G4B4:
    for (int row = 0; row < h; ++row) {
      auto* output = static_cast<unsigned char*>(locked.pBits) + row * locked.Pitch;
      const std::uint32_t* input = pixels + row * stepY * stride;

      for (int column = 0; column < w; ++column) {
        std::uint16_t converted = Convert16(input[column * stepX], D3DFMT_A4R4G4B4);
        const std::uint16_t rgb = converted & 0x0FFF;
        if (rgb == 0xF0F) {
          converted = 0;
        } else if (blackTransparent && !rgb) {
          converted = 0;
        }

        std::memcpy(output + column * sizeof(converted), &converted, sizeof(converted));
      }
    }
    break;
  case D3DFMT_R5G6B5:
    for (int row = 0; row < h; ++row) {
      auto* output = static_cast<unsigned char*>(locked.pBits) + row * locked.Pitch;
      const std::uint32_t* input = pixels + row * stepY * stride;

      for (int column = 0; column < w; ++column) {
        const std::uint16_t converted = Convert16(input[column * stepX], D3DFMT_R5G6B5);
        std::memcpy(output + column * sizeof(converted), &converted, sizeof(converted));
      }
    }
    break;
  case D3DFMT_A8R8G8B8:
    for (int row = 0; row < h; ++row) {
      auto* output = static_cast<unsigned char*>(locked.pBits) + row * locked.Pitch;
      const std::uint32_t* input = pixels + row * stepY * stride;

      for (int column = 0; column < w; ++column) {
        const std::uint32_t color = input[column * stepX];
        std::memcpy(output + column * sizeof(color), &color, sizeof(color));
      }
    }
    break;
  default:
    break;
  }

  return SUCCEEDED(surface->UnlockRect());
}

// OFFSET: 0x00573600
// FUNCTION: fn_02491
void CTextureDX9::Upload(int x, int y, int width, int height, const std::uint32_t* pixels, int option, int sourceWidth) {
  this->SetImageSize(width, height);
  std::uint8_t& transparent = reinterpret_cast<std::uint8_t*>(&this->textureFlags)[1];
  transparent = static_cast<std::uint8_t>(option);
  if (!pixels) {
    return;
  }

  IDirect3DDevice9Ex* device = this->m_owner->device;
  Microsoft::WRL::ComPtr<IDirect3DTexture9> temporary;
  Microsoft::WRL::ComPtr<IDirect3DSurface9> surface;

  if (this->m_renderTarget) {
    if (FAILED(device->CreateTexture(
          this->m_textureWidth, this->m_textureHeight, !this->m_owner->dynamicTextures, 0, this->m_description.Format, D3DPOOL_SYSTEMMEM,
          temporary.GetAddressOf(), nullptr
        ))) {
      return;
    }

    if (FAILED(temporary->GetSurfaceLevel(0, surface.GetAddressOf()))) {
      return;
    }

    this->ClearSurface(surface.Get(), 0);
  } else {
    surface = this->m_surface;
  }

  if (surface) {
    this->UploadSurface(surface.Get(), x, y, width, height, pixels, option, sourceWidth);
  }

  if (this->m_renderTarget && temporary) {
    if (temporary->GetLevelCount() > 1) {
      D3DXFilterTexture(temporary.Get(), nullptr, UINT(-1), DWORD(-1));
    }

    device->UpdateTexture(temporary.Get(), this->m_texture);
  } else {
    this->Filter();
  }
}

// OFFSET: 0x005737E0
// FUNCTION: fn_02493
void CTextureDX9::UploadSprite(int x, int y, int width, int height, const SpriteRleImage* rawImage, const std::uint32_t* palette) {
  if (!this->m_surface) {
    return;
  }

  RECT rectangle{x, y, x + width, y + height};
  D3DLOCKED_RECT locked;
  if (FAILED(this->m_surface->LockRect(&locked, &rectangle, 0))) {
    return;
  }

  if (this->m_description.Format == D3DFMT_A1R5G5B5) {
    for (int row = 0; row < height; ++row) {
      std::memset(static_cast<unsigned char*>(locked.pBits) + row * locked.Pitch, 0, width * sizeof(std::uint16_t));
    }
  } else if (this->m_description.Format == D3DFMT_A8R8G8B8) {
    for (int row = 0; row < height; ++row) {
      std::memset(static_cast<unsigned char*>(locked.pBits) + row * locked.Pitch, 0, width * sizeof(std::uint32_t));
    }
  }

  const SpriteRleImage& image = *rawImage;
  const std::uint8_t* input = image.runs;
  int remaining = 0;

  for (int row = 0; row < image.height; ++row) {
    auto* output = static_cast<unsigned char*>(locked.pBits) + row * locked.Pitch;

    for (int column = 0; column < image.width; ++column) {
      if (remaining) {
        --remaining;
        const std::uint16_t color = 0;
        std::memcpy(output + column * sizeof(color), &color, sizeof(color));
      } else {
        const unsigned value = *input++;
        if (value) {
          const std::uint16_t color = palette[value];
          std::memcpy(output + column * sizeof(color), &color, sizeof(color));
        } else {
          const std::uint16_t color = 0;
          std::memcpy(output + column * sizeof(color), &color, sizeof(color));
          remaining = *input++ - 1;
        }
      }
    }
  }

  if (SUCCEEDED(this->m_surface->UnlockRect())) {
    this->Filter();
  }
}

// OFFSET: 0x005729E0
// FUNCTION: fn_02479
bool CTextureDX9::CopyTexture(CTexture* source, int sourceX, int sourceY, int sourceWidth, int sourceHeight, int x, int y, int width, int height) {
  RECT src{sourceX, sourceY, sourceX + sourceWidth, sourceY + sourceHeight};
  RECT dst{x, y, x + width, y + height};
  if (FAILED(
        D3DXLoadSurfaceFromSurface(this->m_surface, nullptr, &dst, static_cast<CTextureDX9*>(source)->m_surface, nullptr, &src, 3, 0)
      )) {
    return false;
  }

  this->Filter();
  return true;
}

void CTextureDX9::Draw(int x, int y, int width, int height, std::uint32_t color) {
  this->DrawTo(
    g_renderer->primitives, g_renderer->queues, x, y, width, height, color, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
  );
}

void CTextureDX9::Stretch(int x, int y, int width, int height) {
  this->StretchTo(
    g_renderer->primitives, g_renderer->queues, x, y, width, height, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
  );
}

// OFFSET: 0x0056CEE0
// FUNCTION: fn_02352
std::unique_ptr<CTextureDX9> CreateTextureDX9(DeviceOwnerDX9** owner) {
  return std::unique_ptr<CTextureDX9>(new CTextureDX9(**owner));
}
