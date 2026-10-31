#ifndef RAGNAROK_SOURCE_RENDER_DX7_RENDERSYSTEMDX7_H
#define RAGNAROK_SOURCE_RENDER_DX7_RENDERSYSTEMDX7_H

#include "Render/RenderSystemBase.h"

#include <memory>

struct TextureAllocationDX7;

class DeviceOwnerDX7;
class CTextureDX7;

class CRenderSystemDX7 : public CRenderSystemBase {
public:
  CRenderSystemDX7();
  ~CRenderSystemDX7() override;

  std::int32_t Backend() override;
  HRESULT Initialize(HWND window) override;
  HRESULT Close() override;
  bool IsWindowed() override;
  std::uint32_t Width() override;
  std::uint32_t Height() override;
  CRenderer* CreateRenderer() override;
  void DestroySurface(CSurface* surface) override;
  CSurface* CreateSurface(std::uint32_t width, std::uint32_t height) override;
  CTexture* CreateTexture(std::uint32_t width, std::uint32_t height, std::int32_t format, bool option1, bool option2) override;
  CSurface* CreateSurfaceOrTexture(std::uint32_t width, std::uint32_t height) override;
  CTexture* BackBufferTexture() override;
  std::shared_ptr<CSurface> BackBufferSurface() override;
  void UpdateOption() override;
  void RestoreDisplayMode() override;
  void PrepareErrorDialog() override;
  HRESULT TestDevice() override;
  HRESULT Present(bool option) override;
  int RefreshRendererCapabilities(int mipmaps, TextureAllocationDX7& allocation, bool& multitexture, CRenderSystemBase* renderSystem);

  // INLINED
  DeviceOwnerDX7* Device() const {
    return this->m_device;
  }

private:
  /* 0004 */ DeviceOwnerDX7* m_device;
  /* 0008 */ CTextureDX7* m_defaultTexture;
  /* 000C */ std::shared_ptr<CSurface> m_backBuffer;
};

extern int g_configuredFullscreen;
extern int g_configuredBitsPerPixel;
extern int g_configuredScreenWidth;
extern int g_configuredScreenHeight;
extern int g_configuredOldWidth;
extern int g_configuredOldHeight;
extern int g_configuredDeviceCount;
extern int g_configuredModeCount;
extern int g_renderSystemReset;
extern GUID g_configuredDeviceGuid;
extern char g_configuredDeviceName[0x20];
extern int g_viewportWidth;
extern int g_viewportHeight;
extern const GUID g_defaultDrawGuidDX7;

void ClearDirectDrawModes();

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRenderSystemDX7) == 20);
#endif

#endif
