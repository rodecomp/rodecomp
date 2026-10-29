#include "Render/DX7/RenderSystemDX7.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/DX7/DirectDrawEnumeration.h"
#include "Render/DX7/SurfaceDX7.h"
#include "Render/DX7/TextureAllocationDX7.h"
#include "Render/DX7/TextureDX7.h"
#include "Render/DeviceSelection.h"
#include "Render/RenderAdapter.h"

#include <new>

// GLOBAL: gl_24503
DeviceOwnerDX7* g_deviceOwnerDX7;

// GLOBAL: gl_24765
int g_configuredFullscreen;

// GLOBAL: gl_24716
int g_configuredScreenWidth;

// GLOBAL: gl_24717
int g_configuredScreenHeight;

// GLOBAL: gl_24764
int g_configuredBitsPerPixel;

// OFFSET: 0x0056AFE0
// FUNCTION: fn_02307
CRenderSystemDX7::CRenderSystemDX7() : m_defaultTexture(nullptr) {
  this->m_device = new DeviceOwnerDX7;
  g_deviceOwnerDX7 = this->m_device;
}

// OFFSET: 0x0056B500
// FUNCTION: fn_02315
CRenderSystemDX7::~CRenderSystemDX7() {
  delete this->m_device;
  this->m_device = nullptr;
  this->m_backBuffer.reset();
  delete this->m_defaultTexture;
}

// OFFSET: 0x0056B5D0
// FUNCTION: fn_02316
void ClearDirectDrawModes() {
  DirectDrawEnumeration::ClearModes();
}

// OFFSET: 0x0056B5E0
// FUNCTION: fn_02317
int CRenderSystemDX7::RefreshRendererCapabilities(int mipmaps, TextureAllocationDX7& allocation, bool& multitexture, CRenderSystemBase* renderSystem) {
  return this->m_device->RefreshRendererCapabilities(mipmaps, allocation, multitexture, renderSystem);
}

// OFFSET: 0x0056B5F0
// FUNCTION: fn_02318
CSurface* CRenderSystemDX7::CreateSurface(std::uint32_t width, std::uint32_t height) {
  return new (std::nothrow) CSurfaceDX7(*this->m_device, static_cast<int>(width), static_cast<int>(height));
}

// OFFSET: 0x0056B680
// FUNCTION: fn_02319
CTexture* CRenderSystemDX7::CreateTexture(std::uint32_t width, std::uint32_t height, std::int32_t format, bool, bool) {
  return new (std::nothrow) CTextureDX7(*reinterpret_cast<TextureDX7Context*>(this->m_device), static_cast<int>(width), static_cast<int>(height), format);
}

// OFFSET: 0x0056B710
// FUNCTION: fn_02320
CSurface* CRenderSystemDX7::CreateSurfaceOrTexture(std::uint32_t width, std::uint32_t height) {
  if (this->m_device->SupportsTextureDimensions(width, height)) {
    return new CTextureDX7(*reinterpret_cast<TextureDX7Context*>(this->m_device), static_cast<int>(width), static_cast<int>(height), TextureFormat::A1R5G5B5);
  }

  return new CSurfaceWallpaperDX7(*this->m_device, static_cast<int>(this->m_device->width), static_cast<int>(this->m_device->height));
}

// OFFSET: 0x0056B7C0
// FUNCTION: fn_02321
HRESULT CRenderSystemDX7::Close() {
  delete this->m_defaultTexture;
  this->m_defaultTexture = nullptr;
  this->m_backBuffer.reset();

  return this->m_device->Close();
}

// OFFSET: 0x0056B870
// FUNCTION: fn_02322
void CRenderSystemDX7::DestroySurface(CSurface* surface) {
  delete surface;
}

// OFFSET: 0x0056B880
// FUNCTION: fn_02323
void CRenderSystemDX7::UpdateOption() {
  if (this->m_device) {
    const bool trilinear = (this->m_device->caps.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEARMIPLINEAR) != 0;
    (void)trilinear;
  }
}

// OFFSET: 0x0056BBA0
// FUNCTION: fn_02328
CTexture* CRenderSystemDX7::BackBufferTexture() {
  if (!this->m_defaultTexture) {
    this->m_defaultTexture = new CTextureDX7(*reinterpret_cast<TextureDX7Context*>(this->m_device));
  }

  return this->m_defaultTexture;
}

// OFFSET: 0x0056BC70
// FUNCTION: fn_02329
std::uint32_t CRenderSystemDX7::Height() {
  return this->m_device->height;
}

// OFFSET: 0x0056BC80
// FUNCTION: fn_02330
std::shared_ptr<CSurface> CRenderSystemDX7::BackBufferSurface() {
  if (!this->m_backBuffer) {
    this->m_backBuffer = std::make_shared<CSurfaceDX7>(*this->m_device, this->m_device->renderTarget);
  }

  return this->m_backBuffer;
}

// OFFSET: 0x0056C290
// FUNCTION: fn_02332
std::int32_t CRenderSystemDX7::Backend() {
  return 1;
}

// OFFSET: 0x0056C2A0
// FUNCTION: fn_02333
std::uint32_t CRenderSystemDX7::Width() {
  return this->m_device->width;
}

// OFFSET: 0x0056C2B0
// FUNCTION: fn_02334
HRESULT CRenderSystemDX7::Initialize(HWND window) {
  const std::vector<RenderAdapter> adapters = EnumerateAdaptersDX7();

  if (adapters.empty()) {
    MessageBoxA(nullptr, "No compatible devices", "Error", 0);
    return static_cast<HRESULT>(0x81000004);
  }

  GUID drawIdentity{};
  GUID renderIdentity{};
  const int backend = 1;
  const GUID savedDraw = *reinterpret_cast<const GUID*>(g_guidDriver);
  const GUID savedRender = *reinterpret_cast<const GUID*>(g_guidDevice);
  const GUID savedDevice = *reinterpret_cast<const GUID*>(g_guidDevice);
  const AdapterConfiguration configuration{backend, savedDraw, savedRender, savedDevice, g_deviceName};
  const RenderAdapter selected = SelectedRenderAdapter(configuration, true);

  for (const RenderAdapter& adapter : adapters) {
    if (adapter.Matches(selected)) {
      drawIdentity = adapter.drawGuid;
      renderIdentity = adapter.renderGuid;
      break;
    }
  }

  DDSURFACEDESC2 mode{};
  mode.dwWidth = static_cast<DWORD>(g_configuredScreenWidth);
  mode.dwHeight = static_cast<DWORD>(g_configuredScreenHeight);
  mode.ddpfPixelFormat.dwRGBBitCount = static_cast<DWORD>(g_configuredBitsPerPixel);
  const DWORD flags = g_configuredFullscreen != 0 ? 7 : 6;
  return this->m_device->Initialize(window, &drawIdentity, &renderIdentity, &mode, flags);
}

// OFFSET: 0x0056C4A0
// FUNCTION: fn_02335
bool CRenderSystemDX7::IsWindowed() {
  return this->m_device->fullscreen != 0;
}

// OFFSET: 0x0056C4C0
// FUNCTION: fn_02337
HRESULT CRenderSystemDX7::Present(bool option) {
  return this->m_device->Present(option);
}
