#include "Render/DeviceContext/DCBitmap.h"

// OFFSET: 0x00545790
// FUNCTION: fn_01832
CDCBitmap::CDCBitmap(int width, int height) : m_bitmap(width, height) {
}

CDCBitmap::~CDCBitmap() = default;

// OFFSET: 0x00569E90
// FUNCTION: fn_02273
CDCBitmap* CreateBitmapDC(int width, int height, bool fill) {
  CDCBitmap* bitmap = new CDCBitmap(width, height);
  if (fill) {
    bitmap->Fill(nullptr, 0xFFFF00FFu);
  }

  return bitmap;
}

// INLINED
void CDCBitmap::UploadTexture(CRenderSystemBase& system, CTexture*& texture) {
  this->m_bitmap.UploadTexture(system, texture);
}

void CDCBitmap::Grow(std::uint32_t width, std::uint32_t height) {
  if (width > this->m_bitmap.width || height > this->m_bitmap.height) {
    this->m_cache.Clear();
    this->m_bitmap.Grow(width, height);
  }
}

bool CDCBitmap::GetDC(HDC* output) {
  return this->m_bitmap.GetDC(output);
}

void CDCBitmap::ReleaseDC(HDC dc) {
  this->m_bitmap.ReleaseDC(dc);
}

void CDCBitmap::Copy(int x, int y, int width, int height, const std::uint32_t* source, bool transparent) {
  this->m_bitmap.Copy(x, y, width, height, source, transparent);
}


// OFFSET: 0x005482D0
// FUNCTION: fn_01853
void CDCBitmap::SetAlpha(unsigned char maxLuminance) {
  if (this->m_bitmap.height <= 0) {
    return;
  }

  for (std::uint32_t y = 0; y < this->m_bitmap.height; ++y) {
    for (std::uint32_t x = 0; x < this->m_bitmap.width; ++x) {
      const std::uint32_t index = y * this->m_bitmap.width + x;
      std::uint32_t pixel = this->m_bitmap.pixels[index];
      const unsigned char r = pixel & 0xFF;
      const unsigned char g = (pixel >> 8) & 0xFF;
      const unsigned char b = (pixel >> 16) & 0xFF;
      const unsigned char avg = static_cast<unsigned char>((r + g + b) / 3);
      const unsigned char alpha = static_cast<unsigned char>((static_cast<float>(avg) / static_cast<float>(maxLuminance)) * 255.0f);
      reinterpret_cast<unsigned char*>(&pixel)[3] = alpha;
      this->m_bitmap.pixels[index] = pixel;
    }
  }
}
void CDCBitmap::ScaleCopy(int x, int y, int sourceWidth, int sourceHeight, int width, int height, const std::uint32_t* source, int option) {
  this->m_bitmap.ScaleCopy(x, y, sourceWidth, sourceHeight, width, height, source, option);
}

void CDCBitmap::CopyRegion(
  int x, int y, int sourceLeft, int sourceTop, int sourceRight, int sourceBottom, const std::uint32_t* source, int sourceWidth, int sourceHeight,
  bool transparent, bool alpha
) {
  this->m_bitmap.CopyRegion(x, y, sourceLeft, sourceTop, sourceRight, sourceBottom, source, sourceWidth, sourceHeight, transparent, alpha);
}

void CDCBitmap::CopyFrom(int x, int y, int sourceX, int sourceY, int width, int height, const CDCBitmap& source) {
  this->m_bitmap.CopyFrom(x, y, sourceX, sourceY, width, height, source.m_bitmap);
}

void CDCBitmap::BlendFrom(std::uint32_t x, std::uint32_t y, int width, int height, const CDCBitmap& source) {
  this->m_bitmap.BlendFrom(x, y, width, height, source.m_bitmap, source.m_cache.m_alpha);
}

void CDCBitmap::DrawRleSprite(
  int x, int y, std::span<const SpriteFrameCell> cells, const SpriteRleImage* const* const* images, const std::uint32_t* palette, int cellCount
) {
  this->m_bitmap.DrawRleSprite(x, y, cells, images, palette, cellCount);
}

void CDCBitmap::Fill(const RECT* rectangle, std::uint32_t color) {
  this->m_bitmap.Fill(rectangle, color);
}

void CDCBitmap::Draw(CRenderSystemBase& system, int x, int y, int width, int height, std::uint32_t color) {
  this->m_cache.Draw(this->m_bitmap, system, x, y, width, height, color);
}

void CDCBitmap::DrawScaledSprite(
  int x, int y, int width, int height, std::span<const SpriteFrameCell> cells, const SpriteRleImage* const* const* images, int cellCount, int offsetX,
  int offsetY, float scale, const std::uint32_t* palette, SpriteAtlas& atlas, CRenderSystemBase& system, int serviceType, std::uint32_t reuseDelay
) {
  this->m_bitmap.DrawScaledSprite(x, y, width, height, cells, images, cellCount, offsetX, offsetY, scale, palette, atlas, system, serviceType, reuseDelay);
}

void CDCBitmap::DrawScaledSurface(
  int x, int y, int width, int height, CSurface* source, int sourceWidth, int sourceHeight, float scaleX, float scaleY, std::uint32_t color, bool mirrored,
  int offsetX, int offsetY, int sourceX, int sourceY
) {
  this->m_bitmap.DrawScaledSurface(x, y, width, height, source, sourceWidth, sourceHeight, scaleX, scaleY, color, mirrored, offsetX, offsetY, sourceX, sourceY);
}

void CDCBitmap::Brighten(int x, int y, int width, int height, int amount) {
  this->m_bitmap.Brighten(x, y, width, height, amount);
}

void CDCBitmap::Dim(int x, int y, int width, int height, int percent) {
  this->m_bitmap.Dim(x, y, width, height, percent);
}
