#include "Client/Application/ErrorDialog.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/DX7/TextureDX7.h"
#include "Render/DX7/TextureAllocationDX7.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"

#include <climits>
#include <cstring>
#include <memory>
#include <new>

namespace {
  // STATIC: 0x01387270
  DDPIXELFORMAT formats[5] = {
    {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX},
    {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX},
    {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX},
    {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX},
    {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX},
  };

  // OFFSET: 0x00544B50
  // FUNCTION: fn_01824
  HRESULT CALLBACK SelectFormat(DDPIXELFORMAT* candidate, void* context) {
    auto* format = static_cast<unsigned*>(context);

    if (!candidate || !format) {
      return D3DENUMRET_OK;
    }

    switch (*format) {
    case TextureFormat::A1R5G5B5:
      if ((candidate->dwFlags & DDPF_ALPHAPIXELS) && candidate->dwRGBBitCount == 16 && candidate->dwRGBAlphaBitMask == 0x8000) {
        formats[TextureFormat::A1R5G5B5] = *candidate;
        return D3DENUMRET_CANCEL;
      }
      break;
    case TextureFormat::A4R4G4B4:
      if ((candidate->dwFlags & DDPF_ALPHAPIXELS) && candidate->dwRGBBitCount == 16 && candidate->dwRGBAlphaBitMask == 0xF000) {
        formats[TextureFormat::A4R4G4B4] = *candidate;
        return D3DENUMRET_CANCEL;
      }
      break;
    case TextureFormat::R5G6B5:
      if (!(candidate->dwFlags & DDPF_ALPHAPIXELS) && candidate->dwRGBBitCount == 16 && candidate->dwRBitMask == 0xF800) {
        formats[TextureFormat::R5G6B5] = *candidate;
        return D3DENUMRET_CANCEL;
      }
      break;
    case TextureFormat::A8R8G8B8:
      if ((candidate->dwFlags & DDPF_ALPHAPIXELS) && candidate->dwRGBBitCount == 32 && candidate->dwRGBAlphaBitMask == 0xFF000000) {
        formats[TextureFormat::A8R8G8B8] = *candidate;
        return D3DENUMRET_CANCEL;
      }
      break;
    }

    return D3DENUMRET_OK;
  }

  // INLINED
  RECT Rectangle(int x, int y, int width, int height) {
    return {x, y, x + width, y + height};
  }

}

  // OFFSET: 0x0056FCE0
  // FUNCTION: fn_02410
  void DecodeSprite(const SpriteRleImage& image, const std::uint32_t* palette, void* pixels, int pitch) {
    const auto* input = image.runs;
    auto* output = static_cast<unsigned char*>(pixels);
    int remaining = 0;

    for (int y = 0; y < image.height; ++y) {
      auto* row = output;

      for (int x = 0; x < image.width; ++x) {
        if (remaining) {
          const std::uint16_t value = 0;
          std::memcpy(row + x * sizeof(value), &value, sizeof(value));
          --remaining;
        } else {
          if (*input) {
            const std::uint16_t value = palette[*input];
            std::memcpy(row + x * sizeof(value), &value, sizeof(value));
          } else {
            ++input;
            const std::uint16_t value = 0;
            std::memcpy(row + x * sizeof(value), &value, sizeof(value));
            remaining = static_cast<unsigned>(*input) - 1;
          }

          ++input;
        }
      }

      output += pitch;
    }
  }

// OFFSET: 0x00543FC0
// FUNCTION: fn_45883
bool MatchesTextureFormatDX7(const DDPIXELFORMAT& format, TextureFormat requested) {
  switch (requested) {
  case TextureFormat::A1R5G5B5:
    if (format.dwRGBAlphaBitMask == 0x8000 && format.dwRBitMask == 0x7C00 && format.dwGBitMask == 0x03E0 &&
        format.dwBBitMask == 0x001F) {
      break;
    }
    return false;
  case TextureFormat::A4R4G4B4:
    if (format.dwRGBAlphaBitMask == 0xF000 && format.dwRBitMask == 0x0F00 && format.dwGBitMask == 0x00F0 &&
        format.dwBBitMask == 0x000F) {
      break;
    }
    return false;
  default:
    return false;
  case TextureFormat::R5G6B5:
    if (format.dwRGBAlphaBitMask == 0 && format.dwRBitMask == 0xF800 && format.dwGBitMask == 0x07E0 &&
        format.dwBBitMask == 0x001F) {
      break;
    }
    return false;
  case TextureFormat::A8R8G8B8:
    if (format.dwRGBAlphaBitMask == 0xFF000000 && format.dwRBitMask == 0x00FF0000 &&
        format.dwGBitMask == 0x0000FF00 && format.dwBBitMask == 0x000000FF) {
      break;
    }
    return false;
  }
  return true;
}

// OFFSET: 0x00543B00
// FUNCTION: fn_01807
CTextureDX7::CTextureDX7(DeviceOwnerDX7& owner) : m_owner(&owner), m_surface(nullptr) {
  this->m_textureWidth = this->m_width = 1;
  this->m_textureHeight = this->m_height = 1;
  this->m_format = TextureFormat::A1R5G5B5;
}

// OFFSET: 0x00543B90
// FUNCTION: fn_01808
CTextureDX7::CTextureDX7(DeviceOwnerDX7& owner, int width, int height, int format) : m_owner(&owner), m_surface(nullptr) {
  this->Create(width, height, format);
  this->SetImageSize(width, height);
}

// OFFSET: 0x0056AF60
// FUNCTION: fn_02306
std::unique_ptr<CTextureDX7> CreateTextureDX7(DeviceOwnerDX7*& owner) {
  return std::unique_ptr<CTextureDX7>(new CTextureDX7(*owner));
}

// OFFSET: 0x00543DC0
// FUNCTION: fn_01809
CTextureDX7::~CTextureDX7() {
  if (this->m_surface) {
    this->m_surface->Release();
    this->m_surface = nullptr;
  }
}

// OFFSET: 0x00544230
// FUNCTION: fn_01815
__forceinline bool CTextureDX7::Create(int width, int height, int format) {
  unsigned selected = format;
  this->m_textureWidth = width;
  this->m_textureHeight = height;
  this->m_format = format;
  this->m_owner->AdjustTextureDimensions(this->m_textureWidth, this->m_textureHeight);

  if (this->m_surface) {
    this->m_surface->Release();
    this->m_surface = nullptr;
  }

  if (selected != TextureFormat::A1R5G5B5 && selected != TextureFormat::A4R4G4B4 && selected != TextureFormat::A8R8G8B8 && selected != TextureFormat::R5G6B5) {
    ShowErrorDialog("Create Texture::Unsupported pixelformat", 0);
  }

  if (formats[selected].dwSize != sizeof(DDPIXELFORMAT)) {
    this->m_owner->device->EnumTextureFormats(SelectFormat, &selected);
  }

  if (formats[selected].dwSize != sizeof(DDPIXELFORMAT)) {
    ShowErrorDialog("GFC 를 구동할 수 없는 비디오시스템입니다", 0);
  }

  DDSURFACEDESC2 description = {sizeof(description), 0};
  description.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
  description.dwWidth = this->m_textureWidth;
  description.dwFlags = (selected == TextureFormat::R5G6B5 ? DDSD_CKSRCBLT : 0) | g_textureAllocationDX7.descriptorFlags | DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH |
                        DDSD_PIXELFORMAT | DDSD_TEXTURESTAGE;
  description.ddsCaps.dwCaps = g_textureAllocationDX7.surfaceCaps | DDSCAPS_TEXTURE;
  description.dwHeight = this->m_textureHeight;
  description.dwTextureStage = 0;
  description.ddpfPixelFormat = formats[selected];
  description.ddckCKSrcBlt.dwColorSpaceLowValue = description.ddpfPixelFormat.dwBBitMask | description.ddpfPixelFormat.dwRBitMask;
  description.ddckCKSrcBlt.dwColorSpaceHighValue = description.ddckCKSrcBlt.dwColorSpaceLowValue;
  description.dwMipMapCount = 1;

  while ((1u << description.dwMipMapCount) < this->m_textureWidth) {
    if ((1u << description.dwMipMapCount) >= this->m_textureHeight) {
      break;
    }

    ++description.dwMipMapCount;
  }

  return SUCCEEDED(this->m_owner->draw->CreateSurface(&description, &this->m_surface, nullptr));
}

// OFFSET: 0x005443F0
// FUNCTION: fn_01816
bool CTextureDX7::Resize(int width, int height) {
  return this->Create(width, height, this->m_format);
}

// OFFSET: 0x00544AA0
// FUNCTION: fn_01822
SurfaceLock CTextureDX7::Lock() {
  SurfaceLock output{};

  if (!this->m_surface) {
    return output;
  }

  DDSURFACEDESC2 description{};
  if (FAILED(this->m_surface->Lock(nullptr, &description, DDLOCK_WAIT, nullptr))) {
    return output;
  }

  output.success = true;
  output.pixels = description.lpSurface;
  output.pitch = description.lPitch;
  output.width = description.dwWidth;
  output.height = description.dwHeight;
  output.bitsPerPixel = description.ddpfPixelFormat.dwRGBBitCount;
  return output;
}

// OFFSET: 0x00544C70
// FUNCTION: fn_01825
void CTextureDX7::Unlock() {
  if (this->m_surface) {
    this->m_surface->Unlock(nullptr);
  }
}

// OFFSET: 0x00544A70
// FUNCTION: fn_01821
bool CTextureDX7::GetDC(HDC* output) {
  if (!this->m_surface) {
    return false;
  }

  return SUCCEEDED(this->m_surface->GetDC(output));
}

// OFFSET: 0x00544B30
// FUNCTION: fn_01823
void CTextureDX7::ReleaseDC(HDC dc) {
  if (this->m_surface) {
    this->m_surface->ReleaseDC(dc);
  }
}

// OFFSET: 0x00544C90
// FUNCTION: fn_01826
void CTextureDX7::Upload(int x, int y, int width, int height, const std::uint32_t* pixels, int option, int sourceWidth) {
  this->SetImageSize(width, height);
  this->transparent = static_cast<std::uint8_t>(option);
  if (!this->m_surface) {
    return;
  }
  if (pixels == nullptr) {
    return;
  }

  if (sourceWidth == 0) {
    sourceWidth = width;
  }

  DDSURFACEDESC2 description = {sizeof(DDSURFACEDESC2), 0};
  if (FAILED(this->m_surface->GetSurfaceDesc(&description))) {
    return;
  }

  int stepX;
  int stepY;
  for (stepX = 1; static_cast<unsigned>(width / stepX) > description.dwWidth; ++stepX) {
  }
  for (stepY = 1; static_cast<unsigned>(height / stepY) > description.dwHeight; ++stepY) {
  }

  x = x / stepX;
  y = y / stepY;
  width = width / stepX;
  height = height / stepY;

  if (width == 0 || height == 0) {
    return;
  }

  RECT rectangle = {x, y, x + width, y + height};
  if (FAILED(this->m_surface->Lock(&rectangle, &description, DDLOCK_WAIT | DDLOCK_WRITEONLY, nullptr))) {
    return;
  }

  std::uint32_t color;
  std::uint16_t* surface;
  std::uint32_t* surface32;
  int row;
  int column;
  int source;
  switch (this->m_format) {
  case TextureFormat::A1R5G5B5:
    surface = static_cast<std::uint16_t*>(description.lpSurface);
    for (row = 0; row < height; ++row) {
      for (column = 0, source = 0; column < width; ++column, source += stepX) {
        color = ((pixels[source] >> 9) & 0x7C00) + ((pixels[source] >> 6) & 0x03E0) + ((pixels[source] >> 3) & 0x001F) + ((pixels[source] >> 16) & 0x8000);

        if (color == 0xFC1F) {
          surface[column] = 0;
        } else if (this->transparent && (color & 0x7FFF) == 0) {
          surface[column] = 0;
        } else {
          surface[column] = static_cast<std::uint16_t>(color);
        }
      }
      surface = reinterpret_cast<std::uint16_t*>(reinterpret_cast<unsigned char*>(surface) + description.lPitch);
      pixels += sourceWidth * stepY;
    }
    break;
  case TextureFormat::A4R4G4B4:
    surface = static_cast<std::uint16_t*>(description.lpSurface);
    for (row = 0; row < height; ++row) {
      for (column = 0, source = 0; column < width; ++column, source += stepX) {
        color = ((pixels[source] >> 16) & 0xF000) + ((pixels[source] >> 12) & 0x0F00) + ((pixels[source] >> 8) & 0x00F0) +
                ((pixels[source] >> 4) & 0x000F);

        if ((color & 0x0FFF) == 0x0F0F) {
          surface[column] = 0;
        } else if (this->transparent && (color & 0x0FFF) == 0) {
          surface[column] = 0;
        } else {
          surface[column] = static_cast<std::uint16_t>(color);
        }
      }
      surface = reinterpret_cast<std::uint16_t*>(reinterpret_cast<unsigned char*>(surface) + description.lPitch);
      pixels += sourceWidth * stepY;
    }
    break;
  case TextureFormat::R5G6B5:
    surface = static_cast<std::uint16_t*>(description.lpSurface);
    for (row = 0; row < height; ++row) {
      for (column = 0, source = 0; column < width; ++column, source += stepX) {
        color = pixels[source];
        color = ((color >> 5) & 0x07E0) + ((color >> 3) & 0x001F) + ((color >> 8) & 0xF800);

        if (!this->transparent) {
          surface[column] = static_cast<std::uint16_t>(color);
        } else if (color != 0xF81F) {
          surface[column] = static_cast<std::uint16_t>(color);
        }
      }
      surface = reinterpret_cast<std::uint16_t*>(reinterpret_cast<unsigned char*>(surface) + description.lPitch);
      pixels += sourceWidth * stepY;
    }
    break;
  case TextureFormat::A8R8G8B8:
    surface32 = static_cast<std::uint32_t*>(description.lpSurface);
    for (row = 0; row < height; ++row) {
      for (column = 0, source = 0; column < width; ++column, source += stepX) {
        if (this->transparent && pixels[source] == 0) {
          surface32[column] = 0;
        } else {
          surface32[column] = pixels[source];
        }
      }
      surface32 = reinterpret_cast<std::uint32_t*>(reinterpret_cast<unsigned char*>(surface32) + description.lPitch);
      pixels += sourceWidth * stepY;
    }
    break;
  }

  this->m_surface->Unlock(nullptr);

  RECT update = {0, 0, static_cast<LONG>(this->m_textureWidth), static_cast<LONG>(this->m_textureHeight)};
  this->Update(&update);
}

// OFFSET: 0x00544080
// FUNCTION: fn_01812
void CTextureDX7::Fill(const RECT* rectangle, std::uint32_t color) {
  if (this->m_surface) {
    const auto& owner = *this->m_owner;
    const auto channel = [](int value, const DeviceOwnerDX7::Channel& mask) { return (value >> mask.expansion) << mask.shift; };
    DDBLTFX effects;

    effects.dwSize = sizeof(effects);
    effects.dwFillColor =
      static_cast<std::uint16_t>(channel((color >> 16) & 255, owner.blue) + channel((color >> 8) & 255, owner.green) + channel(color & 255, owner.red));
    this->m_surface->Blt(const_cast<RECT*>(rectangle), nullptr, nullptr, DDBLT_COLORFILL | DDBLT_WAIT, &effects);
  }
}

// OFFSET: 0x005451B0
// FUNCTION: fn_01827
void CTextureDX7::Update(const RECT* rectangle) {
  RECT destination = *rectangle;
  IDirectDrawSurface7* level = this->m_surface;
  DDSCAPS2 caps{DDSCAPS_TEXTURE | DDSCAPS_MIPMAP};

  while (SUCCEEDED(level->GetAttachedSurface(&caps, &level))) {
    destination.top >>= 1;
    destination.bottom >>= 1;
    destination.left >>= 1;
    destination.right >>= 1;

    if (destination.left != destination.right && destination.top != destination.bottom) {
      if (FAILED(level->Blt(&destination, this->m_surface, const_cast<RECT*>(rectangle), DDBLT_ASYNC, nullptr))) {
        level->Blt(&destination, this->m_surface, const_cast<RECT*>(rectangle), DDBLT_WAIT, nullptr);
      }
    }

    level->Release();
  }
}

// OFFSET: 0x005452A0
// FUNCTION: fn_01828
void CTextureDX7::UploadSprite(int x, int y, int width, int height, const SpriteRleImage* image, const std::uint32_t* palette) {
  if (!this->m_surface) {
    return;
  }

  const RECT rectangle = {x, y, x + width, y + height};
  DDSURFACEDESC2 description = {sizeof(description)};

  if (FAILED(this->m_surface->Lock(const_cast<RECT*>(&rectangle), &description, DDLOCK_WAIT, nullptr))) {
    return;
  }

  if (this->m_format == TextureFormat::A1R5G5B5) {
    for (int index = 0; index < height; ++index) {
      std::memset(static_cast<unsigned char*>(description.lpSurface) + description.lPitch * index, 0, width * sizeof(std::uint16_t));
    }
  } else if (this->m_format == TextureFormat::A8R8G8B8) {
    for (int index = 0; index < height; ++index) {
      std::memset(static_cast<unsigned char*>(description.lpSurface) + description.lPitch * index, 0, width * sizeof(std::uint32_t));
    }
  }

  DecodeSprite(*image, palette, description.lpSurface, description.lPitch);
  this->m_surface->Unlock(nullptr);
  this->Update(&rectangle);
}

// OFFSET: 0x00544160
// FUNCTION: fn_01814
bool CTextureDX7::CopyTexture(CTexture* source, int sourceX, int sourceY, int sourceWidth, int sourceHeight, int x, int y, int width, int height) {
  if (!this->m_surface) {
    return false;
  }

  const RECT from = Rectangle(sourceX, sourceY, sourceWidth, sourceHeight);
  const RECT to = Rectangle(x, y, width, height);
  auto* nativeSource = static_cast<CTextureDX7*>(source)->m_surface;

  if (this->m_surface->Blt(const_cast<RECT*>(&to), nativeSource, const_cast<RECT*>(&from), DDBLT_WAIT, nullptr) != S_OK) {
    return false;
  }

  RECT update{0, 0, static_cast<LONG>(this->m_textureWidth), static_cast<LONG>(this->m_textureHeight)};
  this->Update(&update);
  return true;
}
