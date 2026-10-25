#include "Resource/Bitmap/BitmapImage.h"

#include <bit>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <windows.h>

namespace {
  // INLINED
  std::uint16_t Read16(const unsigned char* data) {
    std::uint16_t value;
    std::memcpy(&value, data, sizeof(value));
    return value;
  }

  // INLINED
  std::uint32_t Read32(const unsigned char* data) {
    std::uint32_t value;
    std::memcpy(&value, data, sizeof(value));
    return value;
  }

  // INLINED
  int Signed(std::uint32_t value) {
    return std::bit_cast<std::int32_t>(value);
  }

  // INLINED
  std::size_t PixelBytes(int width, int height) {
    const std::uint32_t count = static_cast<std::uint32_t>(width) * static_cast<std::uint32_t>(height);
    return count > 0x3FFFFFFFu ? 0xFFFFFFFFu : count * 4u;
  }

  // INLINED
  std::uint32_t Color(const unsigned char* data, unsigned alpha) {
    return data[0] | (data[1] << 8) | (data[2] << 16) | (alpha << 24);
  }
}

// INLINED
BitmapImage::BitmapImage() : alpha(0), pixels(nullptr), auxiliary(nullptr) {
}

// INLINED
void BitmapImage::Clear() {
  if (this->pixels) {
    std::free(this->pixels);
  }

  if (this->auxiliary) {
    std::free(this->auxiliary);
  }

  this->width = this->height = 0;
  this->pixels = nullptr;
  this->auxiliary = nullptr;
}

// OFFSET: 0x0074AB00
// FUNCTION: fn_07869
bool BitmapImage::DecodeBitmap(const void* input, int, bool iconOnly) {
  this->Clear();
  const auto* data = static_cast<const unsigned char*>(input);

  static constexpr unsigned char signature[] = {0xC3, 0xD1, 0xBE, 0xCB, 0xC0, 0xCC, 0xBB, 0xA1, 0xC5, 0xE4, 0xB4, 0xCF, 0};
  unsigned char prefix[13];
  std::memcpy(prefix, data, 12);
  prefix[12] = 0;
  const bool custom = std::strcmp(reinterpret_cast<const char*>(prefix), reinterpret_cast<const char*>(signature)) == 0;
  unsigned bits;
  unsigned char* rgb;
  const unsigned char* source;
  std::uint32_t palette[256];

  if (custom) {
    const std::uint32_t dimensions = (Read32(data + 12) >> 1) - 0x01929300u;
    const int width = dimensions >> 16;
    const int height = static_cast<int>(dimensions & 0xFFFFu) - 1;

    if (data[16] == 0) {
      bits = 24;
    } else if (data[16] == 1) {
      bits = 8;
    } else {
      OutputDebugStringA("Only 16M or 8bit color format supported");
      return false;
    }

    if (iconOnly && (width != 24 || height != 24)) {
      return false;
    }

    this->width = width;
    this->height = height;
    rgb = static_cast<unsigned char*>(std::malloc(3u * width * height));
    this->pixels = static_cast<std::uint32_t*>(std::malloc(PixelBytes(width, height)));
    source = data + 17;

    if (bits == 8) {
      unsigned count = Read32(source);
      source += 4;
      if (!count) {
        count = 256;
      }

      const unsigned char* first = source;
      const unsigned char* second = source + 2u * count;
      source += 4u * count;

      for (unsigned i = 0; i < count; ++i) {
        palette[i] = first[2 * i + 1] | (first[2 * i] << 8) | (second[2 * i + 1] << 16) | (second[2 * i] << 24);
      }
    } else {
      const std::uint32_t count = static_cast<std::uint32_t>(width) * height;
      auto* first = static_cast<unsigned char*>(std::malloc(3u * count));
      auto* second = static_cast<unsigned char*>(std::malloc(2u * count));
      std::memcpy(first, source, 2u * count);
      source += 2u * count;
      std::memcpy(second, source, count);
      source += count;

      for (std::uint32_t i = 0; i < count; ++i) {
        rgb[3 * i] = second[i];
        rgb[3 * i + 1] = first[2 * i];
        rgb[3 * i + 2] = first[2 * i + 1];
      }

      std::free(first);
      std::free(second);
    }
  } else {
    if (Read16(data) != 0x4D42) {
      return false;
    }

    bits = Read16(data + 28);
    if (bits != 24 && bits != 8) {
      OutputDebugStringA("Only 16M or 8bit color format supported");
      return false;
    }

    const int width = Signed(Read32(data + 18));
    const int height = Signed(Read32(data + 22));
    if (iconOnly && (width != 24 || height != 24)) {
      return false;
    }

    this->width = width;
    this->height = height;
    rgb = static_cast<unsigned char*>(std::malloc(3u * width * height));
    this->pixels = static_cast<std::uint32_t*>(std::malloc(PixelBytes(width, height)));
    const std::uint32_t offset = Read32(data + 10);

    if (bits == 8) {
      const unsigned count = Read32(data + 46) ? Read32(data + 46) : 256;
      std::memcpy(palette, data + 54, 4u * count);

      if (offset > 54u + 4u * count) {
        return false;
      }

      source = data + offset;
    } else {
      if (offset > 54) {
        return false;
      }

      source = data + offset;
      const std::uint32_t alignedWidth = (static_cast<std::uint32_t>(width) + 3) & ~3u;

      for (int y = height - 1; y >= 0; --y) {
        std::memcpy(rgb + 3u * y * width, source, 3u * width);
        source += 3u * width;
        if (alignedWidth != static_cast<std::uint32_t>(width)) {
          source += static_cast<std::uint32_t>(width) - alignedWidth + 4u;
        }
      }
    }
  }

  if (bits == 8) {
    const std::uint32_t stride = (static_cast<std::uint32_t>(this->width) + 3) & ~3u;

    for (int y = this->height - 1; y >= 0; --y) {
      for (int x = 0; x < this->width; ++x) {
        const std::uint32_t value = palette[source[x]];
        auto* pixel = rgb + 3u * (static_cast<std::uint32_t>(y) * this->width + x);
        pixel[0] = static_cast<unsigned char>(value);
        pixel[1] = static_cast<unsigned char>(value >> 8);
        pixel[2] = static_cast<unsigned char>(value >> 16);
      }

      source += stride;
    }
  }

  for (int y = 0; y < this->height; ++y) {
    const std::uint32_t row = static_cast<std::uint32_t>(y) * this->width;

    this->pixels[row] = Color(rgb + 3u * row, 255);

    for (int x = 1; x < this->width; ++x) {
      this->pixels[row + x] = Color(rgb + 3u * (row + x), 255);
    }
  }

  std::free(rgb);
  this->alpha = 0;
  return true;
}

// OFFSET: 0x0074C300
// FUNCTION: fn_07873
bool BitmapImage::DecodeTga(const void* input, int) {
  const auto* data = static_cast<const unsigned char*>(input);
  if (data[1] != 0 || (data[2] != 10 && data[2] != 2)) {
    return false;
  }

  const unsigned char* source = data + 18 + data[0];
  this->width = Read16(data + 12);
  this->height = Read16(data + 14);

  this->pixels = static_cast<std::uint32_t*>(std::malloc(PixelBytes(this->width, this->height)));
  const bool rgba = data[16] == 32;
  unsigned alpha = 255;
  // INLINED
  auto read = [&]() {
    const unsigned char* pixel = source;
    source += 3;
    if (rgba) {
      alpha = *source++;
    }

    return Color(pixel, alpha);
  };

  for (int y = this->height - 1; y >= 0; --y) {
    int x = 0;

    while (x < this->width) {
      if (data[2] == 10) {
        const unsigned packet = *source++;
        const unsigned count = (packet & 127) + 1;
        if (packet & 128) {
          const std::uint32_t value = read();

          for (unsigned i = 0; i < count; ++i) {
            this->pixels[static_cast<std::uint32_t>(y) * this->width + x++] = value;
          }
        } else {
          for (unsigned i = 0; i < count; ++i) {
            this->pixels[static_cast<std::uint32_t>(y) * this->width + x++] = read();
          }
        }
      } else {
        this->pixels[static_cast<std::uint32_t>(y) * this->width + x++] = read();
      }
    }
  }

  this->alpha = 1;
  return true;
}
