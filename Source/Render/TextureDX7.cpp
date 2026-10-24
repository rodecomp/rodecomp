#include "Client/ErrorDialog.h"
#include "Render/DeviceOwnerDX7.h"
#include "Render/SpritePixels.h"
#include "Render/TextureDX7.h"

#include <array>
#include <bit>
#include <cstring>

namespace {
  // STATIC: 0x01387270; shared across devices, initially all bytes 0xFF.
  std::array<DDPIXELFORMAT, 5> formats = [] {
    std::array<DDPIXELFORMAT, 5> result;
    std::memset(result.data(), 0xFF, sizeof(result));
    return result;
  }();

  // OFFSET: 0x00544B50
  // FUNCTION: fn_01824
  HRESULT CALLBACK SelectFormat(DDPIXELFORMAT* candidate, void* context) {
    auto* format = static_cast<unsigned*>(context);

    if (!candidate || !format) {
      return D3DENUMRET_OK;
    }

    bool accepted = false;

    switch (*format) {
    case 0:
      accepted = (candidate->dwFlags & DDPF_ALPHAPIXELS) && candidate->dwRGBBitCount == 16 &&
                 candidate->dwRGBAlphaBitMask == 0x8000;
      break;
    case 1:
      accepted = (candidate->dwFlags & DDPF_ALPHAPIXELS) && candidate->dwRGBBitCount == 16 &&
                 candidate->dwRGBAlphaBitMask == 0xF000;
      break;
    case 2:
      accepted =
        !(candidate->dwFlags & DDPF_ALPHAPIXELS) && candidate->dwRGBBitCount == 16 && candidate->dwRBitMask == 0xF800;
      break;
    case 4:
      accepted = (candidate->dwFlags & DDPF_ALPHAPIXELS) && candidate->dwRGBBitCount == 32 &&
                 candidate->dwRGBAlphaBitMask == 0xFF000000;
      break;
    }

    if (!accepted) {
      return D3DENUMRET_OK;
    }

    formats[*format] = *candidate;
    return D3DENUMRET_CANCEL;
  }

  RECT Rectangle(int x, int y, int width, int height) {
    return {
      x, y, std::bit_cast<int>(static_cast<unsigned>(x) + width), std::bit_cast<int>(static_cast<unsigned>(y) + height)
    };
  }

  // OFFSET: 0x0056FCE0
  // FUNCTION: fn_02410
  void DecodeSprite(const SpriteRleImage& image, const std::uint32_t* palette, void* pixels, int pitch) {
    const auto* input = image.runs;
    auto* output = static_cast<unsigned char*>(pixels);
    int remaining = 0;

    for (int y = 0; y < image.height; ++y) {
      auto* row = reinterpret_cast<std::uint16_t*>(output);

      for (int x = 0; x < image.width; ++x) {
        if (remaining) {
          --remaining;
          row[x] = 0;
        } else {
          if (*input) {
            row[x] = static_cast<std::uint16_t>(palette[*input]);
          } else {
            ++input;
            row[x] = 0;
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
CTextureDX7::CTextureDX7(TextureDX7Context& context)
    : m_owner(&context.owner), m_surface(nullptr), m_context(&context) {
  this->m_width = this->m_height = this->m_textureWidth = this->m_textureHeight = 1;
  this->m_format = 0;
}

// OFFSET: 0x00543B90
// FUNCTION: fn_01808
CTextureDX7::CTextureDX7(TextureDX7Context& context, int width, int height, int format)
    : m_owner(&context.owner), m_surface(nullptr), m_context(&context) {
  this->Create(width, height, format);
  this->SetImageSize(width, height);
}

// OFFSET: 0x00543DC0
// FUNCTION: fn_01809
// OFFSET: 0x00543E30
// FUNCTION: fn_01810
CTextureDX7::~CTextureDX7() {
  if (this->m_surface) {
    this->m_surface->Release();
    this->m_surface = nullptr;
  }
}

// OFFSET: 0x00544230
// FUNCTION: fn_01815
bool CTextureDX7::Create(int width, int height, int format) {
  this->m_textureWidth = width;
  this->m_textureHeight = height;
  this->m_format = format;
  this->m_owner->AdjustTextureDimensions(this->m_textureWidth, this->m_textureHeight);

  if (this->m_surface) {
    this->m_surface->Release();
    this->m_surface = nullptr;
  }

  if (static_cast<unsigned>(format) >= 2 && format != 2 && format != 4) {
    ShowErrorDialog(this->m_context->renderSystem, "Create Texture::Unsupported pixelformat", 0);
  }

  unsigned selected = format;

  if (formats[selected].dwSize != 32) {
    this->m_owner->device->EnumTextureFormats(SelectFormat, &selected);
  }

  if (formats[selected].dwSize != 32) {
    ShowErrorDialog(this->m_context->renderSystem, "GFC ", 0);
  }

  DDSURFACEDESC2 description{};
  description.dwSize = sizeof(description);
  description.dwFlags = this->m_context->allocation.descriptorFlags | 0x101007;

  if (selected == 2) {
    description.dwFlags |= DDSD_CKSRCBLT;
  }

  description.dwWidth = this->m_textureWidth;
  description.dwHeight = this->m_textureHeight;
  description.ddpfPixelFormat = formats[selected];
  description.ddsCaps.dwCaps = this->m_context->allocation.surfaceCaps | DDSCAPS_TEXTURE;
  description.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
  description.ddckCKSrcBlt.dwColorSpaceLowValue =
    description.ddpfPixelFormat.dwRBitMask | description.ddpfPixelFormat.dwBBitMask;
  description.ddckCKSrcBlt.dwColorSpaceHighValue = description.ddckCKSrcBlt.dwColorSpaceLowValue;
  unsigned count = 1;
  description.dwMipMapCount = 1;

  while (this->m_textureWidth > (1u << (count & 31))) {
    if ((1u << (count & 31)) >= this->m_textureHeight) {
      break;
    }

    description.dwMipMapCount = ++count;
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

  if (this->m_surface) {
    DDSURFACEDESC2 description{};

    if (SUCCEEDED(this->m_surface->Lock(nullptr, &description, DDLOCK_WAIT, nullptr))) {
      output = {
        true,
        description.lpSurface,
        description.lPitch,
        description.dwWidth,
        description.dwHeight,
        static_cast<int>(description.ddpfPixelFormat.dwRGBBitCount)
      };
    }
  }

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
  return this->m_surface && SUCCEEDED(this->m_surface->GetDC(output));
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
void CTextureDX7::Upload(
  int x,
  int y,
  int width,
  int height,
  const std::uint32_t* pixels,
  int option,
  int sourceWidth
) {
  this->SetImageSize(width, height);
  this->unknown_0008 = (this->unknown_0008 & 255) | (static_cast<unsigned char>(option) << 8);

  if (!this->m_surface || !pixels) {
    return;
  }

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

  const int columns = width / stepX;
  const int rows = height / stepY;

  if (!columns || !rows) {
    return;
  }

  const RECT rectangle = Rectangle(x / stepX, y / stepY, columns, rows);

  if (
    FAILED(this->m_surface->Lock(const_cast<RECT*>(&rectangle), &description, DDLOCK_WAIT | DDLOCK_WRITEONLY, nullptr))
  ) {
    return;
  }

  const unsigned stride = sourceWidth ? sourceWidth : width;
  auto* output = static_cast<unsigned char*>(description.lpSurface);
  const bool transparent = static_cast<unsigned char>(option) != 0;

  for (int row = 0; row < rows; ++row) {
    for (int column = 0; column < columns; ++column) {
      const std::uint32_t color = pixels[column * stepX];
      auto* words = reinterpret_cast<std::uint16_t*>(output);

      switch (this->m_format) {
      case 0: {
        std::uint16_t value =
          ((color >> 16) & 0x8000) | ((color >> 3) & 0x1F) | ((color >> 6) & 0x3E0) | ((color >> 9) & 0x7C00);

        if (value == 0xFC1F || (transparent && !(value & 0x7FFF))) {
          value = 0;
        }

        words[column] = value;
        break;
      }
      case 1: {
        std::uint16_t value =
          ((color & 255) >> 4) | ((color >> 8) & 0xF0) | ((color >> 12) & 0xF00) | ((color >> 16) & 0xF000);

        if ((value & 0xFFF) == 0xF0F || (transparent && !(value & 0xFFF))) {
          value = 0;
        }

        words[column] = value;
        break;
      }
      case 2: {
        const std::uint16_t value = ((color >> 8) & 0xF800) | ((color >> 3) & 0x1F) | ((color >> 5) & 0x7E0);

        if (!transparent || value != 0xF81F) {
          words[column] = value;
        }

        break;
      }
      case 4:
        reinterpret_cast<std::uint32_t*>(output)[column] = color;
        break;
      }
    }

    output += description.lPitch;
    pixels += static_cast<unsigned>(stepY) * stride;
  }

  this->m_surface->Unlock(nullptr);
  RECT update{0, 0, static_cast<LONG>(this->m_textureWidth), static_cast<LONG>(this->m_textureHeight)};
  this->Update(static_cast<int>(reinterpret_cast<std::intptr_t>(&update)));
}

// OFFSET: 0x00544080
// FUNCTION: fn_01812
void CTextureDX7::Fill(const RECT* rectangle, std::uint32_t color) {
  if (this->m_surface) {
    const auto& owner = *this->m_owner;
    const auto channel = [](unsigned value, const DeviceOwnerDX7::Channel& mask) {
      return (value >> (mask.expansion & 31)) << (mask.shift & 31);
    };
    DDBLTFX effects;
    effects.dwSize = sizeof(effects);
    effects.dwFillColor = static_cast<std::uint16_t>(
      channel(color & 255, owner.red) + channel((color >> 8) & 255, owner.green) +
      channel((color >> 16) & 255, owner.blue)
    );
    this->m_surface->Blt(const_cast<RECT*>(rectangle), nullptr, nullptr, DDBLT_COLORFILL | DDBLT_WAIT, &effects);
  }
}

// OFFSET: 0x00544410
// FUNCTION: fn_01817
void CTextureDX7::Draw(int x, int y, int width, int height, std::uint32_t color) {
  const auto& context = *this->m_context;
  this->DrawTo(
    context.primitives, context.queues, x, y, width, height, color, context.depthAlpha, context.depthStart,
    context.depthEnd
  );
}

// OFFSET: 0x00544610
// FUNCTION: fn_01818
void CTextureDX7::Stretch(int x, int y, int width, int height) {
  const auto& context = *this->m_context;
  this->StretchTo(
    context.primitives, context.queues, x, y, width, height, context.depthAlpha, context.depthStart, context.depthEnd
  );
}

// OFFSET: 0x005451B0
// FUNCTION: fn_01827
void CTextureDX7::Update(int argument) {
  const auto* rectangle = reinterpret_cast<const RECT*>(static_cast<std::uintptr_t>(static_cast<unsigned>(argument)));
  RECT destination = *rectangle;
  DDSCAPS2 caps{DDSCAPS_TEXTURE | DDSCAPS_MIPMAP};
  IDirectDrawSurface7* level = this->m_surface;

  while (SUCCEEDED(level->GetAttachedSurface(&caps, &level))) {
    destination.left >>= 1;
    destination.top >>= 1;
    destination.right >>= 1;
    destination.bottom >>= 1;

    if (destination.left != destination.right && destination.top != destination.bottom) {
      if (FAILED(level->Blt(&destination, this->m_surface, const_cast<RECT*>(rectangle), DDBLT_DDFX, nullptr))) {
        level->Blt(&destination, this->m_surface, const_cast<RECT*>(rectangle), DDBLT_WAIT, nullptr);
      }
    }

    // The owning mip chain retains this level during the following traversal.
    level->Release();
  }
}

// OFFSET: 0x005452A0
// FUNCTION: fn_01828
void CTextureDX7::UploadSprite(int x, int y, int width, int height, const void* rawImage, int palette) {
  if (!this->m_surface) {
    return;
  }

  const RECT rectangle = Rectangle(x, y, width, height);
  DDSURFACEDESC2 description{};
  description.dwSize = sizeof(description);

  if (FAILED(this->m_surface->Lock(const_cast<RECT*>(&rectangle), &description, DDLOCK_WAIT, nullptr))) {
    return;
  }

  if (this->m_format == 0 || this->m_format == 4) {
    const unsigned bytes = this->m_format == 0 ? 2 : 4;
    auto* row = static_cast<unsigned char*>(description.lpSurface);

    for (int index = 0; index < height; ++index) {
      std::memset(row, 0, bytes * static_cast<unsigned>(width));
      row += description.lPitch;
    }
  }

  const auto* colors =
    reinterpret_cast<const std::uint32_t*>(static_cast<std::uintptr_t>(static_cast<unsigned>(palette)));
  DecodeSprite(*static_cast<const SpriteRleImage*>(rawImage), colors, description.lpSurface, description.lPitch);
  this->m_surface->Unlock(nullptr);
  this->Update(static_cast<int>(reinterpret_cast<std::intptr_t>(&rectangle)));
}

// OFFSET: 0x00544160
// FUNCTION: fn_01814
bool CTextureDX7::CopyTexture(
  CTexture* source,
  int sourceX,
  int sourceY,
  int sourceWidth,
  int sourceHeight,
  int x,
  int y,
  int width,
  int height
) {
  if (!this->m_surface) {
    return false;
  }

  const RECT from = Rectangle(sourceX, sourceY, sourceWidth, sourceHeight);
  const RECT to = Rectangle(x, y, width, height);
  auto* nativeSource = static_cast<CTextureDX7*>(source)->m_surface;

  if (
    this->m_surface->Blt(const_cast<RECT*>(&to), nativeSource, const_cast<RECT*>(&from), DDBLT_WAIT, nullptr) != S_OK
  ) {
    return false;
  }

  RECT update{0, 0, static_cast<LONG>(this->m_textureWidth), static_cast<LONG>(this->m_textureHeight)};
  this->Update(static_cast<int>(reinterpret_cast<std::intptr_t>(&update)));
  return true;
}
