#include "Resource/Bitmap/BitmapImage.h"

#include <bit>
#include <cstdlib>
#include <cstring>
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
  std::uint32_t Color(const unsigned char* data, unsigned alpha) {
    return ((alpha << 8 | data[2]) << 8 | data[1]) << 8 | data[0];
  }

  // INLINED
  __forceinline const unsigned char* ReadPalette(RGBQUAD* palette, unsigned char* first, unsigned char* second, const unsigned char* source, std::uint32_t count) {
    std::memcpy(first, source, count * 2);
    source += count * 2;
    std::memcpy(second, source, count * 2);
    source += count * 2;

    for (std::uint32_t i = 0; i < count; ++i) {
      palette[i].rgbGreen = first[2 * i];
      palette[i].rgbReserved = second[2 * i];
      palette[i].rgbBlue = first[2 * i + 1];
      palette[i].rgbRed = second[2 * i + 1];
    }

    return source;
  }

  // INLINED
  __forceinline void DecodeIndexedRows(const BitmapImage& image, unsigned char* rgb, const unsigned char* source, const RGBQUAD* palette, unsigned char* row) {
    for (int y = image.height - 1; y >= 0; --y) {
      const std::uint32_t stride = (image.width + 3) & ~3u;
      std::memcpy(row, source, stride);
      source += stride;
      unsigned char* pixel = rgb + image.width * y * 3;

      for (int x = 0; x < image.width; ++x) {
        const RGBQUAD& color = palette[row[x]];
        pixel[0] = color.rgbBlue;
        pixel[1] = color.rgbGreen;
        pixel[2] = color.rgbRed;
        pixel += 3;
      }
    }
  }
}

// OFFSET: 0x0074A680
// FUNCTION: fn_07866
bool BitmapImage::CreateAuxiliary(int mode) {
  if (!this->pixels || this->auxiliary) {
    return false;
  }

  this->auxiliary = new std::uint32_t[this->width * this->height];
  std::memset(this->auxiliary, 255, 4u * this->width * this->height);
  auto* output = static_cast<std::uint32_t*>(this->auxiliary);
  for (int y = 0; y < this->height; ++y) {
    for (int x = 0; x < this->width; ++x) {
      const int index = this->width * y + x;
      const std::uint32_t value = this->pixels[index];
      unsigned red = (value >> 16) & 255;
      unsigned green = (value >> 8) & 255;
      unsigned blue = value & 255;
      if (red != 255 || blue != 255 || green != 0) {
        unsigned intensity = (red + green + blue) / 3;
        const unsigned minimum = mode == 1 ? 120 : 100;
        if (intensity < minimum) {
          intensity = minimum;
        }
        red = green = blue = intensity;
        if (mode == 2) {
          red = 200;
        } else if (mode == 3) {
          green = 200;
        } else if (mode == 4) {
          blue = 200;
        }
      }
      output[index] = (((value >> 24) << 8 | red) << 8 | green) << 8 | blue;
    }
  }
  return true;
}

// OFFSET: 0x0074A7F0
// FUNCTION: fn_07867
std::uint32_t BitmapImage::Pixel(int x, int y) const {
  if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
    return 0x00FF0000;
  }
  return this->pixels[x + y * this->width];
}

// OFFSET: 0x0074C540
// FUNCTION: fn_07874
void BitmapImage::ClearAuxiliary() {
  if (this->auxiliary) {
    delete[] static_cast<std::uint32_t*>(this->auxiliary);
    this->auxiliary = nullptr;
  }
}

// OFFSET: 0x0074AB00
// FUNCTION: fn_07869
bool BitmapImage::DecodeBitmap(const void* input, int, bool iconOnly) {
  this->Clear();
  const auto* data = static_cast<const unsigned char*>(input);

  BITMAPFILEHEADER fileHeader;
  std::memcpy(&fileHeader, data, sizeof(fileHeader));
  char prefix[13];
  std::memcpy(prefix, data, 12);
  prefix[12] = 0;
  RGBQUAD palette[256];
  unsigned char row[3584];
  unsigned char* rgb;

  if (std::strcmp(prefix, "총알이빨토니") == 0) {
    const std::uint32_t dimensions = (Read32(data + 12) >> 1) - 0x01929300u;
    const int height = static_cast<int>(dimensions & 0xFFFFu) - 1;
    const unsigned char* source = data + 17;
    const int width = dimensions >> 16;
    WORD bits;

    switch (data[16]) {
    case 0:
      bits = 24;
      break;
    case 1:
      bits = 8;
      break;
    default:
      OutputDebugStringA("Only 16M or 8bit color format supported");
      return false;
    }

    if (iconOnly && (width != 24 || height != 24)) {
      return false;
    }

    this->height = height;
    this->width = width;
    rgb = new unsigned char[height * width * 3];
    this->pixels = new std::uint32_t[this->width * this->height];

    if (bits == 8) {
      const std::uint32_t count = Read32(source);
      source += 4;
      unsigned char first[512];
      unsigned char second[512];

      if (count != 0) {
        source = ReadPalette(palette, first, second, source, count);
      } else {
        source = ReadPalette(palette, first, second, source, 256);
      }

      DecodeIndexedRows(*this, rgb, source, palette, row);
    } else if (bits == 24) {
      auto* first = new unsigned char[this->width * this->height * 3];
      auto* second = new unsigned char[this->width * this->height * 2];

      unsigned char* target = first;
      for (int y = this->height - 1; y >= 0; --y) {
        std::memcpy(target, source, this->width * 2);
        target += this->width * 2;
        source += this->width * 2;
      }

      target = second;
      for (int y = this->height - 1; y >= 0; --y) {
        std::memcpy(target, source, this->width);
        target += this->width;
        source += this->width;
      }

      unsigned char* pixel = rgb;
      int index = 0;
      for (int y = 0; y < this->height; ++y) {
        for (int x = 0; x < this->width; ++x) {
          pixel[0] = second[x + this->width * y];
          pixel[1] = first[index];
          pixel[2] = first[index + 1];
          index += 2;
          pixel += 3;
        }
      }

      delete[] first;
      delete[] second;
    }
  } else {
    if (fileHeader.bfType != 0x4D42) {
      return false;
    }

    BITMAPINFOHEADER infoHeader;
    std::memcpy(&infoHeader, data + sizeof(BITMAPFILEHEADER), sizeof(infoHeader));
    const unsigned char* colors = data + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 8) {
      OutputDebugStringA("Only 16M or 8bit color format supported");
      return false;
    }

    if (iconOnly && (infoHeader.biWidth != 24 || infoHeader.biHeight != 24)) {
      return false;
    }

    this->height = infoHeader.biHeight;
    const std::uint32_t stride = (infoHeader.biWidth + 3) & ~3u;
    this->width = infoHeader.biWidth;
    rgb = new unsigned char[infoHeader.biHeight * infoHeader.biWidth * 3];
    this->pixels = new std::uint32_t[this->width * this->height];

    if (infoHeader.biBitCount == 8) {
      std::uint32_t paletteSize;

      if (infoHeader.biClrUsed != 0) {
        paletteSize = infoHeader.biClrUsed * sizeof(RGBQUAD);
        std::memcpy(palette, colors, paletteSize);
      } else {
        std::memcpy(palette, colors, sizeof(palette));
        paletteSize = sizeof(palette);
      }

      if (fileHeader.bfOffBits > static_cast<std::uint32_t>(colors - data) + paletteSize) {
        return false;
      }

      DecodeIndexedRows(*this, rgb, data + fileHeader.bfOffBits, palette, row);
    } else if (infoHeader.biBitCount == 24) {
      if (fileHeader.bfOffBits > sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)) {
        return false;
      }

      const unsigned char* source = data + fileHeader.bfOffBits;

      for (int y = this->height - 1; y >= 0; --y) {
        std::memcpy(rgb + this->width * y * 3, source, this->width * 3);
        source += this->width * 3;

        if (stride != static_cast<std::uint32_t>(this->width)) {
          source += this->width - stride + 4;
        }
      }
    }
  }

  for (int y = 0; y < this->height; ++y) {
    const unsigned char* source = rgb + this->width * y * 3;
    std::uint32_t* target = this->pixels + this->width * y;

    if (source && target) {
      *target = Color(source, 255);

      for (int x = 1; x < this->width; ++x) {
        ++target;
        source += 3;
        *target = Color(source, 255);
      }
    }
  }

  if (rgb) {
    delete[] rgb;
  }

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

  this->pixels = new std::uint32_t[this->width * this->height];
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
