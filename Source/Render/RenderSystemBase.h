#ifndef RAGNAROK_SOURCE_RENDER_RENDERSYSTEMBASE_H
#define RAGNAROK_SOURCE_RENDER_RENDERSYSTEMBASE_H

#include <cstdint>
#include <memory>
#include <windows.h>

class CRenderer;
class CSurface;
class CTexture;

class CRenderSystemBase {
public:
  // OFFSET: 0x0056B4D0
  virtual ~CRenderSystemBase() = default;
  virtual std::int32_t Backend() = 0;
  virtual HRESULT Initialize(HWND window) = 0;
  virtual HRESULT Close() = 0;
  virtual bool IsWindowed() = 0;
  virtual std::uint32_t Width() = 0;
  virtual std::uint32_t Height() = 0;
  virtual CRenderer* CreateRenderer() = 0;
  virtual void DestroySurface(CSurface* surface) = 0;
  virtual CSurface* CreateSurface(std::uint32_t width, std::uint32_t height) = 0;
  virtual CTexture* CreateTexture(std::uint32_t width, std::uint32_t height, std::int32_t format, bool option1, bool option2) = 0;
  virtual CSurface* CreateSurfaceOrTexture(std::uint32_t width, std::uint32_t height) = 0;
  virtual CTexture* BackBufferTexture() = 0;
  virtual std::shared_ptr<CSurface> BackBufferSurface() = 0;
  virtual void UpdateOption() = 0;
  virtual void PrepareErrorDialog() = 0;
  virtual void RestoreDisplayMode() = 0;
  virtual HRESULT TestDevice() = 0;
  virtual HRESULT Present(bool option) = 0;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRenderSystemBase) == 4);
#endif

#endif
