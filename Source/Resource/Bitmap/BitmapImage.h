#ifndef RAGNAROK_SOURCE_RESOURCE_BITMAP_BITMAPIMAGE_H
#define RAGNAROK_SOURCE_RESOURCE_BITMAP_BITMAPIMAGE_H

#include <cstdint>

class IJL15Runtime;

struct BitmapImage {
  /* 0110 */ int alpha;
  /* 0114 */ int width;
  /* 0118 */ int height;
  /* 011C */ std::uint32_t* pixels;
  /* 0120 */ void* auxiliary;

  BitmapImage();
  void Clear();
  bool DecodeBitmap(const void* data, int bytes, bool iconOnly);
  bool DecodeTga(const void* data, int bytes);
  bool DecodePng(const void* data, int bytes, const char* name);
  bool DecodeJpeg(const void* data, int bytes, IJL15Runtime& runtime);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BitmapImage) == 20);
#endif

#endif
