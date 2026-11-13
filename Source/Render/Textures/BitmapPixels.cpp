#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/DeviceContext/DCBitmap.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Textures/BitmapPixels.h"
#include "Render/RenderSystemBase.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Surface.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"

#include <algorithm>
#include <cstring>
#include <vector>

// INLINED
BitmapPixels::BitmapPixels(int newWidth, int newHeight) : dc(CreateCompatibleDC(nullptr)), bitmap(nullptr), previous(nullptr), pixels(nullptr) {
  this->Create(newWidth, newHeight);
}

// INLINED
BitmapPixels::~BitmapPixels() {
  SelectObject(this->dc, this->previous);
  DeleteObject(this->bitmap);
  DeleteDC(this->dc);
}

// OFFSET: 0x00547C90
// FUNCTION: fn_01847
void CDCBitmap::UploadTexture(CTexture*& texture) {
  const std::int32_t size = 4 * static_cast<int>(this->m_bitmap.width) * static_cast<int>(this->m_bitmap.height);
  unsigned char* bytes = reinterpret_cast<unsigned char*>(this->m_bitmap.pixels);

  for (std::int32_t offset = 0; offset < size; offset += 4) {
    bytes[offset + 3] = 255;
  }

  if (!texture) {
    texture = g_texMgr.Upload(this->m_bitmap.width, this->m_bitmap.height, this->m_bitmap.pixels, TextureFormat::A1R5G5B5, 0, true, false, false);
  } else {
    texture->Upload(0, 0, this->m_bitmap.width, this->m_bitmap.height, this->m_bitmap.pixels, 0, 0);
  }
}

// INLINED
void BitmapPixels::Create(int newWidth, int newHeight) {
  this->width = newWidth;
  this->height = newHeight;
  this->dirty = true;
  BITMAPINFO info;
  info.bmiHeader.biSize = 40;
  info.bmiHeader.biWidth = newWidth;
  info.bmiHeader.biHeight = -newHeight;
  info.bmiHeader.biPlanes = 1;
  info.bmiHeader.biBitCount = 32;
  info.bmiHeader.biCompression = BI_RGB;
  info.bmiHeader.biSizeImage = 0;
  info.bmiHeader.biXPelsPerMeter = 100;
  info.bmiHeader.biYPelsPerMeter = 100;
  info.bmiHeader.biClrUsed = 0;
  info.bmiHeader.biClrImportant = 0;
  this->bitmap = CreateDIBSection(nullptr, &info, DIB_RGB_COLORS, reinterpret_cast<void**>(&this->pixels), nullptr, 0);

  if (!this->bitmap) {
    this->pixels = nullptr;
  }

  this->previous = SelectObject(this->dc, this->bitmap);
}

// OFFSET: 0x00547A80
// FUNCTION: fn_01845
__declspec(noinline) HGDIOBJ CDCBitmap::Create(int width, int height) {
  this->m_bitmap.width = width;
  this->m_bitmap.dirty = true;
  this->m_bitmap.height = height;
  this->m_cache.Clear();

  BITMAPINFO info;
  info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  info.bmiHeader.biWidth = this->m_bitmap.width;
  info.bmiHeader.biHeight = -static_cast<int>(this->m_bitmap.height);
  info.bmiHeader.biPlanes = 1;
  info.bmiHeader.biBitCount = 32;
  info.bmiHeader.biCompression = BI_RGB;
  info.bmiHeader.biSizeImage = 0;
  info.bmiHeader.biXPelsPerMeter = 100;
  info.bmiHeader.biYPelsPerMeter = 100;
  info.bmiHeader.biClrUsed = 0;
  info.bmiHeader.biClrImportant = 0;
  this->m_bitmap.bitmap = CreateDIBSection(nullptr, &info, DIB_RGB_COLORS, reinterpret_cast<void**>(&this->m_bitmap.pixels), nullptr, 0);

  if (!this->m_bitmap.bitmap) {
    this->m_bitmap.pixels = nullptr;
  }

  this->m_bitmap.previous = SelectObject(this->m_bitmap.dc, this->m_bitmap.bitmap);
  return this->m_bitmap.previous;
}

// OFFSET: 0x00547D40
// FUNCTION: fn_01850
void CDCBitmap::Grow(std::uint32_t newWidth, std::uint32_t newHeight) {
  if (newWidth > this->m_bitmap.width || newHeight > this->m_bitmap.height) {
    SelectObject(this->m_bitmap.dc, this->m_bitmap.previous);
    DeleteObject(this->m_bitmap.bitmap);
    this->Create(newWidth, newHeight);
  }
}

// OFFSET: 0x00547D10
// FUNCTION: fn_01848
bool CDCBitmap::GetDC(HDC* output) {
  this->m_bitmap.dirty = true;
  *output = this->m_bitmap.dc;
  return true;
}

// OFFSET: 0x00547D30
// FUNCTION: fn_01849
void CDCBitmap::ReleaseDC(HDC) {
}

// OFFSET: 0x00547460
// FUNCTION: fn_01841
void CDCBitmap::Fill(const RECT* rectangle, std::uint32_t color) {
  if (!this->m_bitmap.pixels) {
    return;
  }

  this->m_bitmap.dirty = true;
  const std::uint32_t converted = ((color >> 16) & 0xFF) + ((color & 0xFF) << 16) + (color & 0xFF000000) + (color & 0xFF00);

  if (rectangle) {
    for (int y = rectangle->top; y < rectangle->bottom; ++y) {
      for (int x = rectangle->left; x < rectangle->right; ++x) {
        this->m_bitmap.pixels[x + y * this->m_bitmap.width] = converted;
      }
    }
  } else {
    for (std::uint32_t y = 0; y < this->m_bitmap.height; ++y) {
      for (std::uint32_t x = 0; x < this->m_bitmap.width; ++x) {
        this->m_bitmap.pixels[x + y * this->m_bitmap.width] = converted;
      }
    }
  }
}

// OFFSET: 0x00548230
// FUNCTION: fn_01852
void CDCBitmap::Copy(int x, int y, int copyWidth, int copyHeight, const std::uint32_t* source, bool transparent) {
  if (!this->m_bitmap.pixels || !source) {
    return;
  }

  this->m_bitmap.dirty = true;
  const std::int32_t count = static_cast<std::int32_t>(this->m_bitmap.width * this->m_bitmap.height);

  for (int row = 0; row < copyHeight; ++row) {
    const std::uint32_t* input = source;
    for (int column = 0; column < copyWidth; ++column, ++input) {
      const std::uint32_t value = *input;
      if (!transparent || value != 0xFFFF00FF) {
        const std::int32_t index = static_cast<std::int32_t>((row + y) * this->m_bitmap.width + column + x);
        if (index > -1 && count > index) {
          this->m_bitmap.pixels[index] = value;
        }
      }
    }

    source += copyWidth;
  }
}

// OFFSET: 0x00547790
// FUNCTION: fn_01843
void CDCBitmap::CopyFrom(int x, int y, int sourceX, int sourceY, int copyWidth, int copyHeight, const CDCBitmap& source) {
  if (!this->m_bitmap.pixels) {
    return;
  }

  this->m_bitmap.dirty = true;
  std::uint32_t* destination = this->m_bitmap.pixels + x + y * this->m_bitmap.width;
  const std::uint32_t* input = source.m_bitmap.pixels + sourceX + sourceY * source.m_bitmap.width;

  for (int row = 0; row < copyHeight; ++row) {
    if (!destination || !input) {
      break;
    }

    for (int column = 0; column < copyWidth; ++column) {
      if (input[column] != 0xFFFF00FF) {
        destination[column] = input[column];
      }
    }

    input += source.m_bitmap.width;
    destination += this->m_bitmap.width;
  }
}

// OFFSET: 0x00547840
// FUNCTION: fn_01844
void CDCBitmap::BlendFrom(std::uint32_t x, std::uint32_t y, int copyWidth, int copyHeight, const CDCBitmap& source) {
  if (!this->m_bitmap.pixels || x >= this->m_bitmap.width || y >= this->m_bitmap.height) {
    return;
  }

  if (x + copyWidth > this->m_bitmap.width) {
    copyWidth = this->m_bitmap.width - x;
  }

  if (y + copyHeight > this->m_bitmap.height) {
    copyHeight = this->m_bitmap.height - y;
  }

  this->m_bitmap.dirty = true;
  std::uint32_t* destination = this->m_bitmap.pixels + x + y * this->m_bitmap.width;
  const std::uint32_t* input = source.m_bitmap.pixels;

  for (int row = 0; row < copyHeight; ++row) {
    if (!destination || !input) {
      break;
    }

    for (int column = 0; column < copyWidth; ++column) {
      if (source.m_cache.m_alpha) {
        const std::uint32_t target = destination[column];
        const std::uint32_t pixel = input[column];
        const WORD targetAlpha = static_cast<WORD>(target >> 24);
        const WORD sourceAlpha = static_cast<WORD>(pixel >> 24);
        const BYTE blue = GetBValue(target) * (255 - sourceAlpha) / 255 + GetBValue(pixel) * sourceAlpha / 255;
        const BYTE green = GetGValue(target) * (255 - sourceAlpha) / 255 + GetGValue(pixel) * sourceAlpha / 255;
        const BYTE red = GetRValue(target) * (255 - sourceAlpha) / 255 + GetRValue(pixel) * sourceAlpha / 255;
        destination[column] = (std::max(sourceAlpha, targetAlpha) << 24) + (blue << 16) + (green << 8) + red;
      } else if (input[column] != 0xFFFF00FF) {
        destination[column] = input[column] - 0x01000000;
      }
    }

    destination += this->m_bitmap.width;
    input += source.m_bitmap.width;
  }
}

// OFFSET: 0x005489F0
// FUNCTION: fn_01857
void CDCBitmap::Brighten(int x, int y, int copyWidth, int copyHeight, int amount) {
  if (x < 0 || y < 0 || this->m_bitmap.width < static_cast<unsigned>(x) || this->m_bitmap.height < static_cast<unsigned>(y) || copyWidth < 1 || copyHeight < 1) {
    return;
  }

  this->m_bitmap.dirty = true;
  const int right = std::min(static_cast<std::uint32_t>(x + copyWidth), this->m_bitmap.width);
  const int bottom = std::min(static_cast<std::uint32_t>(y + copyHeight), this->m_bitmap.height);

  for (int row = y; row < bottom; ++row) {
    for (int column = x; column < right; ++column) {
      std::uint32_t& pixel = this->m_bitmap.pixels[column + row * this->m_bitmap.width];
      if (pixel != 0xFFFF00FF) {
        const int brightRed = static_cast<int>((pixel >> 16) & 0xFF) + amount;
        const int red = std::min(brightRed >= 0 ? brightRed : 0, 255);
        const int brightGreen = static_cast<int>((pixel >> 8) & 0xFF) + amount;
        const int green = std::min(brightGreen >= 0 ? brightGreen : 0, 255);
        const int brightBlue = static_cast<int>(pixel & 0xFF) + amount;
        const int blue = std::min(brightBlue >= 0 ? brightBlue : 0, 255);
        pixel = 0xFF000000 | ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF);
      }
    }
  }
}

// OFFSET: 0x00548650
// FUNCTION: fn_01855
void BitmapPixels::Dim(int x, int y, int copyWidth, int copyHeight, int percent) {
  if (x < 0 || y < 0 || this->width < static_cast<unsigned>(x) || this->height < static_cast<unsigned>(y) || copyWidth < 1 || copyHeight < 1) {
    return;
  }

  const int right = std::min(static_cast<std::uint32_t>(x + copyWidth), this->width);
  const int bottom = std::min(static_cast<std::uint32_t>(y + copyHeight), this->height);
  const float factor = static_cast<float>(100 - percent) * 0.01f;

  for (int row = y; row < bottom; ++row) {
    for (int column = x; column < right; ++column) {
      std::uint32_t& pixel = this->pixels[column + row * this->width];
      if (pixel != 0xFFFF00FF) {
        const int red = static_cast<int>(static_cast<BYTE>(pixel >> 16) * factor);
        const int green = static_cast<int>(static_cast<BYTE>(pixel >> 8) * factor);
        const int blue = static_cast<int>(static_cast<BYTE>(pixel) * factor);
        pixel = 0xFF000000 | ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF);
      }
    }
  }
}

// OFFSET: 0x00548780
// FUNCTION: fn_01856
void BitmapPixels::CopyRegion(
  int x, int y, int sourceLeft, int sourceTop, int sourceRight, int sourceBottom, const std::uint32_t* source, int sourceWidth, int sourceHeight,
  bool transparent, bool alpha
) {
  if (!this->pixels || !source) {
    return;
  }

  this->dirty = true;

  for (int sourceY = sourceTop, destinationY = y; sourceY < sourceBottom; ++sourceY, ++destinationY) {
    if (sourceY < 0 || sourceY >= sourceHeight || destinationY < 0 || static_cast<unsigned>(destinationY) >= this->height) {
      continue;
    }

    for (int sourceX = sourceLeft, destinationX = x; sourceX < sourceRight; ++sourceX, ++destinationX) {
      if (sourceX < 0 || sourceX >= sourceWidth || destinationX < 0 || static_cast<unsigned>(destinationX) >= this->width) {
        continue;
      }

      const std::uint32_t input = source[sourceX + sourceY * sourceWidth];
      if (transparent && input == 0xFFFF00FF) {
        continue;
      }

      std::uint32_t& destination = this->pixels[destinationX + destinationY * this->width];
      if (alpha) {
        const std::uint32_t target = destination;
        const std::int64_t targetAlpha = target >> 24;
        const std::int64_t targetChannels[3] = {GetBValue(target), GetGValue(target), GetRValue(target)};
        const std::int64_t sourceAlpha = input >> 24;
        const std::int64_t sourceChannels[3] = {GetBValue(input), GetGValue(input), GetRValue(input)};
        int channels[3] = {};

        for (std::int64_t channel = 0; channel < 3; ++channel) {
          channels[channel] = static_cast<int>(sourceChannels[channel] * sourceAlpha / 255 + targetChannels[channel] * (255 - sourceAlpha) / 255);
        }

        const std::uint32_t outputAlpha = static_cast<std::uint32_t>(std::max(sourceAlpha, targetAlpha));
        this->pixels[destinationX + destinationY * this->width] =
          (outputAlpha << 24) + (static_cast<BYTE>(channels[0]) << 16) + (static_cast<BYTE>(channels[1]) << 8) + static_cast<BYTE>(channels[2]);
      } else {
        destination = input;
      }
    }
  }
}

// INLINED
static std::uint32_t PackPixel(unsigned alpha, unsigned red, unsigned green, unsigned blue) {
  return ((((alpha << 8) + static_cast<unsigned char>(red)) << 8) + static_cast<unsigned char>(green) << 8) + static_cast<unsigned char>(blue);
}

// INLINED
static float SurfacePixelAlpha(int value) {
  const float alpha = static_cast<float>(static_cast<unsigned>(value) >> 12) * 0.06666667f;
  return alpha > 0.97f ? 1.0f : alpha;
}

// OFFSET: 0x005460B0
// FUNCTION: fn_01839
void CDCBitmap::DrawSurface(
  int x, int y, CSurface* source, int sourceX, int sourceY, int sourceWidth, int sourceHeight, bool mirrored, int scaleX, int scaleY, int filter
) {
  if (!source || !this->m_bitmap.pixels) {
    return;
  }

  SurfaceLock locked = source->Lock();
  if (!locked.success) {
    return;
  }

  const auto* input = static_cast<const std::uint16_t*>(locked.pixels);
  const int stride = locked.pitch / 2;
  this->m_bitmap.dirty = true;
  const int format = static_cast<CTexture*>(source)->Format();
  const int width = this->m_bitmap.width;
  std::uint32_t* pixels = this->m_bitmap.pixels;
  unsigned red, green, blue;

  if (format == TextureFormat::A4R4G4B4) {
    if (mirrored) {
      for (int row = 0; row < sourceHeight; ++row) {
        for (int column = sourceWidth - 1; column >= 0; --column) {
          const int value = input[(sourceY + row) * stride + sourceX + column];
          if (!(value & 0xF000)) {
            continue;
          }

          const float alpha = SurfacePixelAlpha(value);
          red = (value >> 4) & 0xF0;
          green = value & 0xF0;
          blue = (value & 0xF) << 4;
          this->ApplyPixelFilter(filter, &red, &green, &blue);

          for (int dy = 0; dy < scaleY; ++dy) {
            for (int dx = 0; dx < scaleX; ++dx) {
              const int destinationX = x + (sourceWidth - column - 1) * scaleX + dx;
              const int destinationY = y + row * scaleY + dy;
              if (destinationX >= 0 && destinationY >= 0 && this->m_bitmap.width > static_cast<unsigned>(destinationX) &&
                  this->m_bitmap.height > static_cast<unsigned>(destinationY)) {
                const float inverse = 1.0f - alpha;
                const int index = destinationX + destinationY * width;
                const unsigned outputRed =
                  static_cast<unsigned>(static_cast<float>((pixels[index] >> 16) & 0xFF) * inverse + static_cast<float>(red) * alpha);
                const unsigned outputGreen =
                  static_cast<unsigned>(static_cast<float>((pixels[index] >> 8) & 0xFF) * inverse + static_cast<float>(green) * alpha);
                const unsigned outputBlue =
                  static_cast<unsigned>(static_cast<float>(pixels[index] & 0xFF) * inverse + static_cast<float>(blue) * alpha);
                pixels[index] = PackPixel(0, outputRed, outputGreen, outputBlue);
              }
            }
          }
        }
      }
    } else {
      for (int row = 0; row < sourceHeight; ++row) {
        for (int column = 0; column < sourceWidth; ++column) {
          const int value = input[(sourceY + row) * stride + sourceX + column];
          if (!(value & 0xF000)) {
            continue;
          }

          const float alpha = SurfacePixelAlpha(value);
          red = (value >> 4) & 0xF0;
          green = value & 0xF0;
          blue = (value & 0xF) << 4;
          this->ApplyPixelFilter(filter, &red, &green, &blue);

          if (!&pixels[(y + (row + 1) * scaleY - 1) * width + x + column * scaleX + scaleX - 1]) {
            continue;
          }

          for (int dy = 0; dy < scaleY; ++dy) {
            for (int dx = 0; dx < scaleX; ++dx) {
              const int destinationX = x + column * scaleX + dx;
              const int destinationY = y + row * scaleY + dy;
              if (destinationX >= 0 && destinationY >= 0 && this->m_bitmap.width > static_cast<unsigned>(destinationX) &&
                  this->m_bitmap.height > static_cast<unsigned>(destinationY)) {
                const float inverse = 1.0f - alpha;
                const int index = destinationX + destinationY * width;
                const unsigned outputRed =
                  static_cast<unsigned>(static_cast<float>((pixels[index] >> 16) & 0xFF) * inverse + static_cast<float>(red) * alpha);
                const unsigned outputGreen =
                  static_cast<unsigned>(static_cast<float>((pixels[index] >> 8) & 0xFF) * inverse + static_cast<float>(green) * alpha);
                const unsigned outputBlue =
                  static_cast<unsigned>(static_cast<float>(pixels[index] & 0xFF) * inverse + static_cast<float>(blue) * alpha);
                pixels[index] = PackPixel(0, outputRed, outputGreen, outputBlue);
              }
            }
          }
        }
      }
    }
  } else if (format == TextureFormat::A1R5G5B5) {
    if (mirrored) {
      for (int row = 0; row < sourceHeight; ++row) {
        for (int column = sourceWidth - 1; column >= 0; --column) {
          const int value = input[(sourceY + row) * stride + sourceX + column];
          if (!(value & 0x8000)) {
            continue;
          }

          red = (value >> 7) & 0xF8;
          green = (value >> 2) & 0xF8;
          blue = (value & 0x1F) << 3;
          this->ApplyPixelFilter(filter, &red, &green, &blue);

          for (int dy = 0; dy < scaleY; ++dy) {
            for (int dx = 0; dx < scaleX; ++dx) {
              const int destinationX = x + (sourceWidth - column - 1) * scaleX + dx;
              const int destinationY = y + row * scaleY + dy;
              if (destinationX >= 0 && destinationY >= 0 && this->m_bitmap.width > static_cast<unsigned>(destinationX) &&
                  this->m_bitmap.height > static_cast<unsigned>(destinationY)) {
                pixels[destinationX + destinationY * width] = PackPixel(0xFF, red, green, blue);
              }
            }
          }
        }
      }
    } else {
      for (int row = 0; row < sourceHeight; ++row) {
        for (int column = 0; column < sourceWidth; ++column) {
          const int value = input[(sourceY + row) * stride + sourceX + column];
          if (!(value & 0x8000)) {
            continue;
          }

          red = (value >> 7) & 0xF8;
          green = (value >> 2) & 0xF8;
          blue = (value & 0x1F) << 3;
          this->ApplyPixelFilter(filter, &red, &green, &blue);

          if (!&pixels[(y + (row + 1) * scaleY - 1) * width + x + column * scaleX + scaleX - 1]) {
            continue;
          }

          for (int dy = 0; dy < scaleY; ++dy) {
            for (int dx = 0; dx < scaleX; ++dx) {
              const int destinationX = x + column * scaleX + dx;
              const int destinationY = y + row * scaleY + dy;
              if (destinationX >= 0 && destinationY >= 0 && this->m_bitmap.width > static_cast<unsigned>(destinationX) &&
                  this->m_bitmap.height > static_cast<unsigned>(destinationY)) {
                pixels[destinationX + destinationY * width] = PackPixel(0xFF, red, green, blue);
              }
            }
          }
        }
      }
    }
  }

  source->Unlock();
}

// INLINED
static __forceinline void BlendScaledPixel(
  std::uint32_t& pixel, std::uint32_t red, std::uint32_t green, std::uint32_t blue, float inverse, float redScale, float greenScale, float blueScale
) {
  const BYTE r = static_cast<BYTE>(static_cast<std::uint32_t>(static_cast<float>((pixel >> 16) & 255) * inverse + static_cast<float>(red) * redScale));
  const BYTE g = static_cast<BYTE>(static_cast<std::uint32_t>(static_cast<float>((pixel >> 8) & 255) * inverse + static_cast<float>(green) * greenScale));
  const BYTE b = static_cast<BYTE>(static_cast<std::uint32_t>(static_cast<float>(pixel & 255) * inverse + static_cast<float>(blue) * blueScale));
  pixel = 0xFF000000 + (r << 16) + (g << 8) + b;
}

// OFFSET: 0x005469E0
// FUNCTION: fn_01840
void CDCBitmap::DrawScaledSurface(
  int x, int y, int drawWidth, int drawHeight, CSurface* source, int sourceWidth, int sourceHeight, float scaleX, float scaleY, std::uint32_t color,
  bool mirrored, int offsetX, int offsetY, int sourceX, int sourceY
) {
  if (!source || x < 0 || this->m_bitmap.width < static_cast<unsigned>(x) || y < 0 || this->m_bitmap.height < static_cast<unsigned>(y) || drawWidth < 0 || drawHeight < 0 ||
      this->m_bitmap.width < static_cast<unsigned>(x) + drawWidth || this->m_bitmap.height < static_cast<unsigned>(y) + drawHeight || scaleX <= 0.0f || scaleY <= 0.0f ||
      static_cast<float>(color) <= 0.0f) {
    return;
  }

  const int format = static_cast<CTexture*>(source)->Format();
  SurfaceLock locked = source->Lock();
  if (!locked.success) {
    return;
  }

  const std::uint16_t* input = static_cast<const std::uint16_t*>(locked.pixels);
  const int stride = locked.pitch / 2;
  const std::uint16_t* sourcePixels = input + sourceY * stride + sourceX;
  std::uint32_t* destinationPixels = this->m_bitmap.pixels + this->m_bitmap.width * y + x;
  const float alpha = static_cast<float>(color >> 24) / 255.0f;
  const float redScale = static_cast<float>((color >> 16) & 0xFF) * alpha / 255.0f;
  const float greenScale = static_cast<float>((color >> 8) & 0xFF) * alpha / 255.0f;
  const float blueScale = static_cast<float>(color & 0xFF) * alpha / 255.0f;
  const float inverse = 1.0f - alpha;
  const float inverseX = 1.0f / scaleX;
  const float inverseY = 1.0f / scaleY;
  const int scaledWidth = static_cast<int>(static_cast<float>(sourceWidth) * scaleX);
  const int scaledHeight = static_cast<int>(static_cast<float>(sourceHeight) * scaleY);
  const int limit = sourceHeight * stride;

  if (format == TextureFormat::A4R4G4B4) {
    if (!mirrored) {
      for (int row = 0; row < scaledHeight; ++row) {
        for (int column = 0; column < scaledWidth; ++column) {
          if (offsetX + column < 0 || drawWidth < offsetX + column) {
            continue;
          }

          if (offsetY + row < 0 || drawHeight < offsetY + row) {
            continue;
          }

          const int index = static_cast<int>(static_cast<float>(row) * inverseY) * stride + static_cast<int>(static_cast<float>(column) * inverseX);
          if (index < 0 || limit <= index) {
            continue;
          }

          const unsigned value = sourcePixels[index];
          if (!(value & 0xF000)) {
            continue;
          }

          std::uint32_t& destination = destinationPixels[this->m_bitmap.width * (offsetY + row) + column + offsetX];
          const std::uint32_t pixel = destination;
          const unsigned red =
            static_cast<unsigned>(static_cast<float>((pixel >> 16) & 0xFF) * inverse + static_cast<float>((value >> 4) & 0xF0) * redScale);
          const unsigned green = static_cast<unsigned>(static_cast<float>((pixel >> 8) & 0xFF) * inverse + static_cast<float>(value & 0xF0) * greenScale);
          const unsigned blue =
            static_cast<unsigned>(static_cast<float>((value & 0x0F) << 4) * blueScale + static_cast<float>(pixel & 0xFF) * inverse);
          destination = PackPixel(0xFF, red, green, blue);
        }
      }
    } else {
      for (int row = 0; row < scaledHeight; ++row) {
        for (int column = 0; column < scaledWidth; ++column) {
          if (offsetX + column < 0 || drawWidth < offsetX + column) {
            continue;
          }

          if (offsetY + row < 0 || drawHeight < offsetY + row) {
            continue;
          }

          const int index = sourceWidth - 1 + static_cast<int>(static_cast<float>(row) * inverseY) * stride -
                            static_cast<int>(static_cast<float>(column) * inverseX);
          if (index < 0 || limit <= index) {
            continue;
          }

          const unsigned value = sourcePixels[index];
          if (!(value & 0xF000)) {
            continue;
          }

          std::uint32_t& destination = destinationPixels[this->m_bitmap.width * (offsetY + row) + column + offsetX];
          const std::uint32_t pixel = destination;
          const unsigned red =
            static_cast<unsigned>(static_cast<float>((pixel >> 16) & 0xFF) * inverse + static_cast<float>((value >> 4) & 0xF0) * redScale);
          const unsigned green = static_cast<unsigned>(static_cast<float>((pixel >> 8) & 0xFF) * inverse + static_cast<float>(value & 0xF0) * greenScale);
          const unsigned blue =
            static_cast<unsigned>(static_cast<float>((value & 0x0F) << 4) * blueScale + static_cast<float>(pixel & 0xFF) * inverse);
          destination = PackPixel(0xFF, red, green, blue);
        }
      }
    }
  } else if (format == TextureFormat::A1R5G5B5) {
    if (!mirrored) {
      for (int row = 0; row < scaledHeight; ++row) {
        for (int column = 0; column < scaledWidth; ++column) {
          if (offsetX + column < 0 || drawWidth < offsetX + column) {
            continue;
          }

          if (offsetY + row < 0 || drawHeight < offsetY + row) {
            continue;
          }

          const int index = static_cast<int>(static_cast<float>(row) * inverseY) * stride + static_cast<int>(static_cast<float>(column) * inverseX);
          if (index < 0 || limit <= index) {
            continue;
          }

          const unsigned value = sourcePixels[index];
          if (!(value & 0x8000)) {
            continue;
          }

          std::uint32_t& destination = destinationPixels[this->m_bitmap.width * (offsetY + row) + column + offsetX];
          const std::uint32_t pixel = destination;
          const unsigned red =
            static_cast<unsigned>(static_cast<float>((pixel >> 16) & 0xFF) * inverse + static_cast<float>((value >> 7) & 0xF8) * redScale);
          const unsigned green = static_cast<unsigned>(static_cast<float>((pixel >> 8) & 0xFF) * inverse + static_cast<float>((value >> 2) & 0xF8) * greenScale);
          const unsigned blue =
            static_cast<unsigned>(static_cast<float>((value & 0x1F) << 3) * blueScale + static_cast<float>(pixel & 0xFF) * inverse);
          destination = PackPixel(0xFF, red, green, blue);
        }
      }
    } else {
      for (int row = 0; row < scaledHeight; ++row) {
        for (int column = 0; column < scaledWidth; ++column) {
          if (offsetX + column < 0 || drawWidth < offsetX + column) {
            continue;
          }

          if (offsetY + row < 0 || drawHeight < offsetY + row) {
            continue;
          }

          const int index = sourceWidth - 1 + static_cast<int>(static_cast<float>(row) * inverseY) * stride -
                            static_cast<int>(static_cast<float>(column) * inverseX);
          if (index < 0 || limit <= index) {
            continue;
          }

          const unsigned value = sourcePixels[index];
          if (!(value & 0x8000)) {
            continue;
          }

          std::uint32_t& destination = destinationPixels[this->m_bitmap.width * (offsetY + row) + column + offsetX];
          const std::uint32_t pixel = destination;
          const unsigned red =
            static_cast<unsigned>(static_cast<float>((pixel >> 16) & 0xFF) * inverse + static_cast<float>((value >> 7) & 0xF8) * redScale);
          const unsigned green = static_cast<unsigned>(static_cast<float>((pixel >> 8) & 0xFF) * inverse + static_cast<float>((value >> 2) & 0xF8) * greenScale);
          const unsigned blue =
            static_cast<unsigned>(static_cast<float>((value & 0x1F) << 3) * blueScale + static_cast<float>(pixel & 0xFF) * inverse);
          destination = PackPixel(0xFF, red, green, blue);
        }
      }
    }
  }

  source->Unlock();
}

// OFFSET: 0x00545E20
// FUNCTION: fn_01838
void CDCBitmap::DrawRleSprite(int x, int y, const CSprRes& sprite, const ActionFrame& frame, const std::uint32_t* palette) {
  for (int cellIndex = 0; cellIndex < frame.cellCount; ++cellIndex) {
    const SpriteFrameCell& cell = frame.Cell(cellIndex);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite.images[cell.imageType][cell.image];
    const int left = x - image->width / 2;
    const int top = y - image->height / 2;
    const int destinationX = std::max(left, 0);
    const int copyWidth = static_cast<int>(std::min(this->m_bitmap.width, static_cast<std::uint32_t>(left + image->width))) - destinationX;
    const int destinationY = std::max(top, 0);
    int copyHeight = static_cast<int>(std::min(this->m_bitmap.height, static_cast<std::uint32_t>(top + image->height))) - destinationY;
    const int rowSkip = image->width - copyWidth;
    const int skip = std::max(-top, 0) * image->width + std::max(-left, 0);
    std::uint32_t* row = this->m_bitmap.pixels + destinationY * this->m_bitmap.width + destinationX;
    const std::uint8_t* runs = image->runs;
    int remaining = 0;
    auto next = [&]() -> unsigned {
      if (remaining) {
        --remaining;
        return 0;
      }

      unsigned value = *runs++;
      if (!value) {
        remaining = *runs++ - 1;
      }

      return value;
    };

    for (int index = 0; index < skip; ++index) {
      next();
    }

    if (!cell.mirrored) {
      for (; copyHeight > 0; --copyHeight) {
        for (int column = 0; column < copyWidth; ++column) {
          const unsigned index = next();
          if (index) {
            const std::uint32_t color = palette[index];
            row[column] = ((color & 0x1F) << 3) + ((color & 0x3E0) << 6) + ((color & 0x7C00) << 9) - 0x01000000;
          }
        }

        for (int index = 0; index < rowSkip; ++index) {
          next();
        }

        row += this->m_bitmap.width;
      }
    } else {
      for (; copyHeight > 0; --copyHeight) {
        std::uint32_t* pixel = row + copyWidth - 1;
        for (int column = copyWidth; column > 0; --column, --pixel) {
          const unsigned index = next();
          if (index) {
            const std::uint32_t color = palette[index];
            *pixel = ((color & 0x1F) << 3) + ((color & 0x3E0) << 6) + ((color & 0x7C00) << 9) - 0x01000000;
          }
        }

        for (int index = 0; index < rowSkip; ++index) {
          next();
        }

        row += this->m_bitmap.width;
      }
    }
  }
}

// OFFSET: 0x00545C40
// FUNCTION: fn_01837
void CDCBitmap::DrawScaledSprite(
  int x, int y, int drawWidth, int drawHeight, const CSprRes* sprite, const ActionFrame* frame, int offsetX, int offsetY, float scale,
  const std::uint32_t* palette
) {
  if (!sprite || !frame) {
    return;
  }

  if (!palette) {
    palette = sprite->palette;
  }

  SpriteAtlasRecord record{};

  for (int index = 0; index < frame->cellCount; ++index) {
    const SpriteFrameCell* cell = &frame->Cell(index);
    if (!cell || cell->image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell->imageType][cell->image];
    if (!image) {
      continue;
    }

    const float scaleX = scale * cell->scaleX;
    const float scaleY = scale * cell->scaleY;
    record.u0 = 0;
    record.v0 = 0;
    CSurface* surface;

    if (!cell->imageType) {
      surface = g_renderer->atlas.Find(image, palette, record);
      if (!surface) {
        surface = g_renderer->atlas.Insert(image, palette, record);
      }
    } else {
      surface = static_cast<CSurface*>(const_cast<void*>(image->surface));
    }

    if (!surface) {
      continue;
    }

    const std::uint8_t* channels = reinterpret_cast<const std::uint8_t*>(&cell->color);
    const std::uint32_t color = PackPixel(channels[3], channels[0], channels[1], channels[2]);
    this->DrawScaledSurface(
      x, y, drawWidth, drawHeight, surface, image->width, image->height, scaleX, scaleY, color, cell->mirrored != 0,
      static_cast<int>(static_cast<float>(cell->x) * scaleX + static_cast<float>(offsetX)),
      static_cast<int>(static_cast<float>(cell->y) * scaleY + static_cast<float>(offsetY)), static_cast<int>(record.u0 * 256), static_cast<int>(record.v0 * 256)
    );
  }
}

// OFFSET: 0x00547D80
// FUNCTION: fn_01851
void CDCBitmap::ScaleCopy(int x, int y, int sourceWidth, int sourceHeight, int drawWidth, int drawHeight, const std::uint32_t* source, int) {
  if (!this->m_bitmap.pixels || !source) {
    return;
  }

  this->m_bitmap.dirty = true;
  const float scaleX = static_cast<float>(drawWidth) / static_cast<float>(sourceWidth);
  const float scaleY = static_cast<float>(drawHeight) / static_cast<float>(sourceHeight);
  std::vector<std::uint32_t> scratch;
  const int columns = scaleX < 1 ? sourceWidth : drawWidth;
  const int rows = scaleY < 1 ? sourceHeight : drawHeight;
  scratch.resize(static_cast<std::uint32_t>(columns) * rows);

  if (scaleX == 1) {
    for (int row = 0; row < sourceHeight; ++row) {
      for (int column = 0; column < sourceWidth; ++column) {
        scratch[row * sourceWidth + column] = source[row * sourceWidth + column];
      }
    }
  } else if (scaleX < 1) {
    int output = 0, previous = 0;

    for (int row = 0; row < sourceHeight; ++row) {
      for (int column = 0; column < sourceWidth; ++column) {
        int next = static_cast<int>(static_cast<float>(column) * scaleX);
        if (output != next) {
          scratch[row * drawWidth + output] = source[row * sourceWidth + previous];
          if (column == sourceWidth - 1 && output != drawWidth - 1) {
            scratch[(row + 1) * drawWidth - 1] = source[row * sourceWidth + previous];
          }

          previous = column;
          output = next;
        }
      }
    }
  } else {
    int output = 0;

    for (int row = 0; row < sourceHeight; ++row) {
      for (int column = 1; column <= sourceWidth; ++column) {
        int next = static_cast<int>(static_cast<float>(column) * scaleX);

        for (int index = output; index < next; ++index) {
          scratch[row * drawWidth + index] = source[row * sourceWidth + column - 1];
        }

        output = next;
      }
    }
  }

  if (scaleY < 1) {
    int output = 0, previous = 0;

    for (int column = 0; column < drawWidth; ++column) {
      if (static_cast<unsigned>(x + column) >= this->m_bitmap.width) {
        continue;
      }

      int sample = previous * drawWidth;

      for (int row = 0; row < sourceHeight; ++row) {
        const int next = static_cast<int>(static_cast<float>(row) * scaleY);
        if (output != next && static_cast<unsigned>(y + next) <= this->m_bitmap.height) {
          previous = row;
          this->m_bitmap.pixels[x + column + (y + output) * this->m_bitmap.width] = scratch[sample + column];
          output = next;

          sample = row * drawWidth;
        }
      }
    }
  } else {
    int output = 0;

    for (int column = 0; column < drawWidth; ++column) {
      if (static_cast<unsigned>(x + column) >= this->m_bitmap.width) {
        continue;
      }

      for (int row = 1; row <= sourceHeight; ++row) {
        int next = static_cast<int>(static_cast<float>(row) * scaleY);

        for (int index = output; index < next; ++index) {
          if (static_cast<unsigned>(y + index) >= this->m_bitmap.height) {
            continue;
          }

          this->m_bitmap.pixels[x + column + (y + index) * this->m_bitmap.width] = scratch[(row - 1) * drawWidth + column];
        }

        output = next;
      }
    }
  }
}

// OFFSET: 0x00547670
// FUNCTION: fn_01842
void CDCBitmap::ApplyPixelFilter(int filter, unsigned int* red, unsigned int* green, unsigned int* blue) {
  if (!filter) {
    return;
  }

  unsigned int average = (*red + *green + *blue) / 3;
  const unsigned int threshold = filter != 1 ? 100u : 120u;

  if (average < threshold) {
    average = threshold;
  }

  *red = average;
  *green = average;
  *blue = average;

  if (filter == 2) {
    *red = 200;
  } else if (filter == 3) {
    *green = 200;
  } else if (filter == 4) {
    *blue = 200;
  }
}
