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

// OFFSET: 0x005482D0
// FUNCTION: fn_01853
void CDCBitmap::SetAlpha(unsigned char maxLuminance) {
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

void CDCBitmap::CopyRegion(
  int x, int y, int sourceLeft, int sourceTop, int sourceRight, int sourceBottom, const std::uint32_t* source, int sourceWidth, int sourceHeight,
  bool transparent, bool alpha
) {
  this->m_bitmap.CopyRegion(x, y, sourceLeft, sourceTop, sourceRight, sourceBottom, source, sourceWidth, sourceHeight, transparent, alpha);
}

void CDCBitmap::Dim(int x, int y, int width, int height, int percent) {
  this->m_bitmap.Dim(x, y, width, height, percent);
}
