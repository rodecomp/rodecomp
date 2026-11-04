#include "Render/DeviceContext/DCBitmap.h"
#include "Render/Textures/BitmapPixels.h"
#include "Render/RenderSystemBase.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Surface.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

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
void BitmapPixels::UploadTexture(CRenderSystemBase& system, CTexture*& texture) {
  const std::int32_t bytes = 4 * static_cast<int>(this->width) * static_cast<int>(this->height);

  for (std::int32_t offset = 0; offset < bytes; offset += 4) {
    reinterpret_cast<unsigned char*>(this->pixels)[offset + 3] = 255;
  }

  if (texture) {
    texture->Upload(0, 0, this->width, this->height, this->pixels, 0, 0);
  } else {
    CTexture* created = system.CreateTexture(this->width, this->height, TextureFormat::A1R5G5B5, false, false);
    if (created) {
      created->Upload(0, 0, this->width, this->height, this->pixels, 0, 0);
    }

    texture = created;
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
HGDIOBJ CDCBitmap::Create(int width, int height) {
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
void BitmapPixels::Grow(std::uint32_t newWidth, std::uint32_t newHeight) {
  if (newWidth > this->width || newHeight > this->height) {
    SelectObject(this->dc, this->previous);
    DeleteObject(this->bitmap);
    this->Create(newWidth, newHeight);
  }
}

// OFFSET: 0x00547D10
// FUNCTION: fn_01848
bool BitmapPixels::GetDC(HDC* output) {
  this->dirty = true;
  *output = this->dc;
  return true;
}

// OFFSET: 0x00547D30
// FUNCTION: fn_01849
void BitmapPixels::ReleaseDC(HDC) {
}

// OFFSET: 0x00547460
// FUNCTION: fn_01841
void BitmapPixels::Fill(const RECT* rectangle, std::uint32_t color) {
  if (!this->pixels) {
    return;
  }

  this->dirty = true;
  const std::uint32_t converted = ((color >> 16) & 0xFF) + ((color & 0xFF) << 16) + (color & 0xFF000000) + (color & 0xFF00);

  if (rectangle) {
    for (int y = rectangle->top; y < rectangle->bottom; ++y) {
      for (int x = rectangle->left; x < rectangle->right; ++x) {
        this->pixels[x + y * this->width] = converted;
      }
    }
  } else {
    for (std::uint32_t y = 0; y < this->height; ++y) {
      for (std::uint32_t x = 0; x < this->width; ++x) {
        this->pixels[x + y * this->width] = converted;
      }
    }
  }
}

// OFFSET: 0x00548230
// FUNCTION: fn_01852
void BitmapPixels::Copy(int x, int y, int copyWidth, int copyHeight, const std::uint32_t* source, bool transparent) {
  if (!this->pixels || !source) {
    return;
  }

  this->dirty = true;
  const std::int32_t count = static_cast<std::int32_t>(this->width * this->height);

  for (int row = 0; row < copyHeight; ++row) {
    for (int column = 0; column < copyWidth; ++column) {
      std::uint32_t value = *source++;
      if (!transparent || value != 0xFFFF00FF) {
        const std::uint32_t index = static_cast<std::uint32_t>(x) + column + this->width * (static_cast<std::uint32_t>(y) + row);
        if (static_cast<std::int32_t>(index) > -1 && count > static_cast<std::int32_t>(index)) {
          this->pixels[index] = value;
        }
      }
    }
  }
}

// OFFSET: 0x00547790
// FUNCTION: fn_01843
void BitmapPixels::CopyFrom(int x, int y, int sourceX, int sourceY, int copyWidth, int copyHeight, const BitmapPixels& source) {
  if (!this->pixels) {
    return;
  }

  this->dirty = true;
  std::uint32_t* destination = this->pixels + x + y * this->width;
  const std::uint32_t* input = source.pixels + sourceX + sourceY * source.width;

  for (int row = 0; row < copyHeight; ++row) {
    if (!destination || !input) {
      break;
    }

    for (int column = 0; column < copyWidth; ++column) {
      if (input[column] != 0xFFFF00FF) {
        destination[column] = input[column];
      }
    }

    destination += this->width;
    input += source.width;
  }
}

// OFFSET: 0x00547840
// FUNCTION: fn_01844
void BitmapPixels::BlendFrom(std::uint32_t x, std::uint32_t y, int copyWidth, int copyHeight, const BitmapPixels& source, bool alpha) {
  if (!this->pixels || x >= this->width || y >= this->height) {
    return;
  }

  if (x + copyWidth > this->width) {
    copyWidth = this->width - x;
  }

  if (y + copyHeight > this->height) {
    copyHeight = this->height - y;
  }

  this->dirty = true;

  for (int row = 0; row < copyHeight; ++row) {
    for (int column = 0; column < copyWidth; ++column) {
      std::uint32_t& destination = this->pixels[x + column + this->width * (y + row)];
      const std::uint32_t input = source.pixels[column + source.width * row];
      if (alpha) {
        const unsigned a = input >> 24;
        std::uint32_t output = std::max(a, destination >> 24) << 24;

        for (unsigned shift : {0u, 8u, 16u}) {
          output |= (a * ((input >> shift) & 255) / 255 + (255 - a) * ((destination >> shift) & 255) / 255) << shift;
        }

        destination = output;
      } else if (input != 0xFFFF00FF) {
        destination = input - 0x01000000;
      }
    }
  }
}

// OFFSET: 0x005489F0
// FUNCTION: fn_01857
void BitmapPixels::Brighten(int x, int y, int copyWidth, int copyHeight, int amount) {
  if (x < 0 || y < 0 || this->width < static_cast<unsigned>(x) || this->height < static_cast<unsigned>(y) || copyWidth < 1 || copyHeight < 1) {
    return;
  }

  this->dirty = true;
  const unsigned right = std::min(this->width, static_cast<unsigned>(x) + copyWidth);
  const unsigned bottom = std::min(this->height, static_cast<unsigned>(y) + copyHeight);

  for (unsigned row = y; row < bottom; ++row) {
    for (unsigned column = x; column < right; ++column) {
      std::uint32_t& pixel = this->pixels[column + row * this->width];
      if (pixel != 0xFFFF00FF) {
        std::uint32_t value = 0xFF000000;

        for (unsigned shift : {0u, 8u, 16u}) {
          value |= std::clamp(amount + static_cast<int>((pixel >> shift) & 255), 0, 255) << shift;
        }

        pixel = value;
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

  const unsigned right = std::min(this->width, static_cast<unsigned>(x) + copyWidth);
  const unsigned bottom = std::min(this->height, static_cast<unsigned>(y) + copyHeight);
  const float factor = static_cast<float>(100 - percent) * 0.01f;

  for (unsigned row = y; row < bottom; ++row) {
    for (unsigned column = x; column < right; ++column) {
      std::uint32_t& pixel = this->pixels[column + row * this->width];
      if (pixel != 0xFFFF00FF) {
        std::uint32_t value = 0xFF000000;

        for (unsigned shift : {0u, 8u, 16u}) {
          value |= static_cast<unsigned char>(static_cast<int>(static_cast<float>((pixel >> shift) & 255) * factor)) << shift;
        }

        pixel = value;
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
        const unsigned a = input >> 24;
        std::uint32_t output = std::max(a, destination >> 24) << 24;

        for (unsigned shift : {0u, 8u, 16u}) {
          output |= (a * ((input >> shift) & 255) / 255 + (255 - a) * ((destination >> shift) & 255) / 255) << shift;
        }

        destination = output;
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
  int x, int y, CSurface* source, int sourceX, int sourceY, int sourceWidth, int sourceHeight, bool mirrored, int scaleX, int scaleY, int filter, int
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
              if (destinationX >= 0 && destinationY >= 0 && static_cast<unsigned>(destinationX) < this->m_bitmap.width &&
                  static_cast<unsigned>(destinationY) < this->m_bitmap.height) {
                const float inverse = 1.0f - alpha;
                const int index = destinationX + destinationY * width;
                const unsigned outputRed =
                  static_cast<unsigned>(static_cast<float>(static_cast<unsigned char>(pixels[index] >> 16)) * inverse + static_cast<float>(red) * alpha);
                const unsigned outputGreen =
                  static_cast<unsigned>(static_cast<float>(static_cast<unsigned char>(pixels[index] >> 8)) * inverse + static_cast<float>(green) * alpha);
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
              if (destinationX >= 0 && destinationY >= 0 && static_cast<unsigned>(destinationX) < this->m_bitmap.width &&
                  static_cast<unsigned>(destinationY) < this->m_bitmap.height) {
                const float inverse = 1.0f - alpha;
                const int index = destinationX + destinationY * width;
                const unsigned outputRed =
                  static_cast<unsigned>(static_cast<float>(static_cast<unsigned char>(pixels[index] >> 16)) * inverse + static_cast<float>(red) * alpha);
                const unsigned outputGreen =
                  static_cast<unsigned>(static_cast<float>(static_cast<unsigned char>(pixels[index] >> 8)) * inverse + static_cast<float>(green) * alpha);
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
              if (destinationX >= 0 && destinationY >= 0 && static_cast<unsigned>(destinationX) < this->m_bitmap.width &&
                  static_cast<unsigned>(destinationY) < this->m_bitmap.height) {
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
              if (destinationX >= 0 && destinationY >= 0 && static_cast<unsigned>(destinationX) < this->m_bitmap.width &&
                  static_cast<unsigned>(destinationY) < this->m_bitmap.height) {
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
void BitmapPixels::DrawScaledSurface(
  int x, int y, int drawWidth, int drawHeight, CSurface* source, int sourceWidth, int sourceHeight, float scaleX, float scaleY, std::uint32_t color,
  bool mirrored, int offsetX, int offsetY, int sourceX, int sourceY
) {
  if (!source || x < 0 || this->width < static_cast<unsigned>(x) || y < 0 || this->height < static_cast<unsigned>(y) || drawWidth < 0 || drawHeight < 0 ||
      this->width < static_cast<unsigned>(x) + drawWidth || this->height < static_cast<unsigned>(y) + drawHeight || !(scaleX > 0) || !(scaleY > 0) || !color) {
    return;
  }

  const int format = static_cast<CTexture*>(source)->Format();
  SurfaceLock locked = source->Lock();
  if (!locked.success) {
    return;
  }

  const auto* input = static_cast<const unsigned char*>(locked.pixels);
  const int stride = locked.pitch / 2;
  const int start = sourceX + sourceY * stride;
  const int limit = sourceHeight * stride;
  const float alpha = static_cast<float>(color >> 24) / 255;
  const float inverse = 1 - alpha;
  const float redScale = static_cast<float>((color >> 16) & 255) * alpha / 255;
  const float greenScale = static_cast<float>((color >> 8) & 255) * alpha / 255;
  const float blueScale = static_cast<float>(color & 255) * alpha / 255;
  const float inverseX = 1 / scaleX;
  const float inverseY = 1 / scaleY;
  const int scaledWidth = static_cast<int>(static_cast<float>(sourceWidth) * scaleX);
  const int scaledHeight = static_cast<int>(static_cast<float>(sourceHeight) * scaleY);

  for (int row = 0; row < scaledHeight; ++row) {
    for (int column = 0; column < scaledWidth; ++column) {
      const int destinationX = offsetX + column;
      const int destinationY = offsetY + row;

      if (destinationX < 0 || drawWidth < destinationX || destinationY < 0 || drawHeight < destinationY) {
        continue;
      }

      const int sampleX = static_cast<int>(static_cast<float>(column) * inverseX);
      const int sampleY = static_cast<int>(static_cast<float>(row) * inverseY);
      const int index = sampleY * stride + (mirrored ? sourceWidth - sampleX - 1 : sampleX);

      if (index < 0 || index >= limit) {
        continue;
      }

      std::uint16_t value;
      std::memcpy(&value, input + 2 * (start + index), sizeof(value));
      if ((format == TextureFormat::A1R5G5B5 && !(value & 0x8000)) || (format == TextureFormat::A4R4G4B4 && !(value & 0xF000)) ||
          (format != TextureFormat::A1R5G5B5 && format != TextureFormat::A4R4G4B4)) {
        continue;
      }

      const unsigned red = format != TextureFormat::A1R5G5B5 ? (value >> 4) & 0xF0 : (value >> 7) & 0xF8;
      const unsigned green = format != TextureFormat::A1R5G5B5 ? value & 0xF0 : (value >> 2) & 0xF8;
      const unsigned blue = format != TextureFormat::A1R5G5B5 ? (value & 15) << 4 : (value & 31) << 3;
      std::uint32_t& destination = this->pixels[x + destinationX + (y + destinationY) * this->width];
      const auto b = static_cast<unsigned char>(static_cast<unsigned>(static_cast<float>(destination & 255) * inverse + static_cast<float>(blue) * blueScale));
      const auto g =
        static_cast<unsigned char>(static_cast<unsigned>(static_cast<float>((destination >> 8) & 255) * inverse + static_cast<float>(green) * greenScale));
      const auto r =
        static_cast<unsigned char>(static_cast<unsigned>(static_cast<float>((destination >> 16) & 255) * inverse + static_cast<float>(red) * redScale));
      destination = 0xFF000000 | (r << 16) | (g << 8) | b;
    }
  }

  source->Unlock();
}

// OFFSET: 0x00545E20
// FUNCTION: fn_01838
void BitmapPixels::DrawRleSprite(
  int x, int y, std::span<const SpriteFrameCell> cells, const SpriteRleImage* const* const* images, const std::uint32_t* palette, int cellCount
) {
  const SpriteFrameCell missing{0, 0, -1, 0, 0xFFFFFFFF, 1, 1, 0, 0};

  for (int cellIndex = 0; cellIndex < cellCount; ++cellIndex) {
    const SpriteFrameCell& cell = static_cast<unsigned>(cellIndex) < cells.size() ? cells[cellIndex] : missing;
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage& image = *images[cell.imageType][cell.image];
    const int left = x - image.width / 2;
    const int top = y - image.height / 2;
    const int destinationX = std::max(left, 0);
    const int destinationY = std::max(top, 0);
    const int copyWidth = static_cast<int>(std::min(this->width, static_cast<std::uint32_t>(left + image.width))) - destinationX;
    const int copyHeight = static_cast<int>(std::min(this->height, static_cast<std::uint32_t>(top + image.height))) - destinationY;
    const int skip = std::max(-left, 0) + image.width * std::max(-top, 0);
    const std::uint8_t* runs = image.runs;
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

    for (int row = 0; row < copyHeight; ++row) {
      for (int column = 0; column < copyWidth; ++column) {
        const unsigned index = next();
        if (index) {
          const std::uint32_t color = palette[index];
          const unsigned pixelX = destinationX + (cell.mirrored ? copyWidth - column - 1 : column);
          this->pixels[pixelX + (destinationY + row) * this->width] = ((color & 0x1F) << 3) + ((color & 0x3E0) << 6) + ((color & 0x7C00) << 9) - 0x01000000;
        }
      }

      for (int index = 0; index < image.width - copyWidth; ++index) {
        next();
      }
    }
  }
}

// OFFSET: 0x00545C40
// FUNCTION: fn_01837
void BitmapPixels::DrawScaledSprite(
  int x, int y, int drawWidth, int drawHeight, std::span<const SpriteFrameCell> cells, const SpriteRleImage* const* const* images, int cellCount, int offsetX,
  int offsetY, float scale, const std::uint32_t* palette, SpriteAtlas& atlas, CRenderSystemBase& system, int serviceType, std::uint32_t reuseDelay
) {
  if (!images) {
    return;
  }

  const SpriteFrameCell missing{0, 0, -1, 0, 0xFFFFFFFF, 1, 1, 0, 0};
  SpriteAtlasRecord record{};

  for (int index = 0; index < cellCount; ++index) {
    const SpriteFrameCell& cell = static_cast<unsigned>(index) < cells.size() ? cells[index] : missing;
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = images[cell.imageType][cell.image];
    if (!image) {
      continue;
    }

    const float scaleX = scale * cell.scaleX;
    const float scaleY = scale * cell.scaleY;
    CSurface* surface;

    if (!cell.imageType) {
      surface = atlas.Find(image, palette, record);
      if (!surface) {
        surface = atlas.Insert(system, image, palette, record, serviceType, reuseDelay);
      }
    } else {
      surface = static_cast<CSurface*>(const_cast<void*>(image->unknown_0008));
    }

    if (!surface) {
      continue;
    }

    const std::uint32_t color = (cell.color & 0xFF00FF00u) | ((cell.color & 255) << 16) | ((cell.color >> 16) & 255);
    this->DrawScaledSurface(
      x, y, drawWidth, drawHeight, surface, image->width, image->height, scaleX, scaleY, color, cell.mirrored != 0,
      static_cast<int>(static_cast<float>(cell.x) * scaleX + static_cast<float>(offsetX)),
      static_cast<int>(static_cast<float>(cell.y) * scaleY + static_cast<float>(offsetY)), static_cast<int>(record.u0 * 256), static_cast<int>(record.v0 * 256)
    );
  }
}

// OFFSET: 0x00547D80
// FUNCTION: fn_01851
void BitmapPixels::ScaleCopy(int x, int y, int sourceWidth, int sourceHeight, int drawWidth, int drawHeight, const std::uint32_t* source, int) {
  if (!this->pixels || !source) {
    return;
  }

  this->dirty = true;
  const float scaleX = static_cast<float>(drawWidth) / static_cast<float>(sourceWidth);
  const float scaleY = static_cast<float>(drawHeight) / static_cast<float>(sourceHeight);
  const int columns = scaleX < 1 ? sourceWidth : drawWidth;
  const int rows = scaleY < 1 ? sourceHeight : drawHeight;
  std::vector<std::uint32_t> scratch(static_cast<std::uint32_t>(columns) * rows);

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
      if (static_cast<unsigned>(x + column) >= this->width) {
        continue;
      }

      int sample = previous * drawWidth;

      for (int row = 0; row < sourceHeight; ++row) {
        const int next = static_cast<int>(static_cast<float>(row) * scaleY);
        if (output != next && static_cast<unsigned>(y + next) <= this->height) {
          previous = row;
          this->pixels[x + column + (y + output) * this->width] = scratch[sample + column];
          output = next;

          sample = row * drawWidth;
        }
      }
    }
  } else {
    int output = 0;

    for (int column = 0; column < drawWidth; ++column) {
      if (static_cast<unsigned>(x + column) >= this->width) {
        continue;
      }

      for (int row = 1; row <= sourceHeight; ++row) {
        int next = static_cast<int>(static_cast<float>(row) * scaleY);

        for (int index = output; index < next; ++index) {
          if (static_cast<unsigned>(y + index) >= this->height) {
            break;
          }

          this->pixels[x + column + (y + index) * this->width] = scratch[(row - 1) * drawWidth + column];
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
  const unsigned int threshold = filter == 1 ? 120u : 100u;

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
