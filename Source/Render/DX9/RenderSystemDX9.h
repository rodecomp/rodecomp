#ifndef RAGNAROK_SOURCE_RENDER_DX9_RENDERSYSTEMDX9_H
#define RAGNAROK_SOURCE_RENDER_DX9_RENDERSYSTEMDX9_H

#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/Device/RenderAdapter.h"
#include "Render/RenderSystemBase.h"
#include "Render/DX9/RendererDX9.h"

class CRenderSystemDX9 : public CRenderSystemBase {
public:
  CRenderSystemDX9();
  ~CRenderSystemDX9() override;
  std::int32_t Backend() override;
  HRESULT Initialize(HWND window) override;
  HRESULT Close() override;
  bool IsWindowed() override;
  std::uint32_t Width() override;
  std::uint32_t Height() override;
  CRenderer* CreateRenderer() override;
  void DestroyRenderer(CRenderer* renderer) override;
  CSurface* CreateSurface(std::uint32_t width, std::uint32_t height) override;
  CTexture* CreateTexture(std::uint32_t width, std::uint32_t height, std::int32_t format, bool dynamic, bool renderTarget) override;
  CSurface* CreateSurfaceOrTexture(std::uint32_t width, std::uint32_t height) override;
  CTexture* BackBufferTexture() override;
  std::shared_ptr<CSurface> BackBufferSurface() override;
  void UpdateOption() override;
  void PrepareErrorDialog() override;
  void RestoreDisplayMode() override;
  HRESULT TestDevice() override;
  HRESULT Present(bool option) override;

  // INLINED
  DeviceOwnerDX9& Device() {
    return *this->m_device;
  }

private:
  /* 0004 */ DeviceOwnerDX9* m_device;
  /* 0008 */ std::unique_ptr<CTextureDX9> m_defaultTexture;
  /* 000C */ std::shared_ptr<CSurface> m_backBuffer;

};

#endif
