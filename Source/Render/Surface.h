#ifndef RAGNAROK_SOURCE_RENDER_SURFACE_H
#define RAGNAROK_SOURCE_RENDER_SURFACE_H

#include <cstdint>
#include <windows.h>

struct SurfaceLock {
  bool success;
  void* pixels;
  int pitch;
  std::uint32_t width;
  std::uint32_t height;
  int bitsPerPixel;
};

// RTTI CSurface. Pure backend contracts, not stand-ins.
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
  virtual void
  Upload(int x, int y, int width, int height, const std::uint32_t* pixels, int option, int sourceWidth) = 0;
  virtual void Fill(const RECT* rectangle, std::uint32_t color) = 0;
  virtual void Draw(int x, int y, int width, int height, std::uint32_t color) = 0;
  virtual void Stretch(int x, int y, int width, int height) = 0;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSurface) == 4);
static_assert(sizeof(SurfaceLock) == 24);
#endif

#endif
