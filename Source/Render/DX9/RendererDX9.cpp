#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/TextureQuad.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/World/World.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/TextureNames.h"

#include <memory>

// OFFSET: 0x00564EB0
// FUNCTION: fn_02209
CRendererDX9::CRendererDX9(DeviceOwnerDX9& owner)
    : device(&owner), nativeDevice(owner.device), clearColor(0), lastTick(0), frames(0), fps(0.0f) {
}

// OFFSET: 0x00564FE0
// OFFSET: 0x00565070
// FUNCTION: fn_02210
CRendererDX9::~CRendererDX9() = default;

// OFFSET: 0x00568360
// FUNCTION: fn_02238
void CRendererDX9::SetClearColor(unsigned color) {
  this->clearColor = color;
}

void CRendererDX9::SetFog(int enabled) {
  this->fog.Set(enabled, this->device->caps);
}

HRESULT CRendererDX9::Present(int option) {
  RendererPresentation presentation{this->frame, this->lastTick, this->frames, this->fps};
  return presentation.Present(*this->device, option);
}

// OFFSET: 0x00567E70
// FUNCTION: fn_02231
float CRendererDX9::FPS() const {
  return this->fps;
}

bool CRendererDX9::CaptureBackBuffer() {
  return this->device->CaptureBackBuffer();
}

bool CRendererDX9::RestoreBackBuffer() {
  return this->device->RestoreBackBuffer();
}

// OFFSET: 0x00565860
// FUNCTION: fn_02220
void CRendererDX9::DrawAuxiliaryPrimitive(LightmappedPrimitive* primitive) {
  if (primitive->vertexCount <= 0) {
    return;
  }

  this->device->state.SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2);
  const auto type = static_cast<D3DPRIMITIVETYPE>(primitive->primitiveType);
  const unsigned divisor = type == D3DPT_TRIANGLELIST ? 3 : 2;

  if (primitive->indexCount) {
    this->nativeDevice->DrawIndexedPrimitiveUP(
      type, 0, primitive->vertexCount, primitive->indexCount / divisor, primitive->indices, D3DFMT_INDEX16, primitive->vertices, sizeof(LightmappedVertex)
    );
  } else {
    this->nativeDevice->DrawPrimitiveUP(type, primitive->vertexCount / divisor, primitive->vertices, sizeof(LightmappedVertex));
  }
}

bool CRendererDX9::SaveBitmapResource(const char* filename, int format, CBitmapRes* bitmap) {
  std::unique_ptr<CTexture> overlay;

  if (bitmap) {
    const auto& image = bitmap->image;
    overlay.reset(g_texMgr.Upload(image.width, image.height, image.pixels, image.alpha != 0, 0, false, false, false));
  }

  return this->SaveScreenshot(filename, format, overlay.get(), nullptr);
}

// OFFSET: 0x00567FC0
// FUNCTION: fn_02234
bool CRendererDX9::SaveBitmap(const char* filename, CBitmapRes* overlay) {
  return this->SaveBitmapResource(filename, 0, overlay);
}

// OFFSET: 0x00568030
// FUNCTION: fn_02235
bool CRendererDX9::SaveJpeg(const char* filename, CBitmapRes* overlay) {
  return this->SaveBitmapResource(filename, 1, overlay);
}

// OFFSET: 0x00568EC0
// FUNCTION: fn_02242
void CRendererDX9::CleanupTextures() {
}

// OFFSET: 0x005682D0
// FUNCTION: fn_02237
bool CRendererDX9::SaveLogo(std::string filename) {
  const char* name = Resolve("scr_logo.bmp");
  CTexture* overlay = g_texMgr.Load(name, 0, false, true, false);

  return this->SaveScreenshot(filename.c_str(), 1, overlay, nullptr);
}

void CRendererDX9::Submit(RenderCommand* command, std::uint16_t flags) {
  this->queues.Submit(command, flags, this->fog.supported != 0, g_depthStart, g_depthEnd);
}

void CRendererDX9::Submit(LightmappedPrimitive* command, int queue) {
  this->queues.lightmapped.Submit(command, queue, this->fog.supported != 0, g_depthStart, g_depthEnd);
}
