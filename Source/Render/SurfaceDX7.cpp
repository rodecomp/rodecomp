#include "Render/DeviceOwnerDX7.h"
#include "Render/SurfaceDX7.h"

#include <algorithm>
#include <bit>

namespace {
  std::uint32_t AlignDimension(int value) {
    const std::uint32_t dimension = value;
    return dimension + ((0u - dimension) & 3);
  }

  DDSURFACEDESC2 Description(DeviceOwnerDX7& owner, DWORD width, DWORD height) {
    DDSURFACEDESC2 description{};
    description.dwSize = sizeof(description);
    description.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CKSRCBLT;
    description.dwWidth = width;
    description.dwHeight = height;
    description.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    description.ddpfPixelFormat = owner.primaryDescription.ddpfPixelFormat;
    description.ddckCKSrcBlt.dwColorSpaceLowValue =
      description.ddpfPixelFormat.dwRBitMask | description.ddpfPixelFormat.dwBBitMask;
    description.ddckCKSrcBlt.dwColorSpaceHighValue = description.ddckCKSrcBlt.dwColorSpaceLowValue;
    return description;
  }

  std::uint16_t Channel(unsigned value, const DeviceOwnerDX7::Channel& channel) {
    return static_cast<std::uint16_t>((value >> (channel.expansion & 31)) << (channel.shift & 31));
  }

  std::uint16_t Color(DeviceOwnerDX7& owner, unsigned red, unsigned green, unsigned blue) {
    return static_cast<std::uint16_t>(
      Channel(red, owner.red) + Channel(green, owner.green) + Channel(blue, owner.blue)
    );
  }
}

// OFFSET: 0x00542B50
// FUNCTION: fn_01776
CSurfaceDX7::CSurfaceDX7(DeviceOwnerDX7& owner, int width, int height)
    : m_owner(&owner), m_surface(nullptr), m_width(AlignDimension(width)), m_height(AlignDimension(height)) {
  auto description = Description(owner, this->m_width, this->m_height);
  owner.draw->CreateSurface(&description, &this->m_surface, nullptr);
}

// OFFSET: 0x00542C80
// FUNCTION: fn_01777
CSurfaceDX7::CSurfaceDX7(DeviceOwnerDX7& owner, IDirectDrawSurface7* surface)
    : m_owner(&owner), m_surface(nullptr), m_width(0), m_height(0) {
  if (surface) {
    DDSURFACEDESC2 description{};
    description.dwSize = sizeof(description);

    if (SUCCEEDED(surface->GetSurfaceDesc(&description))) {
      this->m_width = description.dwWidth;
      this->m_height = description.dwHeight;
      this->m_surface = surface;
      surface->AddRef();
    }
  }
}

// OFFSET: 0x00542DC0
// FUNCTION: fn_01779
// OFFSET: 0x00542E60
// FUNCTION: fn_01781
CSurfaceDX7::~CSurfaceDX7() {
  if (this->m_surface) {
    this->m_surface->Release();
    this->m_surface = nullptr;
  }
}

// OFFSET: 0x00543360
// FUNCTION: fn_01801
std::uint32_t CSurfaceDX7::Width() {
  return this->m_width;
}

// OFFSET: 0x00543310
// FUNCTION: fn_01796
std::uint32_t CSurfaceDX7::Height() {
  return this->m_height;
}

// OFFSET: 0x00543000
// FUNCTION: fn_01784
bool CSurfaceDX7::Resize(int width, int height) {
  this->m_width = AlignDimension(width);
  this->m_height = AlignDimension(height);

  if (this->m_surface) {
    this->m_surface->Release();
    this->m_surface = nullptr;
  }

  auto description = Description(*this->m_owner, this->m_width, this->m_height);
  return SUCCEEDED(this->m_owner->draw->CreateSurface(&description, &this->m_surface, nullptr));
}

// OFFSET: 0x00543370
// FUNCTION: fn_01802
SurfaceLock CSurfaceDX7::Lock() {
  SurfaceLock output{};

  if (this->m_surface) {
    // The original supplies a zeroed descriptor, including dwSize, at this site.
    DDSURFACEDESC2 description{};

    if (SUCCEEDED(this->m_surface->Lock(nullptr, &description, DDLOCK_WAIT, nullptr))) {
      output.success = true;
      output.pixels = description.lpSurface;
      output.pitch = description.lPitch;
      output.width = description.dwWidth;
      output.height = description.dwHeight;
      output.bitsPerPixel = description.ddpfPixelFormat.dwRGBBitCount;
    }
  }

  return output;
}

// OFFSET: 0x00543710
// FUNCTION: fn_01804
void CSurfaceDX7::Unlock() {
  if (this->m_surface) {
    this->m_surface->Unlock(nullptr);
  }
}

// OFFSET: 0x005432A0
// FUNCTION: fn_01791
bool CSurfaceDX7::GetDC(HDC* output) {
  return this->m_surface && SUCCEEDED(this->m_surface->GetDC(output));
}

// OFFSET: 0x00543400
// FUNCTION: fn_01803
void CSurfaceDX7::ReleaseDC(HDC dc) {
  if (this->m_surface) {
    this->m_surface->ReleaseDC(dc);
  }
}

// OFFSET: 0x00543730
// FUNCTION: fn_01805
void CSurfaceDX7::Upload(int x, int y, int width, int height, const std::uint32_t* pixels, int, int sourceWidth) {
  if (!this->m_surface || !width || !height) {
    return;
  }

  const unsigned stride = sourceWidth ? sourceWidth : width;
  RECT rectangle{
    x, y, std::bit_cast<int>(static_cast<unsigned>(x) + width), std::bit_cast<int>(static_cast<unsigned>(y) + height)
  };
  DDSURFACEDESC2 description{};
  description.dwSize = sizeof(description);

  if (FAILED(this->m_surface->Lock(&rectangle, &description, DDLOCK_WAIT | DDLOCK_WRITEONLY, nullptr))) {
    return;
  }

  const auto transparent = Color(*this->m_owner, 255, 0, 255);
  auto* output = static_cast<unsigned char*>(description.lpSurface);

  for (int row = 0; row < height; ++row) {
    if (description.ddpfPixelFormat.dwRGBBitCount == 16) {
      auto* destination = reinterpret_cast<std::uint16_t*>(output);

      for (int column = 0; column < width; ++column) {
        const auto pixel = pixels[column];
        const auto value = Color(*this->m_owner, (pixel >> 16) & 255, (pixel >> 8) & 255, pixel & 255);

        if (value != transparent) {
          destination[column] = value;
        }
      }
    } else {
      auto* destination = reinterpret_cast<std::uint32_t*>(output);

      for (int column = 0; column < width; ++column) {
        if ((pixels[column] & 0xFFFFFF) != 0xFF00FF) {
          destination[column] = pixels[column];
        }
      }
    }

    output += description.lPitch;
    pixels += stride;
  }

  this->m_surface->Unlock(nullptr);
}

// OFFSET: 0x00542F70
// FUNCTION: fn_01783
void CSurfaceDX7::Fill(const RECT* rectangle, std::uint32_t color) {
  if (this->m_surface) {
    DDBLTFX effects;
    effects.dwSize = sizeof(effects);
    // Unlike Upload, this entry feeds low/high color bytes into red/blue and
    // truncates the packed result to 16 bits even on a 32-bit surface.
    effects.dwFillColor = Color(*this->m_owner, color & 255, (color >> 8) & 255, (color >> 16) & 255);
    this->m_surface->Blt(const_cast<RECT*>(rectangle), nullptr, nullptr, DDBLT_COLORFILL | DDBLT_WAIT, &effects);
  }
}

// OFFSET: 0x005430F0
// FUNCTION: fn_01785
void CSurfaceDX7::Draw(int x, int y, int width, int height, std::uint32_t) {
  RECT source{0, 0, width, height};
  const auto right = std::bit_cast<int>(static_cast<unsigned>(x) + width);
  const auto bottom = std::bit_cast<int>(static_cast<unsigned>(y) + height);

  if (right > std::bit_cast<int>(this->m_owner->width)) {
    source.right = std::bit_cast<int>(this->m_owner->width - static_cast<unsigned>(x));
  }

  if (bottom > std::bit_cast<int>(this->m_owner->height)) {
    source.bottom = std::bit_cast<int>(this->m_owner->height - static_cast<unsigned>(y));
  }

  source.left = x < 0 ? std::bit_cast<int>(0u - static_cast<unsigned>(x)) : 0;
  source.top = y < 0 ? std::bit_cast<int>(0u - static_cast<unsigned>(y)) : 0;

  if (source.right > source.left && source.bottom > source.top) {
    this->m_owner->renderTarget->BltFast(
      std::max(x, 0), std::max(y, 0), this->m_surface, &source, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT
    );
  }
}

// OFFSET: 0x005431C0
// FUNCTION: fn_01786
void CSurfaceDX7::Stretch(int, int, int width, int height) {
  RECT destination{0, 0, width, height};
  this->m_owner->renderTarget->Blt(&destination, this->m_surface, nullptr, DDBLT_WAIT, nullptr);
}

// OFFSET: 0x00542D60
// FUNCTION: fn_01778
CSurfaceWallpaperDX7::CSurfaceWallpaperDX7(DeviceOwnerDX7& owner, int width, int height)
    : CSurfaceDX7(owner, width, height) {
}

// OFFSET: 0x00543990
// FUNCTION: fn_01806
void CSurfaceWallpaperDX7::Upload(int, int, int width, int height, const std::uint32_t* pixels, int, int) {
  CSurfaceDX7 temporary(*this->m_owner, width, height);
  temporary.Upload(0, 0, width, height, pixels, 0, 0);
  this->m_surface->Blt(nullptr, temporary.m_surface, nullptr, DDBLT_WAIT, nullptr);
}
