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

  __declspec(noinline) void DecodeSprite(const SpriteRleImage& image, const std::uint32_t* palette, void* pixels, int pitch);

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
    ShowErrorDialog("GFC ", 0);
  }

  DDSURFACEDESC2 description = {sizeof(description), 0};
  description.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
  description.dwWidth = this->m_textureWidth;
  description.dwFlags = (selected == TextureFormat::R5G6B5 ? DDSD_CKSRCBLT : 0) | g_textureAllocationDX7.descriptorFlags | DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH |
                        DDSD_PIXELFORMAT | DDSD_TEXTURESTAGE;
  description.dwHeight = this->m_textureHeight;
  description.dwTextureStage = 0;
  description.ddsCaps.dwCaps = g_textureAllocationDX7.surfaceCaps | DDSCAPS_TEXTURE;
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
  std::uint8_t& transparent = this->transparent;
  transparent = static_cast<std::uint8_t>(option);

  if (!this->m_surface || !pixels) {
    return;
  }

  const unsigned stride = sourceWidth ? sourceWidth : width;
  DDSURFACEDESC2 description{};
  description.dwSize = sizeof(description);

  if (FAILED(this->m_surface->GetSurfaceDesc(&description))) {
    return;
  }

  int stepX = 1;
  int stepY = 1;

  while (static_cast<unsigned>(width / stepX) > description.dwWidth) {
    ++stepX;
  }

  while (static_cast<unsigned>(height / stepY) > description.dwHeight) {
    ++stepY;
  }

  const int left = x / stepX;
  const int top = y / stepY;
  const int columns = width / stepX;
  const int rows = height / stepY;

  if (!columns || !rows) {
    return;
  }

  const RECT rectangle = Rectangle(left, top, columns, rows);

  if (FAILED(this->m_surface->Lock(const_cast<RECT*>(&rectangle), &description, DDLOCK_WAIT | DDLOCK_WRITEONLY, nullptr))) {
    return;
  }

  auto* output = static_cast<unsigned char*>(description.lpSurface);

  switch (this->m_format) {
  case TextureFormat::A1R5G5B5:
    for (int row = 0; row < rows; ++row) {
      for (int column = 0; column < columns; ++column) {
        const std::uint32_t color = pixels[column * stepX];
        const unsigned packed = ((color >> 9) & 0x7C00) + ((color >> 6) & 0x3E0) + ((color >> 3) & 0x1F) + ((color >> 16) & 0x8000);
        std::uint16_t value;

        if (packed == 0xFC1F) {
          value = 0;
        } else if (transparent && !(packed & 0x7FFF)) {
          value = 0;
        } else {
          value = static_cast<std::uint16_t>(packed);
        }

        std::memcpy(output + column * sizeof(value), &value, sizeof(value));
      }

      output += description.lPitch;
      pixels += static_cast<unsigned>(stepY) * stride;
    }
    break;
  case TextureFormat::A4R4G4B4:
    for (int row = 0; row < rows; ++row) {
      for (int column = 0; column < columns; ++column) {
        const std::uint32_t color = pixels[column * stepX];
        const unsigned packed = ((color >> 16) & 0xF000) + ((color >> 12) & 0xF00) + ((color >> 8) & 0xF0) + ((color >> 4) & 0xF);
        std::uint16_t value;

        if ((packed & 0xFFF) == 0xF0F) {
          value = 0;
        } else if (transparent && !(packed & 0xFFF)) {
          value = 0;
        } else {
          value = static_cast<std::uint16_t>(packed);
        }

        std::memcpy(output + column * sizeof(value), &value, sizeof(value));
      }

      output += description.lPitch;
      pixels += static_cast<unsigned>(stepY) * stride;
    }
    break;
  case TextureFormat::R5G6B5:
    for (int row = 0; row < rows; ++row) {
      for (int column = 0; column < columns; ++column) {
        const std::uint32_t color = pixels[column * stepX];
        const unsigned packed = ((color >> 5) & 0x7E0) + ((color >> 3) & 0x1F) + ((color >> 8) & 0xF800);

        if (!transparent || packed != 0xF81F) {
          const std::uint16_t value = static_cast<std::uint16_t>(packed);
          std::memcpy(output + column * sizeof(value), &value, sizeof(value));
        }
      }

      output += description.lPitch;
      pixels += static_cast<unsigned>(stepY) * stride;
    }
    break;
  case TextureFormat::A8R8G8B8:
    for (int row = 0; row < rows; ++row) {
      for (int column = 0; column < columns; ++column) {
        const std::uint32_t color = transparent && !pixels[column * stepX] ? 0 : pixels[column * stepX];
        std::memcpy(output + column * sizeof(color), &color, sizeof(color));
      }

      output += description.lPitch;
      pixels += static_cast<unsigned>(stepY) * stride;
    }
    break;
  }

  this->m_surface->Unlock(nullptr);
  RECT update{0, 0, static_cast<LONG>(this->m_textureWidth), static_cast<LONG>(this->m_textureHeight)};
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

void CTextureDX7::Draw(int x, int y, int width, int height, std::uint32_t color) {
  this->DrawTo(
    g_renderer->primitives, g_renderer->queues, x, y, width, height, color, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
  );
}

void CTextureDX7::Stretch(int x, int y, int width, int height) {
  this->StretchTo(
    g_renderer->primitives, g_renderer->queues, x, y, width, height, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
  );
}

// OFFSET: 0x005451B0
// FUNCTION: fn_01827
void CTextureDX7::Update(const RECT* rectangle) {
  RECT destination = *rectangle;
  DDSCAPS2 caps{DDSCAPS_TEXTURE | DDSCAPS_MIPMAP};
  IDirectDrawSurface7* level = this->m_surface;

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

  const RECT rectangle = Rectangle(x, y, width, height);
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
