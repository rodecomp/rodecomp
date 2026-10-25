#ifndef RAGNAROK_SOURCE_RENDER_DX7_SURFACEDX7_H
#define RAGNAROK_SOURCE_RENDER_DX7_SURFACEDX7_H

#include "Render/Textures/Surface.h"

#include <ddraw.h>

class DeviceOwnerDX7;

class CSurfaceDX7 : public CSurface {
  friend class CSurfaceWallpaperDX7;
  friend class DeviceOwnerDX7;

public:
  CSurfaceDX7(DeviceOwnerDX7& owner, int width, int height);
  CSurfaceDX7(DeviceOwnerDX7& owner, IDirectDrawSurface7* surface);
  ~CSurfaceDX7() override;
  std::uint32_t Width() override;
  std::uint32_t Height() override;
  bool Resize(int width, int height) override;
  SurfaceLock Lock() override;
  void Unlock() override;
  bool GetDC(HDC* output) override;
  void ReleaseDC(HDC dc) override;
  void Upload(int x, int y, int width, int height, const std::uint32_t* pixels, int option, int sourceWidth) override;
  void Fill(const RECT* rectangle, std::uint32_t color) override;
  void Draw(int x, int y, int width, int height, std::uint32_t color) override;
  void Stretch(int x, int y, int width, int height) override;

protected:
  DeviceOwnerDX7* m_owner;
  IDirectDrawSurface7* m_surface;
  std::uint32_t m_width;
  std::uint32_t m_height;
};

class CSurfaceWallpaperDX7 : public CSurfaceDX7 {
public:
  CSurfaceWallpaperDX7(DeviceOwnerDX7& owner, int width, int height);
  void Upload(int x, int y, int width, int height, const std::uint32_t* pixels, int option, int sourceWidth) override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSurfaceDX7) == 20);
#endif

#endif
