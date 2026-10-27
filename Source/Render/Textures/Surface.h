#ifndef RAGNAROK_SOURCE_RENDER_TEXTURES_SURFACE_H
#define RAGNAROK_SOURCE_RENDER_TEXTURES_SURFACE_H

#include <cstddef>
#include <cstdint>
#include <windows.h>

struct SurfaceLock {
  /* 0000 */ bool success;
  /* 0004 */ void* pixels;
  /* 0008 */ int pitch;
  /* 000C */ std::uint32_t width;
  /* 0010 */ std::uint32_t height;
  /* 0014 */ int bitsPerPixel;
};

class CSurface {
public:
  virtual ~CSurface();
  virtual std::uint32_t Width() = 0;
  virtual std::uint32_t Height() = 0;
  virtual bool Resize(int width, int height) = 0;
  virtual SurfaceLock Lock() = 0;
  virtual void Unlock() = 0;
  virtual bool GetDC(HDC* output) = 0;
  virtual void ReleaseDC(HDC dc) = 0;
  virtual void Upload(int x, int y, int width, int height, const std::uint32_t* pixels, int option, int sourceWidth) = 0;
  virtual void Fill(const RECT* rectangle, std::uint32_t color) = 0;
  virtual void Draw(int x, int y, int width, int height, std::uint32_t color) = 0;
  virtual void Stretch(int x, int y, int width, int height) = 0;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSurface) == 4);
static_assert(sizeof(SurfaceLock) == 24);
static_assert(offsetof(SurfaceLock, pixels) == 0x04);
static_assert(offsetof(SurfaceLock, pitch) == 0x08);
static_assert(offsetof(SurfaceLock, width) == 0x0C);
static_assert(offsetof(SurfaceLock, height) == 0x10);
static_assert(offsetof(SurfaceLock, bitsPerPixel) == 0x14);
#endif

#endif
