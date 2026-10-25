#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/RendererBackBuffer.h"
#include "Render/DX7/RendererDX7Operations.h"
#include "Resource/Bitmap/BitmapImage.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/Bitmap/IJL15Runtime.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {
  // INLINED
  std::uint16_t PackedColor(const DeviceOwnerDX7& owner, unsigned color) {
    const auto channel = [](unsigned value, const DeviceOwnerDX7::Channel& field) {
      return static_cast<std::uint16_t>((value >> (field.expansion & 31)) << (field.shift & 31));
    };
    return static_cast<std::uint16_t>(channel((color >> 16) & 255, owner.red) + channel((color >> 8) & 255, owner.green) + channel(color & 255, owner.blue));
  }

  // INLINED
  unsigned char ExpandedChannel(unsigned value, DWORD mask, const DeviceOwnerDX7::Channel& field) {
    const auto component = static_cast<unsigned char>((value & mask) >> (field.shift & 31));
    return static_cast<unsigned char>(static_cast<unsigned>(component) << (field.expansion & 31));
  }
}

// OFFSET: 0x0055D860
// FUNCTION: fn_02109
bool RendererDX7Operations::SaveBitmap(const char* filename, const BitmapImage& overlay, int width, int height, RendererBackBuffer& backBuffer) {
  if (!backBuffer.Lock(this->owner)) {
    return false;
  }

  std::FILE* file = std::fopen(filename, "wb");

  if (!file) {
    return false;
  }

  BITMAPFILEHEADER header{};
  header.bfType = 0x4D42;
  header.bfSize = 3u * (static_cast<unsigned>(width) * static_cast<unsigned>(height) + 18u);
  header.bfOffBits = 54;
  std::fwrite(&header, sizeof(header), 1, file);
  BITMAPINFOHEADER information{};
  information.biSize = sizeof(information);
  information.biWidth = width;
  information.biHeight = height;
  information.biPlanes = 1;
  information.biBitCount = 24;
  std::fwrite(&information, sizeof(information), 1, file);

  const auto red = this->owner.red;
  const auto green = this->owner.green;
  const auto blue = this->owner.blue;
  const auto redMask = this->owner.redMask;
  const auto greenMask = this->owner.greenMask;
  const auto blueMask = this->owner.blueMask;
  const auto transparent = PackedColor(this->owner, 0xFF00FF);
  std::array<unsigned char, 10800> row;

  for (int y = height - 1; y >= 0; --y) {
    const auto* source = static_cast<const unsigned char*>(backBuffer.pixels) + y * backBuffer.pitch;

    for (int x = 0; x < width; ++x) {
      unsigned color = 0;
      const int overlayX = x + overlay.width - width;
      const int overlayY = y + overlay.height - height;
      bool useOverlay = overlayX >= 0 && overlayX < overlay.width && overlayY >= 0 && overlayY < overlay.height;

      if (useOverlay) {
        color = overlay.pixels[overlayX + overlayY * overlay.width];
        useOverlay = backBuffer.bitsPerPixel == 16 ? PackedColor(this->owner, color) != transparent : (color & 0xFFFFFF) != 0xFF00FF;
      }

      if (!useOverlay) {
        if (backBuffer.bitsPerPixel == 16) {
          std::uint16_t pixel;
          std::memcpy(&pixel, source + 2 * x, sizeof(pixel));
          color = (static_cast<unsigned>(ExpandedChannel(pixel, redMask, red)) << 16) | (static_cast<unsigned>(ExpandedChannel(pixel, greenMask, green)) << 8) |
                  ExpandedChannel(pixel, blueMask, blue);
        } else {
          std::memcpy(&color, source + 4 * x, sizeof(color));
        }
      }

      row[3 * x] = static_cast<unsigned char>(color);
      row[3 * x + 1] = static_cast<unsigned char>(color >> 8);
      row[3 * x + 2] = static_cast<unsigned char>(color >> 16);
    }

    std::fwrite(row.data(), 3u * static_cast<unsigned>(width), 1, file);
  }

  backBuffer.Unlock(this->owner);
  std::fclose(file);
  return true;
}

// OFFSET: 0x0055DDF0
// FUNCTION: fn_02110
bool RendererDX7Operations::SaveJpeg(
  const char* filename, const BitmapImage& overlay, int width, int height, RendererBackBuffer& backBuffer, IJL15Runtime& runtime
) {
  if (!backBuffer.Lock(this->owner)) {
    return false;
  }

  const unsigned size = 3u * static_cast<unsigned>(width) * static_cast<unsigned>(height);
  auto* pixels = static_cast<unsigned char*>(std::malloc(size));
  const auto red = this->owner.red;
  const auto green = this->owner.green;
  const auto blue = this->owner.blue;
  const auto redMask = this->owner.redMask;
  const auto greenMask = this->owner.greenMask;
  const auto blueMask = this->owner.blueMask;
  const auto transparent = PackedColor(this->owner, 0xFF00FF);

  for (int y = height - 1; y >= 0; --y) {
    const auto* source = static_cast<const unsigned char*>(backBuffer.pixels) + y * backBuffer.pitch;
    auto* row = pixels + 3u * static_cast<unsigned>(y) * static_cast<unsigned>(width);

    for (int x = 0; x < width; ++x) {
      unsigned color = 0;
      const int overlayX = x + overlay.width - width;
      const int overlayY = y + overlay.height - height;
      bool useOverlay = overlayX >= 0 && overlayX < overlay.width && overlayY >= 0 && overlayY < overlay.height;

      if (useOverlay) {
        color = overlay.pixels[overlayX + overlayY * overlay.width];
        useOverlay = backBuffer.bitsPerPixel == 16 ? PackedColor(this->owner, color) != transparent : (color & 0xFFFFFF) != 0xFF00FF;
      }

      if (!useOverlay) {
        if (backBuffer.bitsPerPixel == 16) {
          std::uint16_t pixel;
          std::memcpy(&pixel, source + 2 * x, sizeof(pixel));
          color = (static_cast<unsigned>(ExpandedChannel(pixel, redMask, red)) << 16) | (static_cast<unsigned>(ExpandedChannel(pixel, greenMask, green)) << 8) |
                  ExpandedChannel(pixel, blueMask, blue);
        } else {
          std::memcpy(&color, source + 4 * x, sizeof(color));
        }
      }

      row[3 * x] = static_cast<unsigned char>(color);
      row[3 * x + 1] = static_cast<unsigned char>(color >> 8);
      row[3 * x + 2] = static_cast<unsigned char>(color >> 16);
    }
  }

  backBuffer.Unlock(this->owner);
  IJL15Properties properties{};

  if (runtime.initialize(&properties) != 0) {
    return false;
  }

  properties.dibWidth = width;
  properties.dibHeight = height;
  properties.dibBytes = pixels;
  properties.dibPadding = ((3u * static_cast<unsigned>(width) + 3u) & ~3u) - 3u * static_cast<unsigned>(width);
  properties.jpegFile = filename;
  properties.dibChannels = 3;
  properties.dibColor = 2;
  properties.jpegWidth = width;
  properties.jpegHeight = height;
  const int status = runtime.write(&properties, 8);
  std::free(pixels);

  return status == 0;
}

// OFFSET: 0x0055E340
// FUNCTION: fn_02111
bool RendererDX7Operations::SaveLogo(std::string filename, ResourceMgr& resources, TextureNames& names, IJL15Runtime& runtime) {
  auto* logo = static_cast<CBitmapRes*>(resources.Load(names.Resolve("scr_logo.bmp")));

  if (!logo) {
    return false;
  }

  DDSURFACEDESC2 locked{};
  locked.dwSize = sizeof(locked);

  if (FAILED(this->owner.renderTarget->Lock(nullptr, &locked, DDLOCK_WAIT, nullptr))) {
    return false;
  }

  const int pixelBytes = locked.lPitch / static_cast<int>(locked.dwWidth);
  const int paddedWidth = locked.lPitch / pixelBytes;
  unsigned remaining = 3u * static_cast<unsigned>(paddedWidth) * locked.dwHeight;
  auto* pixels = static_cast<unsigned char*>(std::malloc(remaining));
  auto* destination = pixels;
  const auto& image = logo->image;

  for (unsigned y = 0; y < locked.dwHeight; ++y) {
    const auto* source = static_cast<const unsigned char*>(locked.lpSurface) + y * locked.lPitch;

    for (unsigned x = 0; x < locked.dwWidth && static_cast<int>(remaining) > 0; ++x) {
      const unsigned overlayX = x + static_cast<unsigned>(image.width) - locked.dwWidth;
      const unsigned overlayY = y + static_cast<unsigned>(image.height) - locked.dwHeight;
      const bool inOverlay = x >= locked.dwWidth - static_cast<unsigned>(image.width) && y >= locked.dwHeight - static_cast<unsigned>(image.height);
      unsigned color = inOverlay ? image.pixels[overlayX + overlayY * image.width] : 0xFF00FF;

      if (!inOverlay || (color & 0xFFFFFF) == 0xFF00FF) {
        if (locked.ddpfPixelFormat.dwRGBBitCount == 16) {
          std::uint16_t value;
          std::memcpy(&value, source + x * pixelBytes, 2);
          color = (static_cast<unsigned>(ExpandedChannel(value, this->owner.redMask, this->owner.red)) << 16) |
                  (static_cast<unsigned>(ExpandedChannel(value, this->owner.greenMask, this->owner.green)) << 8) |
                  ExpandedChannel(value, this->owner.blueMask, this->owner.blue);
        } else {
          std::memcpy(&color, source + x * pixelBytes, 4);
        }
      }

      destination[0] = static_cast<unsigned char>(color);
      destination[1] = static_cast<unsigned char>(color >> 8);
      destination[2] = static_cast<unsigned char>(color >> 16);
      destination += 3;
      remaining -= 3;
    }

    destination = pixels + 3u * static_cast<unsigned>(paddedWidth) * (y + 1);
    remaining = 3u * static_cast<unsigned>(paddedWidth) * (locked.dwHeight - y - 1);
  }

  this->owner.renderTarget->Unlock(nullptr);
  IJL15Properties properties{};

  if (runtime.initialize(&properties) != 0) {
    std::free(pixels);
    return false;
  }

  properties.dibBytes = pixels;
  properties.dibWidth = paddedWidth;
  properties.dibHeight = locked.dwHeight;
  properties.dibPadding = ((3u * static_cast<unsigned>(paddedWidth) + 3u) & ~3u) - 3u * static_cast<unsigned>(paddedWidth);
  properties.jpegFile = filename.c_str();
  properties.dibChannels = 3;
  properties.dibColor = 2;
  properties.jpegWidth = locked.dwWidth;
  properties.jpegHeight = locked.dwHeight;
  const int status = runtime.write(&properties, 8);
  std::free(pixels);

  return status == 0;
}
