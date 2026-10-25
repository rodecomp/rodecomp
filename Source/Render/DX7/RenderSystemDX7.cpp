#include "Render/DX7/RenderSystemDX7.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/DX7/SurfaceDX7.h"
#include "Render/DX7/TextureDX7.h"

#include <new>

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
