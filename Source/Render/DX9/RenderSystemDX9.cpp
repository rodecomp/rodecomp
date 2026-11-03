#include "Render/DX9/RenderSystemDX9.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "Render/TextureQuad.h"

#include <bit>
#include <new>

// OFFSET: 0x0056CF60
// FUNCTION: fn_02353
CRenderSystemDX9::CRenderSystemDX9() : m_device(new DeviceOwnerDX9), m_defaultTexture(nullptr) {
}

// OFFSET: 0x0056D180
// OFFSET: 0x0056D320
// FUNCTION: fn_02355
CRenderSystemDX9::~CRenderSystemDX9() {
  delete this->m_device;
  this->m_device = nullptr;
  this->m_backBuffer.reset();
  delete this->m_defaultTexture;
}

// OFFSET: 0x0056E250
// FUNCTION: fn_02373
std::int32_t CRenderSystemDX9::Backend() {
  return 2;
}

// INLINED
HRESULT CRenderSystemDX9::Initialize(HWND window) {
  const HRESULT result =
    InitializeConfiguredDeviceDX9(*this->m_device, window);
  return result;
}

// OFFSET: 0x0056D670
// FUNCTION: fn_02363
HRESULT CRenderSystemDX9::Close() {
  CTexture* texture = this->m_defaultTexture;
  this->m_defaultTexture = nullptr;
  delete texture;
  this->m_backBuffer.reset();

  return this->m_device->Close();
}

// OFFSET: 0x0056E3E0
// FUNCTION: fn_02376
bool CRenderSystemDX9::IsWindowed() {
  return this->m_device->presentation.Windowed == 0;
}

// OFFSET: 0x0056E260
// FUNCTION: fn_02374
std::uint32_t CRenderSystemDX9::Width() {
  return this->m_device->presentation.BackBufferWidth;
}

// OFFSET: 0x0056DB90
// FUNCTION: fn_02370
std::uint32_t CRenderSystemDX9::Height() {
  return this->m_device->presentation.BackBufferHeight;
}

// OFFSET: 0x0056D3F0
// FUNCTION: fn_02359
CRenderer* CRenderSystemDX9::CreateRenderer() {
  auto* renderer = new CRendererDX9(*this->m_device);
  renderer->viewport.SetSize(
    std::bit_cast<std::int32_t>(this->Width()), std::bit_cast<std::int32_t>(this->Height())
  );

  return renderer;
}

// OFFSET: 0x0056D720
// FUNCTION: fn_02364
void CRenderSystemDX9::DestroySurface(CSurface* surface) {
  delete surface;
}

// OFFSET: 0x0056D480
// FUNCTION: fn_02360
CSurface* CRenderSystemDX9::CreateSurface(std::uint32_t width, std::uint32_t height) {
  return new (std::nothrow) CSurfaceDX9(*this->m_device, std::bit_cast<std::int32_t>(width), std::bit_cast<std::int32_t>(height));
}

// OFFSET: 0x0056D510
// FUNCTION: fn_02361
CTexture* CRenderSystemDX9::CreateTexture(std::uint32_t width, std::uint32_t height, std::int32_t format, bool dynamic, bool renderTarget) {
  return new (std::nothrow)
    CTextureDX9(*this->m_device, std::bit_cast<std::int32_t>(width), std::bit_cast<std::int32_t>(height), format, dynamic, renderTarget);
}

// OFFSET: 0x0056D5B0
// FUNCTION: fn_02362
CSurface* CRenderSystemDX9::CreateSurfaceOrTexture(std::uint32_t width, std::uint32_t height) {
  if (this->m_device->caps.MaxTextureWidth < width || this->m_device->caps.MaxTextureHeight < height) {
    return this->CreateSurface(width, height);
  }

  return this->CreateTexture(width, height, TextureFormat::A1R5G5B5, false, false);
}

// OFFSET: 0x0056DAC0
// FUNCTION: fn_02369
CTexture* CRenderSystemDX9::BackBufferTexture() {
  if (!this->m_defaultTexture) {
      this->m_defaultTexture = new CTextureDX9(*this->m_device);
  }

  return this->m_defaultTexture;
}

// OFFSET: 0x0056DBA0
// FUNCTION: fn_02371
std::shared_ptr<CSurface> CRenderSystemDX9::BackBufferSurface() {
  if (!this->m_backBuffer) {
      this->m_backBuffer = std::make_shared<CSurfaceDX9>(*this->m_device, this->m_device->backBuffer);
  }

  return this->m_backBuffer;
}

// OFFSET: 0x0056D740
// FUNCTION: fn_02365
void CRenderSystemDX9::UpdateOption() {
  this->m_device->dynamicTextures = (this->m_device->caps.Caps2 & D3DCAPS2_DYNAMICTEXTURES) != 0 && g_textureFiltering != 0;
}

// OFFSET: 0x0056D760
// FUNCTION: fn_02367
void CRenderSystemDX9::PrepareErrorDialog() {
}

// OFFSET: 0x0056D750
// FUNCTION: fn_02366
void CRenderSystemDX9::RestoreDisplayMode() {
}

// OFFSET: 0x0056E3F0
// FUNCTION: fn_02377
HRESULT CRenderSystemDX9::TestDevice() {
  return this->m_device->ClipFullscreenCursor();
}

// OFFSET: 0x0056E400
// FUNCTION: fn_02378
HRESULT CRenderSystemDX9::Present(bool option) {
  return this->m_device->Present(option);
}
