#ifndef RAGNAROK_SOURCE_RESOURCE_BITMAP_BITMAPIMAGE_H
#define RAGNAROK_SOURCE_RESOURCE_BITMAP_BITMAPIMAGE_H

#include <cstdint>
#include <cstdlib>


struct BitmapImage {
  /* 0110 */ int alpha;
  /* 0114 */ int width;
  /* 0118 */ int height;
  /* 011C */ std::uint32_t* pixels;
  /* 0120 */ void* auxiliary;

  // INLINED
  BitmapImage() : alpha(0), pixels(nullptr), auxiliary(nullptr) {
  }
  void Clear();
  bool CreateAuxiliary(int mode);
  std::uint32_t Pixel(int x, int y) const;
  void ClearAuxiliary();
  bool DecodeBitmap(const void* data, int bytes, bool iconOnly);
  bool DecodeTga(const void* data, int bytes);
  bool DecodePng(const void* data, int bytes, const char* name);
  bool DecodeJpeg(const void* data, int bytes);
};

// INLINED
inline void BitmapImage::Clear() {
  if (this->pixels) {
    std::free(this->pixels);
  }

  if (this->auxiliary) {
    std::free(this->auxiliary);
  }

  this->width = 0;
  this->height = 0;
  this->pixels = nullptr;
  this->auxiliary = nullptr;
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BitmapImage) == 20);
#endif

#endif
