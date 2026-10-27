#ifndef RAGNAROK_SOURCE_RENDER_DX9_RENDERSYSTEMDX9_H
#define RAGNAROK_SOURCE_RENDER_DX9_RENDERSYSTEMDX9_H

#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/RenderAdapter.h"
#include "Render/RenderSystemBase.h"
#include "Render/DX9/RendererDX9.h"

struct RenderSystemDX9Context {
  const DeviceOptionsDX9& deviceOptions;
  const AdapterConfiguration& adapter;
  const bool& serviceUsesDX7;
  D3DX9Runtime& d3dx;
  TextureCache& textures;
  TextureSources& sources;
  EffectRetirement& retirement;
  const ServiceOptions& options;
  const unsigned char& multitexture;
  FogConfiguration& fog;
  const unsigned char& worldFlag;
  const unsigned& overlayColor;
  RenderDiagnostic& diagnostic;
  const float& projectionScale;
  float& heightScale;
  float& tangent;
  float& cotangent;
};

class CRenderSystemDX9 : public CRenderSystemBase {
public:
  explicit CRenderSystemDX9(RenderSystemDX9Context& context);
  ~CRenderSystemDX9() override;
  std::int32_t Backend() override;
  HRESULT Initialize(HWND window) override;
  HRESULT Close() override;
  bool IsWindowed() override;
  std::uint32_t Width() override;
  std::uint32_t Height() override;
  CRenderer* CreateRenderer() override;
  void DestroySurface(CSurface* surface) override;
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
  /* 0008 */ CTexture* m_defaultTexture;
  /* 000C */ std::shared_ptr<CSurface> m_backBuffer;

  RenderSystemDX9Context* m_context;
  RendererDX9Context m_rendererContext;
  TextureDX9Context m_textureContext;
  SurfaceDX9Context m_surfaceContext;
  void RefreshContexts();
};

#endif
