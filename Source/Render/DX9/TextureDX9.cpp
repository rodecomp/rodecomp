#include "Render/DX9/D3DX9Runtime.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/DX9/TextureDX9.h"

#include <algorithm>
#include <cstring>

namespace {
  // INLINED
  std::uint16_t Convert16(std::uint32_t color, D3DFORMAT format) {
    switch (format) {
    case D3DFMT_R5G6B5:
      return ((color & 255) >> 3) | ((color >> 5) & 0x7E0) | ((color >> 8) & 0xF800);
    case D3DFMT_A1R5G5B5:
      return ((color >> 16) & 0x8000) | ((color & 255) >> 3) | ((color >> 6) & 0x3E0) | ((color >> 9) & 0x7C00);
    case D3DFMT_A4R4G4B4:
      return ((color & 255) >> 4) | ((color >> 8) & 0xF0) | ((color >> 12) & 0xF00) | ((color >> 16) & 0xF000);
    default:
      return 0;
    }
  }

  // INLINED
  UINT RoundUpPowerOfTwo(UINT value) {
    --value;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;

    return value + 1 + (value == 0xFFFFFFFFu);
  }
}

// OFFSET: 0x00572500
// FUNCTION: fn_02474
CTextureDX9::CTextureDX9(TextureDX9Context& context)
    : m_context(&context), m_renderTargetUsage(false), m_dynamic(false), m_allowDynamic(false), m_renderTarget(false), m_texture(nullptr), m_surface(nullptr),
      m_description{} {
  this->m_textureWidth = this->m_textureHeight = this->m_width = this->m_height = 1;
  this->m_format = TextureFormat::A1R5G5B5;
}

// OFFSET: 0x005725C0
// FUNCTION: fn_02475
CTextureDX9::CTextureDX9(TextureDX9Context& context, int width, int height, int format, bool dynamic, bool renderTarget) : CTextureDX9(context) {
  this->m_allowDynamic = dynamic;
  this->m_renderTarget = renderTarget;
  this->Create(width, height, format);
  this->SetImageSize(width, height);
}

// OFFSET: 0x00572650
// FUNCTION: fn_02476
CTextureDX9::CTextureDX9(TextureDX9Context& context, IDirect3DTexture9* texture) : CTextureDX9(context) {
  this->m_texture = texture;
  if (SUCCEEDED(texture->GetSurfaceLevel(0, &this->m_surface)) && SUCCEEDED(this->m_surface->GetDesc(&this->m_description))) {
    this->m_renderTargetUsage = (this->m_description.Usage & D3DUSAGE_RENDERTARGET) != 0;
    this->m_dynamic = (this->m_description.Usage & D3DUSAGE_DYNAMIC) != 0;
    this->m_textureWidth = this->m_description.Width;
    this->m_textureHeight = this->m_description.Height;

    switch (this->m_description.Format) {
    case D3DFMT_A8R8G8B8:
      this->m_format = TextureFormat::A8R8G8B8;
      break;
    case D3DFMT_R5G6B5:
      this->m_format = TextureFormat::R5G6B5;
      break;
    case D3DFMT_X1R5G5B5:
      this->m_format = TextureFormat::X1R5G5B5;
      break;
    case D3DFMT_A1R5G5B5:
      this->m_format = TextureFormat::A1R5G5B5;
      break;
    case D3DFMT_A4R4G4B4:
      this->m_format = TextureFormat::A4R4G4B4;
      break;
    default:
      this->m_format = TextureFormat::UnknownTextureFormat;
      break;
    }

    this->m_width = this->m_height = this->m_description.Width;
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

  UINT w = std::min<UINT>(RoundUpPowerOfTwo(width), this->m_context->caps.MaxTextureWidth);
  UINT h = std::min<UINT>(RoundUpPowerOfTwo(height), this->m_context->caps.MaxTextureHeight);
  UINT ratio = this->m_context->caps.MaxTextureAspectRatio;

  if (ratio) {
    if (w / h > ratio) {
      h = w / ratio;
    }

    if (h / w > ratio) {
      w = h / ratio;
    }
  }

  this->m_textureWidth = w;
  this->m_textureHeight = h;
  this->m_format = format;
  DWORD usage = this->m_renderTarget ? 0 : D3DUSAGE_AUTOGENMIPMAP;
  D3DPOOL pool = D3DPOOL_MANAGED;
  this->m_dynamic = false;

  if (this->m_allowDynamic && this->m_context->dynamicTextures) {
    this->m_dynamic = this->m_context->direct3D && (this->m_context->caps.Caps2 & D3DCAPS2_DYNAMICTEXTURES) &&
                      this->m_context->direct3D->CheckDeviceFormat(
                        this->m_context->adapter, D3DDEVTYPE_HAL, this->m_context->displayFormat, D3DUSAGE_DYNAMIC, D3DRTYPE_TEXTURE, nativeFormat
                      ) == D3D_OK;
    pool = D3DPOOL_DEFAULT;
    if (this->m_dynamic) {
      usage = D3DUSAGE_DYNAMIC | (this->m_renderTarget ? 0 : D3DUSAGE_AUTOGENMIPMAP);
    }
  }

  if (FAILED(this->m_context->device->CreateTexture(w, h, 1, usage, nativeFormat, pool, &this->m_texture, nullptr)) ||
      FAILED(this->m_texture->GetSurfaceLevel(0, &this->m_surface)) || FAILED(this->m_surface->GetDesc(&this->m_description))) {
    return false;
  }

  if (!this->m_renderTarget) {
    for (UINT level = 0; level < this->m_texture->GetLevelCount(); ++level) {
      IDirect3DSurface9* surface = nullptr;
      if (FAILED(this->m_texture->GetSurfaceLevel(level, &surface))) {
        break;
      }

      this->ClearSurface(surface, 0);
      surface->Release();
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
    output = {true, locked.pBits, locked.Pitch, this->m_description.Width, this->m_description.Height, 0};
    if (this->m_description.Format == D3DFMT_A8R8G8B8 || this->m_description.Format == D3DFMT_X8R8G8B8) {
      output.bitsPerPixel = 32;
    } else if (this->m_description.Format >= D3DFMT_R5G6B5 && this->m_description.Format <= D3DFMT_A4R4G4B4) {
      output.bitsPerPixel = 16;
    }
  }

  return output;
}

// OFFSET: 0x00572D50
// OFFSET: 0x005737A0
// FUNCTION: fn_02482
void CTextureDX9::Filter() {
  if (!this->m_dynamic && !this->m_renderTarget && this->m_texture && this->m_texture->GetLevelCount() > 1) {
    this->m_context->d3dx->filterTexture(this->m_texture, nullptr, UINT(-1), DWORD(-1));
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
    if (SUCCEEDED(this->m_context->d3dx->loadSurfaceFromMemory(this->m_surface, nullptr, rectangle, &color, D3DFMT_A8R8G8B8, 4, nullptr, &pixel, 2, 0))) {
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

  for (UINT y = 0; y < description.Height; ++y) {
    auto* row = static_cast<unsigned char*>(locked.pBits) + y * locked.Pitch;
    if (this->m_description.Format == D3DFMT_A8R8G8B8) {
      for (UINT x = 0; x < description.Width; ++x) {
        std::memcpy(row + x * sizeof(color), &color, sizeof(color));
      }
    } else if (this->m_description.Format == D3DFMT_R5G6B5 || this->m_description.Format == D3DFMT_A1R5G5B5 || this->m_description.Format == D3DFMT_A4R4G4B4) {
      const std::uint16_t value = Convert16(color, this->m_description.Format);

      for (UINT x = 0; x < description.Width; ++x) {
        std::memcpy(row + x * sizeof(value), &value, sizeof(value));
      }
    }
  }

  surface->UnlockRect();
}

// OFFSET: 0x00573240
// FUNCTION: fn_02490
bool CTextureDX9::UploadSurface(
  IDirect3DSurface9* surface, int x, int y, int width, int height, const std::uint32_t* pixels, bool blackTransparent, int sourceWidth
) {
  D3DSURFACE_DESC description;
  if (FAILED(surface->GetDesc(&description))) {
    return false;
  }

  const int stride = sourceWidth ? sourceWidth : width;
  int stepX = 1, stepY = 1;
  int extentX = x + width;
  int extentY = y + height;

  if (static_cast<UINT>(extentX) > this->m_width) {
    do {
      ++stepX;
      extentX /= stepX;
    } while (static_cast<UINT>(extentX) > this->m_width);
  }

  if (static_cast<UINT>(extentY) > this->m_height) {
    do {
      ++stepY;
      extentY /= stepY;
    } while (static_cast<UINT>(extentY) > this->m_height);
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

  for (int row = 0; row < h; ++row) {
    auto* output = static_cast<unsigned char*>(locked.pBits) + row * locked.Pitch;

    for (int column = 0; column < w; ++column) {
      const std::uint32_t color = pixels[row * stepY * stride + column * stepX];
      if (description.Format == D3DFMT_A8R8G8B8) {
        std::memcpy(output + column * sizeof(color), &color, sizeof(color));
      } else if (description.Format == D3DFMT_R5G6B5) {
        const std::uint16_t converted = Convert16(color, description.Format);
        std::memcpy(output + column * sizeof(converted), &converted, sizeof(converted));
      } else if (description.Format == D3DFMT_A1R5G5B5 || description.Format == D3DFMT_A4R4G4B4) {
        std::uint16_t converted = Convert16(color, description.Format);
        const std::uint16_t rgb = description.Format == D3DFMT_A1R5G5B5 ? converted & 0x7FFF : converted & 0x0FFF;
        if (rgb == (description.Format == D3DFMT_A1R5G5B5 ? 0x7C1F : 0xF0F) || (blackTransparent && !rgb)) {
          converted = 0;
        }

        std::memcpy(output + column * sizeof(converted), &converted, sizeof(converted));
      }
    }
  }

  return SUCCEEDED(surface->UnlockRect());
}

// OFFSET: 0x00573600
// FUNCTION: fn_02491
void CTextureDX9::Upload(int x, int y, int width, int height, const std::uint32_t* pixels, int option, int sourceWidth) {
  this->SetImageSize(width, height);
  this->unknown_0008 = (this->unknown_0008 & 255) | (static_cast<unsigned char>(option) << 8);
  if (!pixels) {
    return;
  }

  IDirect3DTexture9* temporary = nullptr;
  IDirect3DSurface9* surface = nullptr;
  if (!this->m_renderTarget) {
    surface = this->m_surface;
    if (surface) {
      surface->AddRef();
    }
  } else if (SUCCEEDED(this->m_context->device->CreateTexture(
               this->m_textureWidth, this->m_textureHeight, !this->m_context->dynamicTextures, 0, this->m_description.Format, D3DPOOL_SYSTEMMEM, &temporary,
               nullptr
             )) &&
             SUCCEEDED(temporary->GetSurfaceLevel(0, &surface))) {
    this->ClearSurface(surface, 0);
  }

  if (surface) {
    this->UploadSurface(surface, x, y, width, height, pixels, option != 0, sourceWidth);
    if (this->m_renderTarget && temporary) {
      if (temporary->GetLevelCount() > 1) {
        this->m_context->d3dx->filterTexture(temporary, nullptr, UINT(-1), DWORD(-1));
      }

      this->m_context->device->UpdateTexture(temporary, this->m_texture);
    } else {
      this->Filter();
    }
  }

  if (surface) {
    surface->Release();
  }

  if (temporary) {
    temporary->Release();
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

  if (this->m_description.Format == D3DFMT_A1R5G5B5 || this->m_description.Format == D3DFMT_A8R8G8B8) {
    const int bytes = this->m_description.Format == D3DFMT_A1R5G5B5 ? 2 : 4;

    for (int row = 0; row < height; ++row) {
      std::memset(static_cast<unsigned char*>(locked.pBits) + row * locked.Pitch, 0, width * bytes);
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
        this->m_context->d3dx->loadSurfaceFromSurface(this->m_surface, nullptr, &dst, static_cast<CTextureDX9*>(source)->m_surface, nullptr, &src, 3, 0)
      )) {
    return false;
  }

  this->Filter();
  return true;
}

// OFFSET: 0x00544410
void CTextureDX9::Draw(int x, int y, int width, int height, std::uint32_t color) {
  this->DrawTo(
    *this->m_context->primitives, *this->m_context->queues, x, y, width, height, color, this->m_context->depthAlpha, this->m_context->depthStart,
    this->m_context->depthEnd
  );
}

// OFFSET: 0x00544610
void CTextureDX9::Stretch(int x, int y, int width, int height) {
  this->StretchTo(
    *this->m_context->primitives, *this->m_context->queues, x, y, width, height, this->m_context->depthAlpha, this->m_context->depthStart,
    this->m_context->depthEnd
  );
}
