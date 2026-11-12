#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/World/World.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/Bitmap/TextureNames.h"

#include <memory>

// OFFSET: 0x00564EB0
// FUNCTION: fn_02209
CRendererDX9::CRendererDX9(DeviceOwnerDX9& owner)
    : device(&owner), nativeDevice(owner.device), clearColor(0), lastTick(0), frames(0), fps(0.0f) {
}

// OFFSET: 0x00564FE0
// FUNCTION: fn_02210
CRendererDX9::~CRendererDX9() = default;

// OFFSET: 0x00568360
// FUNCTION: fn_02238
void CRendererDX9::SetClearColor(unsigned color) {
  this->clearColor = color;
}

// OFFSET: 0x00567E70
// FUNCTION: fn_02231
float CRendererDX9::FPS() const {
  return this->fps;
}

// OFFSET: 0x005650E0
// FUNCTION: fn_02212
bool CRendererDX9::CaptureBackBuffer() {
  return this->device->CaptureBackBuffer();
}

// OFFSET: 0x00567FB0
// FUNCTION: fn_02233
bool CRendererDX9::RestoreBackBuffer() {
  return this->device->RestoreBackBuffer();
}

// OFFSET: 0x00565860
// FUNCTION: fn_02220
void CRendererDX9::DrawAuxiliaryPrimitive(LightmappedPrimitive* primitive) {
  if (primitive->vertexCount <= 0) {
    return;
  }

  const auto type = static_cast<D3DPRIMITIVETYPE>(primitive->primitiveType);
  this->device->state.SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2);

  if (primitive->indexCount) {
    this->nativeDevice->DrawIndexedPrimitiveUP(
      type, 0, primitive->vertexCount, primitive->indexCount / (type == D3DPT_TRIANGLELIST ? 3 : 2), primitive->indices, D3DFMT_INDEX16,
      primitive->vertices, sizeof(LightmappedVertex)
    );
  } else {
    this->nativeDevice->DrawPrimitiveUP(type, primitive->vertexCount / (type == D3DPT_TRIANGLELIST ? 3 : 2), primitive->vertices, sizeof(LightmappedVertex));
  }
}

bool CRendererDX9::SaveBitmapResource(const char* filename, int format, CBitmapRes* bitmap) {
  CTexture* overlay = nullptr;

  if (bitmap) {
    const auto& image = bitmap->image;
    overlay = g_texMgr.Upload(image.width, image.height, image.pixels, image.alpha != 0, 0, false, false, false);
  }

  const bool saved = this->SaveScreenshot(filename, format, overlay, nullptr);
  delete overlay;
  return saved;
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
  CTexture* overlay = g_texMgr.Load(Resolve("scr_logo.bmp"), 0, false, true, false);

  return this->SaveScreenshot(filename.c_str(), 1, overlay, nullptr);
}

void CRendererDX9::Submit(RenderCommand* command, std::uint16_t flags) {
  this->queues.Submit(command, flags, this->fog.supported != 0, g_depthStart, g_depthEnd);
}

// OFFSET: 0x00559BE0
// FUNCTION: fn_02071
void CRendererDX9::Submit(LightmappedPrimitive* command, int queue) {
  if (this->fog.supported) {
    command->ApplyDepthAlpha();
  }

  switch (queue) {
  case 0:
    this->queues.lightmapped.first.push_back(command);
    break;
  case 1:
    this->queues.lightmapped.second.push_back(command);
    break;
  }
}
