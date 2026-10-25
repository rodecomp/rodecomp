#ifndef RAGNAROK_SOURCE_RENDER_DX9_SURFACEDX9_H
#define RAGNAROK_SOURCE_RENDER_DX9_SURFACEDX9_H

#include "Render/Textures/Surface.h"

#include <d3d9.h>

struct SurfaceDX9Context {
  IDirect3DDevice9* device;
  D3DSURFACE_DESC backBufferDescription;
  IDirect3DSurface9* backBuffer;
};

class CSurfaceDX9 : public CSurface {
public:
  CSurfaceDX9(SurfaceDX9Context& context, int width, int height);
  CSurfaceDX9(SurfaceDX9Context& context, IDirect3DSurface9* surface);
  ~CSurfaceDX9() override;
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
  /* 0004 */ SurfaceDX9Context* m_context;
  /* 0008 */ std::uint32_t m_width;
  /* 000C */ std::uint32_t m_height;
  /* 0010 */ IDirect3DSurface9* m_surface;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSurfaceDX9) == 20);
#endif

#endif
