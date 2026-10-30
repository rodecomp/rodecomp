#include "Render/DeviceContext/DCSurface.h"
#include "Render/RenderSystemBase.h"
#include "Render/Textures/Surface.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "Render/DX9/RenderSystemDX9.h"
#include "Render/Text/TextContext.h"
#include "Client/Session.h"
#include "Script/LuaScript.h"

#include <algorithm>
#include <cstring>
#include <utility>

// OFFSET: 0x0056A500
// FUNCTION: fn_02290
std::shared_ptr<CSurface> AcquireBackBufferSurface(CRenderSystemBase* system) {
  return system ? system->BackBufferSurface() : std::shared_ptr<CSurface>{};
}

// OFFSET: 0x0055FAD0
// FUNCTION: fn_02121
int DrawBackBufferText(int x, int y, const char* text, COLORREF color) {
  CDCSurface surface(AcquireBackBufferSurface(GetRenderSystemHolder()->system));
  TextContext context(surface);
  context.SetFont(0, 12, 0, 0);
  SetTextColor(context.Dc(), color);
  TextOutA(context.Dc(), x, y, text, static_cast<int>(std::strlen(text)));
  return 0;
}

// OFFSET: 0x00569F10
// FUNCTION: fn_02274
CRenderer* RenderSystemHolder::CreateRenderer() {
  return this->system ? this->system->CreateRenderer() : nullptr;
}

// OFFSET: 0x00569F20
// FUNCTION: fn_02275
CSurface* RenderSystemHolder::CreateSurface(std::uint32_t width, std::uint32_t height) {
  return this->system ? this->system->CreateSurface(width, height) : nullptr;
}

// OFFSET: 0x00569F40
// FUNCTION: fn_02276
CTexture* RenderSystemHolder::CreateTexture(std::uint32_t width, std::uint32_t height, std::int32_t format, bool dynamic, bool renderTarget) {
  return this->system ? this->system->CreateTexture(width, height, format, dynamic, renderTarget) : nullptr;
}

// OFFSET: 0x00569F60
// FUNCTION: fn_02277
CSurface* RenderSystemHolder::CreateSurfaceOrTexture(std::uint32_t width, std::uint32_t height) {
  return this->system ? this->system->CreateSurfaceOrTexture(width, height) : nullptr;
}

// OFFSET: 0x00569F80
// FUNCTION: fn_02278
HRESULT RenderSystemHolder::Close() {
  return this->system ? this->system->Close() : E_ILLEGAL_METHOD_CALL;
}

// OFFSET: 0x00569FA0
// FUNCTION: fn_02279
void RenderSystemHolder::DestroySurface(CSurface* surface) {
  if (this->system) {
    this->system->DestroySurface(surface);
  }
}

// OFFSET: 0x00569FC0
// FUNCTION: fn_02280
void RenderSystemHolder::UpdateOption() {
  if (this->system) {
    this->system->UpdateOption();
  }
}

// OFFSET: 0x00569FD0
// FUNCTION: fn_02281
void RenderSystemHolder::RestoreDisplayMode() {
  if (this->system) {
    this->system->RestoreDisplayMode();
  }
}

// OFFSET: 0x00569FE0
// FUNCTION: fn_02282
void RenderSystemHolder::PrepareErrorDialog() {
  if (this->system) {
    this->system->PrepareErrorDialog();
  }
}

// OFFSET: 0x0056A230
// FUNCTION: fn_02285
std::int32_t RenderSystemHolder::Backend() const {
  return this->system ? this->system->Backend() : -1;
}

// OFFSET: 0x0056A240
// FUNCTION: fn_02286
ScreenDimensions* DefaultResolution(ScreenDimensions* output) {
  output->height = 768;
  output->width = 1024;
  return output;
}

// OFFSET: 0x0056A280
// FUNCTION: fn_02287
CTexture* RenderSystemHolder::BackBufferTexture() const {
  return this->system ? this->system->BackBufferTexture() : nullptr;
}

// OFFSET: 0x0056A290
// FUNCTION: fn_02288
std::uint32_t RenderSystemHolder::Height() const {
  return this->system ? this->system->Height() : 0;
}

// OFFSET: 0x0056A690
// FUNCTION: fn_02292
std::uint32_t RenderSystemHolder::Width() const {
  return this->system ? this->system->Width() : 0;
}

// OFFSET: 0x00E30A50
// FUNCTION: fn_24169
int CSession::DefaultRenderBackend() {
  return std::max(this->m_hotkeyScript->GlobalInteger("RenderSystemDefault"), 0);
}

// OFFSET: 0x0056A6A0
// FUNCTION: fn_02293
HRESULT RenderSystemHolder::Initialize(int backend, HWND window) {
  if (this->system != nullptr) {
    return 0x8000000e;
  }

  if (backend == 0) {
    backend = (g_session.DefaultRenderBackend() == 1) ? 1 : 2;
  }

  CRenderSystemBase* renderSystem;
  if (backend == 2) {
    renderSystem = new CRenderSystemDX9();
  } else if (backend == 1) {
    renderSystem = new CRenderSystemDX7();
  } else {
    return 0x80070057;
  }

  this->system = renderSystem;
  return renderSystem->Initialize(window);
}

// OFFSET: 0x0056A790
// FUNCTION: fn_02294
bool RenderSystemHolder::IsWindowed() const {
  return this->system ? this->system->IsWindowed() : false;
}

// OFFSET: 0x0056A7A0
// FUNCTION: fn_02295
HRESULT RenderSystemHolder::TestDevice() const {
  return this->system ? this->system->TestDevice() : E_ILLEGAL_METHOD_CALL;
}

// OFFSET: 0x0056A7C0
// FUNCTION: fn_02296
HRESULT RenderSystemHolder::Present(bool option) const {
  return this->system ? this->system->Present(option) : E_ILLEGAL_METHOD_CALL;
}

// OFFSET: 0x00548FE0
// FUNCTION: fn_01864
CDCSurface::CDCSurface(std::shared_ptr<CSurface> surface) : m_surface(std::move(surface)) {
}

// OFFSET: 0x005490D0
// OFFSET: 0x00549180
// FUNCTION: fn_01865
CDCSurface::~CDCSurface() = default;

// OFFSET: 0x00549150
// FUNCTION: fn_01866
void CDCSurface::Reset() {
}

// OFFSET: 0x005492C0
// FUNCTION: fn_01879
void CDCSurface::Grow(std::uint32_t width, std::uint32_t height) {
  if (width > this->m_surface->Width() || height > this->m_surface->Height()) {
    this->m_surface->Resize(width, height);
  }
}

// OFFSET: 0x005492A0
// FUNCTION: fn_01877
bool CDCSurface::GetDC(HDC* output) {
  return this->m_surface->GetDC(output);
}

// OFFSET: 0x005492B0
// FUNCTION: fn_01878
void CDCSurface::ReleaseDC(HDC dc) {
  this->m_surface->ReleaseDC(dc);
}

// OFFSET: 0x00549320
// FUNCTION: fn_01881
void CDCSurface::Copy(int x, int y, int width, int height, const std::uint32_t* source, bool transparent) {
  this->m_surface->Upload(x, y, width, height, source, transparent, 0);
}

// OFFSET: 0x005492F0
// FUNCTION: fn_01880
void CDCSurface::ScaleCopy(int x, int y, int sourceWidth, int sourceHeight, int, int, const std::uint32_t* source, int option) {
  this->m_surface->Upload(x, y, sourceWidth, sourceHeight, source, option, 0);
}

// OFFSET: 0x00549260
// FUNCTION: fn_01873
void CDCSurface::Fill(const RECT* rectangle, std::uint32_t color) {
  this->m_surface->Fill(rectangle, color);
}

// OFFSET: 0x00549290
// FUNCTION: fn_01876
void CDCSurface::Draw(CRenderSystemBase&, int x, int y, int width, int height, std::uint32_t color) {
  this->m_surface->Draw(x, y, width, height, color);
}

// OFFSET: 0x00549360
// FUNCTION: fn_01883
void CDCSurface::CopyRegion(int, int, int, int, int, int, const std::uint32_t*, int, int, bool, bool) {
}

// OFFSET: 0x00549270
// FUNCTION: fn_01874
void CDCSurface::CopyFrom(int, int, int, int, int, int, const CDCBitmap&) {
}

// OFFSET: 0x00549280
// FUNCTION: fn_01875
void CDCSurface::BlendFrom(std::uint32_t, std::uint32_t, int, int, const CDCBitmap&) {
}

// OFFSET: 0x00549230
// FUNCTION: fn_01870
void CDCSurface::DrawRleSprite(int, int, std::span<const SpriteFrameCell>, const SpriteRleImage* const* const*, const std::uint32_t*, int) {
}

// OFFSET: 0x00549240
// FUNCTION: fn_01871
void CDCSurface::DrawSurface(int, int, CSurface*, int, int, int, int, bool, int, int, int, int) {
}

// OFFSET: 0x00549220
// FUNCTION: fn_01869
void CDCSurface::DrawScaledSprite(
  int, int, int, int, std::span<const SpriteFrameCell>, const SpriteRleImage* const* const*, int, int, int, float, const std::uint32_t*, SpriteAtlas&,
  CRenderSystemBase&, int, std::uint32_t
) {
}

// OFFSET: 0x00549250
// FUNCTION: fn_01872
void CDCSurface::DrawScaledSurface(int, int, int, int, CSurface*, int, int, float, float, std::uint32_t, bool, int, int, int, int, int) {
}

// OFFSET: 0x00549370
// FUNCTION: fn_01884
void CDCSurface::Brighten(int, int, int, int, int) {
}

// OFFSET: 0x00549350
// FUNCTION: fn_01882
void CDCSurface::Dim(int, int, int, int, int) {
}
