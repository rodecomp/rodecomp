#include "Render/DX7/RenderSystemDX7.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/RendererBackBuffer.h"
#include "Render/RendererDX7.h"
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
  std::uint16_t PackedColor(const DeviceOwnerDX7* owner, int red, int green, int blue) {
    if (!owner) {
      return 0;
    }

    return static_cast<std::uint16_t>(
      (static_cast<std::uint16_t>(blue >> owner->blue.expansion) << owner->blue.shift) +
      (static_cast<std::uint16_t>(green >> owner->green.expansion) << owner->green.shift) +
      (static_cast<std::uint16_t>(red >> owner->red.expansion) << owner->red.shift)
    );
  }

  // INLINED
  unsigned char ExpandedChannel(unsigned value, DWORD mask, DWORD shift, DWORD expansion) {
    return static_cast<unsigned char>(static_cast<unsigned char>((value & mask) >> shift) << expansion);
  }
}

// OFFSET: 0x0055D860
// FUNCTION: fn_02109
bool CRendererDX7::SaveBitmap(const char* filename, CBitmapRes* overlay) {
  if (!this->LockBackBuffer()) {
    return false;
  }

  std::FILE* file = std::fopen(filename, "wb");

  if (!file) {
    return false;
  }

  BITMAPFILEHEADER header;
  header.bfType = 0x4D42;
  header.bfReserved1 = 0;
  header.bfReserved2 = 0;
  header.bfOffBits = 54;
  header.bfSize = 3 * (this->viewport.width * this->viewport.height + 18);
  std::fwrite(&header, sizeof(header), 1, file);
  BITMAPINFOHEADER information;
  information.biSize = sizeof(information);
  information.biWidth = this->viewport.width;
  information.biHeight = this->viewport.height;
  information.biPlanes = 1;
  information.biBitCount = 24;
  information.biCompression = 0;
  information.biSizeImage = 0;
  information.biXPelsPerMeter = 0;
  information.biYPelsPerMeter = 0;
  information.biClrUsed = 0;
  information.biClrImportant = 0;
  std::fwrite(&information, sizeof(information), 1, file);

  DeviceOwnerDX7* owner = g_deviceOwnerDX7;
  const DWORD redMask = owner ? owner->redMask : 0;
  const DWORD greenMask = owner ? owner->greenMask : 0;
  const DWORD blueMask = owner ? owner->blueMask : 0;
  const DWORD redShift = owner ? owner->red.shift : 0;
  const DWORD greenShift = owner ? owner->green.shift : 0;
  const DWORD blueShift = owner ? owner->blue.shift : 0;
  const DWORD redExpansion = owner ? owner->red.expansion : 8;
  const DWORD greenExpansion = owner ? owner->green.expansion : 8;
  const DWORD blueExpansion = owner ? owner->blue.expansion : 8;
  const std::uint16_t transparent = reinterpret_cast<const DeviceOwnerDX7Handle&>(g_deviceOwnerDX7).PackColor(255, 0, 255);
  unsigned char row[10800];

  for (int y = this->viewport.height - 1; y >= 0; --y) {
    const unsigned char* source = static_cast<const unsigned char*>(this->backBuffer.pixels) + this->backBuffer.pitch * y;
    unsigned char* destination = row;

    if (this->backBuffer.bitsPerPixel == 16) {
      for (int x = 0; x < this->viewport.width; ++x, destination += 3) {
        int red;
        int green;
        int blue;
        bool fromOverlay = false;

        if (x >= this->viewport.width - overlay->image.width && y >= this->viewport.height - overlay->image.height) {
          const int overlayX = x - (this->viewport.width - overlay->image.width);
          const int overlayY = y - (this->viewport.height - overlay->image.height);

          if (overlayX >= 0 && overlayX < overlay->image.width && overlayY >= 0 && overlayY < overlay->image.height) {
            const unsigned color = overlay->image.pixels[overlayX + overlayY * overlay->image.width];
            red = (color >> 16) & 0xFF;
            green = (color >> 8) & 0xFF;
            blue = color & 0xFF;
            fromOverlay = PackedColor(owner, red, green, blue) != transparent;
          }
        }

        if (!fromOverlay) {
          const std::uint16_t pixel = reinterpret_cast<const std::uint16_t*>(source)[x];
          red = ExpandedChannel(pixel, redMask, redShift, redExpansion);
          green = ExpandedChannel(pixel, greenMask, greenShift, greenExpansion);
          blue = ExpandedChannel(pixel, blueMask, blueShift, blueExpansion);
        }

        destination[0] = static_cast<unsigned char>(blue);
        destination[1] = static_cast<unsigned char>(green);
        destination[2] = static_cast<unsigned char>(red);
      }
    } else {
      const unsigned char* pixel = source;

      for (int x = 0; x < this->viewport.width; ++x, pixel += 4, destination += 3) {
        if (x >= this->viewport.width - overlay->image.width && y >= this->viewport.height - overlay->image.height) {
          const int overlayX = x - (this->viewport.width - overlay->image.width);
          const int overlayY = y - (this->viewport.height - overlay->image.height);

          if (overlayX >= 0 && overlayX < overlay->image.width && overlayY >= 0 && overlayY < overlay->image.height) {
            const unsigned color = overlay->image.pixels[overlayX + overlayY * overlay->image.width];

            if ((color & 0xFFFFFF) != 0xFF00FF) {
              std::memcpy(destination, &color, 3);
              continue;
            }
          }
        }

        std::memcpy(destination, pixel, 3);
      }
    }

    std::fwrite(row, 3 * this->viewport.width, 1, file);
    owner = g_deviceOwnerDX7;
  }

  this->UnlockBackBuffer();
  std::fclose(file);
  return true;
}

// OFFSET: 0x0055DDF0
// FUNCTION: fn_02110
bool CRendererDX7::SaveJpeg(const char* filename, CBitmapRes* overlay) {
  if (!this->LockBackBuffer()) {
    return false;
  }

  auto* pixels = static_cast<unsigned char*>(std::malloc(3 * this->viewport.width * this->viewport.height));
  const DWORD redMask = g_deviceOwnerDX7 ? g_deviceOwnerDX7->redMask : 0;
  const DWORD greenMask = g_deviceOwnerDX7 ? g_deviceOwnerDX7->greenMask : 0;
  const DWORD blueMask = g_deviceOwnerDX7 ? g_deviceOwnerDX7->blueMask : 0;
  const DWORD redShift = g_deviceOwnerDX7 ? g_deviceOwnerDX7->red.shift : 0;
  const DWORD greenShift = g_deviceOwnerDX7 ? g_deviceOwnerDX7->green.shift : 0;
  const DWORD blueShift = g_deviceOwnerDX7 ? g_deviceOwnerDX7->blue.shift : 0;
  const DWORD redExpansion = g_deviceOwnerDX7 ? g_deviceOwnerDX7->red.expansion : 8;
  const DWORD greenExpansion = g_deviceOwnerDX7 ? g_deviceOwnerDX7->green.expansion : 8;
  const DWORD blueExpansion = g_deviceOwnerDX7 ? g_deviceOwnerDX7->blue.expansion : 8;
  const std::uint16_t transparent = PackedColor(g_deviceOwnerDX7, 255, 0, 255);

  for (int y = this->viewport.height - 1; y >= 0; --y) {
    const unsigned char* source = static_cast<const unsigned char*>(this->backBuffer.pixels) + this->backBuffer.pitch * y;
    unsigned char* destination = pixels + 3 * (this->viewport.width * y);

    if (this->backBuffer.bitsPerPixel == 16) {
      for (int x = 0; x < this->viewport.width; ++x, destination += 3) {
        int red;
        int green;
        int blue;
        bool fromOverlay = false;

        if (x >= this->viewport.width - overlay->image.width && y >= this->viewport.height - overlay->image.height) {
          const int overlayX = x - (this->viewport.width - overlay->image.width);
          const int overlayY = y - (this->viewport.height - overlay->image.height);

          if (overlayX >= 0 && overlayX < overlay->image.width && overlayY >= 0 && overlayY < overlay->image.height) {
            const unsigned color = overlay->image.pixels[overlayX + overlayY * overlay->image.width];
            red = (color >> 16) & 0xFF;
            green = (color >> 8) & 0xFF;
            blue = color & 0xFF;
            fromOverlay = PackedColor(g_deviceOwnerDX7, red, green, blue) != transparent;
          }
        }

        if (!fromOverlay) {
          const std::uint16_t pixel = reinterpret_cast<const std::uint16_t*>(source)[x];
          red = ExpandedChannel(pixel, redMask, redShift, redExpansion);
          green = ExpandedChannel(pixel, greenMask, greenShift, greenExpansion);
          blue = ExpandedChannel(pixel, blueMask, blueShift, blueExpansion);
        }

        destination[0] = static_cast<unsigned char>(blue);
        destination[1] = static_cast<unsigned char>(green);
        destination[2] = static_cast<unsigned char>(red);
      }
    } else {
      const unsigned char* pixel = source;

      for (int x = 0; x < this->viewport.width; ++x, pixel += 4, destination += 3) {
        if (x >= this->viewport.width - overlay->image.width && y >= this->viewport.height - overlay->image.height) {
          const int overlayX = x - (this->viewport.width - overlay->image.width);
          const int overlayY = y - (this->viewport.height - overlay->image.height);

          if (overlayX >= 0 && overlayX < overlay->image.width && overlayY >= 0 && overlayY < overlay->image.height) {
            const unsigned color = overlay->image.pixels[overlayX + overlayY * overlay->image.width];

            if ((color & 0xFFFFFF) != 0xFF00FF) {
              std::memcpy(destination, &color, 3);
              continue;
            }
          }
        }

        std::memcpy(destination, pixel, 3);
      }
    }
  }

  this->UnlockBackBuffer();
  IJL15Properties properties{};

  if (ijlInit(&properties) != 0) {
    return false;
  }

  properties.dibWidth = this->viewport.width;
  properties.dibHeight = this->viewport.height;
  properties.dibBytes = pixels;
  properties.dibPadding = ((3 * this->viewport.width + 3) & ~3) - 3 * this->viewport.width;
  properties.jpegFile = filename;
  properties.dibChannels = 3;
  properties.dibColor = 2;
  properties.jpegWidth = this->viewport.width;
  properties.jpegHeight = this->viewport.height;

  if (ijlWrite(&properties, 8) != 0) {
    std::free(pixels);
    return false;
  }

  std::free(pixels);
  return true;
}

// OFFSET: 0x0055E340
// FUNCTION: fn_02111
bool CRendererDX7::SaveLogo(std::string filename) {
  auto* logo = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("scr_logo.bmp")));

  if (!logo) {
    return false;
  }

  DDSURFACEDESC2 locked = {sizeof(DDSURFACEDESC2)};
  IDirectDrawSurface7* target = g_deviceOwnerDX7 ? g_deviceOwnerDX7->renderTarget : nullptr;

  if (FAILED(target->Lock(nullptr, &locked, DDLOCK_WAIT, nullptr))) {
    return false;
  }

  const int pixelBytes = locked.lPitch / locked.dwWidth;
  const int paddedWidth = locked.lPitch / pixelBytes;
  const int size = 3 * paddedWidth * locked.dwHeight;
  auto* pixels = static_cast<unsigned char*>(std::malloc(size));
  const auto& image = logo->image;

  for (DWORD y = 0; y < locked.dwHeight; ++y) {
    const unsigned char* source = static_cast<const unsigned char*>(locked.lpSurface) + y * locked.lPitch;
    unsigned char* destination = pixels + 3 * paddedWidth * y;
    int remaining = size - 3 * paddedWidth * y;

    for (DWORD x = 0; x < locked.dwWidth && remaining > 0; ++x, destination += 3, remaining -= 3) {
      const unsigned char* pixel = source + x * pixelBytes;

      if (x >= locked.dwWidth - image.width && y >= locked.dwHeight - image.height) {
        const std::uint32_t* color = &image.pixels[(image.height - locked.dwHeight + y) * image.width + (image.width - locked.dwWidth + x)];

        if ((*color & 0xFFFFFF) != 0xFF00FF) {
          memcpy_s(destination, remaining, color, 3);
          continue;
        }
      }

      if (locked.ddpfPixelFormat.dwRGBBitCount == 16) {
        const std::uint16_t value = *reinterpret_cast<const std::uint16_t*>(pixel);
        destination[0] = ExpandedChannel(
          value, g_deviceOwnerDX7 ? g_deviceOwnerDX7->blueMask : 0, g_deviceOwnerDX7 ? g_deviceOwnerDX7->blue.shift : 0,
          g_deviceOwnerDX7 ? g_deviceOwnerDX7->blue.expansion : 8
        );
        destination[1] = ExpandedChannel(
          value, g_deviceOwnerDX7 ? g_deviceOwnerDX7->greenMask : 0, g_deviceOwnerDX7 ? g_deviceOwnerDX7->green.shift : 0,
          g_deviceOwnerDX7 ? g_deviceOwnerDX7->green.expansion : 8
        );
        destination[2] = ExpandedChannel(
          value, g_deviceOwnerDX7 ? g_deviceOwnerDX7->redMask : 0, g_deviceOwnerDX7 ? g_deviceOwnerDX7->red.shift : 0,
          g_deviceOwnerDX7 ? g_deviceOwnerDX7->red.expansion : 8
        );
      } else {
        memcpy_s(destination, remaining, pixel, 3);
      }
    }
  }

  target = g_deviceOwnerDX7 ? g_deviceOwnerDX7->renderTarget : nullptr;
  target->Unlock(nullptr);
  IJL15Properties properties;
  SecureZeroMemory(&properties, sizeof(properties));

  if (ijlInit(&properties) != 0) {
    std::free(pixels);
    return false;
  }

  properties.dibBytes = pixels;
  properties.dibWidth = paddedWidth;
  properties.dibHeight = locked.dwHeight;
  properties.dibPadding = ((3 * paddedWidth + 3) & ~3) - 3 * paddedWidth;
  properties.jpegFile = filename.c_str();
  properties.jpegWidth = locked.dwWidth;
  properties.dibChannels = 3;
  properties.dibColor = 2;
  properties.jpegHeight = locked.dwHeight;

  if (ijlWrite(&properties, 8) != 0) {
    std::free(pixels);
    return false;
  }

  std::free(pixels);
  return true;
}
