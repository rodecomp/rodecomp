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
  std::uint32_t Color(unsigned alpha, unsigned red, unsigned green, unsigned blue) {
    return (((((alpha << 8) + red) << 8) + green) << 8) + blue;
  }

  // INLINED
  template <typename Count>
  __forceinline const unsigned char* ReadPalette(RGBQUAD* palette, unsigned char* first, unsigned char* second, const unsigned char* source, Count count) {
    std::memcpy(first, source, count * 2);
    source += count * 2;
    std::memcpy(second, source, count * 2);
    source += count * 2;

    int offset = 0;
    for (Count i = 0; i < count; ++i) {
      palette[i].rgbGreen = first[offset];
      palette[i].rgbReserved = second[offset];
      palette[i].rgbBlue = first[offset + 1];
      palette[i].rgbRed = second[offset + 1];
      offset += 2;
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
        const unsigned char index = row[x];
        pixel[0] = palette[index].rgbBlue;
        pixel[1] = palette[index].rgbGreen;
        pixel[2] = palette[index].rgbRed;
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
  for (int y = 0; y < this->height; ++y) {
    for (int x = 0; x < this->width; ++x) {
      const int index = this->width * y + x;
      const std::uint32_t value = this->pixels[index];
      unsigned red = (value >> 16) & 255;
      unsigned green = (value >> 8) & 255;
      const unsigned alpha = value >> 24;
      unsigned blue = value & 255;
      if (red != 255 || blue != 255 || green != 0) {
        int intensity = (red + green + blue) / 3;
        const int minimum = mode != 1 ? 100 : 120;
        if (intensity < minimum) {
          intensity = minimum;
        }
        red = intensity;
        green = intensity;
        blue = intensity;
        if (mode == 2) {
          red = 200;
        } else if (mode == 3) {
          green = 200;
        } else if (mode == 4) {
          blue = 200;
        }
      }
      static_cast<std::uint32_t*>(this->auxiliary)[index] = Color(alpha, red, green, blue);
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
  unsigned char row[4096];
  unsigned char* rgb;

  if (std::strcmp(prefix, "총알이빨토니") == 0) {
    const unsigned char* source = data + 12;
    const std::uint32_t dimensions = (Read32(source) >> 1) - 0x01929300u;
    source += 4;
    const int height = static_cast<int>(dimensions & 0xFFFFu) - 1;
    const unsigned char format = *source++;
    const int width = dimensions >> 16;
    WORD bits;

    switch (format) {
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
          pixel[0] = *(second + this->width * y + x);
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
    const unsigned char* colors = data + sizeof(BITMAPFILEHEADER);
    std::memcpy(&infoHeader, colors, sizeof(infoHeader));
    colors += sizeof(infoHeader);

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

        if (stride - this->width != 0) {
          source += this->width - stride + 4;
        }
      }
    }
  }

  for (int y = 0; y < this->height; ++y) {
    const unsigned char* source = rgb + this->width * y * 3;

    if (source) {
      std::uint32_t* target = this->pixels + this->width * y;

      if (target) {
        *target++ = Color(255, source[2], source[1], source[0]);

        for (int x = 1; x < this->width; ++x) {
          source += 3;
          *target++ = Color(255, source[2], source[1], source[0]);
        }
      }
    }
  }

  if (rgb) {
    delete[] rgb;
  }

  this->alpha = 0;
  return true;
}

#pragma pack(push, 1)
struct TgaHeader {
  /* 0000 */ std::uint8_t idLength;
  /* 0001 */ std::uint8_t colorMapType;
  /* 0002 */ std::uint8_t imageType;
  /* 0003 */ std::uint16_t colorMapStart;
  /* 0005 */ std::uint16_t colorMapLength;
  /* 0007 */ std::uint8_t colorMapDepth;
  /* 0008 */ std::uint16_t xOrigin;
  /* 000A */ std::uint16_t yOrigin;
  /* 000C */ std::uint16_t width;
  /* 000E */ std::uint16_t height;
  /* 0010 */ std::uint8_t bitsPerPixel;
  /* 0011 */ std::uint8_t descriptor;
};
#pragma pack(pop)

struct TgaPixel {
  /* 0000 */ std::uint8_t blue;
  /* 0001 */ std::uint8_t green;
  /* 0002 */ std::uint8_t red;
  /* 0003 */ std::uint8_t alpha;
};

// OFFSET: 0x0074C300
// FUNCTION: fn_07873
bool BitmapImage::DecodeTga(const void* input, int) {
  const auto* source = static_cast<const unsigned char*>(input);
  const TgaHeader header = *static_cast<const TgaHeader*>(input);
  source += sizeof(header);
  if (header.colorMapType != 0 || (header.imageType != 10 && header.imageType != 2)) {
    return false;
  }

  if (header.idLength) {
    source += header.idLength;
  }

  this->width = header.width;
  this->height = header.height;
  this->pixels = new std::uint32_t[this->width * this->height];
  std::uint32_t* row = this->pixels + (this->height - 1) * this->width;
  TgaPixel pixel = {};
  pixel.alpha = 255;
  // INLINED
  auto read = [&source, &pixel](std::uint8_t bitsPerPixel) {
    pixel.blue = *source++;
    pixel.green = *source++;
    pixel.red = *source++;
    if (bitsPerPixel == 32) {
      pixel.alpha = *source++;
    }

    return Color(pixel.alpha, pixel.red, pixel.green, pixel.blue);
  };

  for (int y = 0; y < this->height; ++y) {
    int x = 0;

    while (x < this->width) {
      if (header.imageType == 10) {
        const unsigned char packet = *source++;
        std::uint16_t count = (packet & 127) + 1;
        if (packet & 128) {
          const std::uint32_t value = read(header.bitsPerPixel);

          while (count--) {
            row[x++] = value;
          }
        } else {
          while (count--) {
            row[x++] = read(header.bitsPerPixel);
          }
        }
      } else {
        row[x++] = read(header.bitsPerPixel);
      }
    }

    row -= this->width;
  }

  this->alpha = 1;
  return true;
}
