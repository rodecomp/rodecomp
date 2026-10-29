#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/DX9/RendererFrameDX9.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/TextureNames.h"

#include <memory>

// OFFSET: 0x00564EB0
// FUNCTION: fn_02209
CRendererDX9::CRendererDX9(DeviceOwnerDX9& owner, RendererDX9Context& context)
    : CRenderer(context.system), device(&owner), nativeDevice(owner.device), clearColor(0), lastTick(0), frames(0), fps(0.0f), m_context(&context),
      m_textureContext(owner.TextureContext(context.d3dx, this->primitives, this->queues, false, context.fog.start, context.fog.end)) {
}

// OFFSET: 0x00564FE0
// OFFSET: 0x00565070
// FUNCTION: fn_02210
CRendererDX9::~CRendererDX9() = default;

TextureDX9Context& CRendererDX9::TextureContext() {
  this->m_textureContext.dynamicTextures = this->device->dynamicTextures;
  this->m_textureContext.depthAlpha = this->fog.supported != 0;
  this->m_textureContext.depthStart = this->m_context->fog.start;
  this->m_textureContext.depthEnd = this->m_context->fog.end;

  return this->m_textureContext;
}

RendererDX9Operations CRendererDX9::Operations(TextureDX9Context& context) {
  context.device = this->nativeDevice;
  return {context, this->device->state};
}

void CRendererDX9::SetCamera(const float* eye, const float* target, const float* up) {
  auto& context = this->TextureContext();
  this->Operations(context).SetCamera(eye, target, up);
}

void CRendererDX9::SetLighting(const float* direction, const float* diffuse, const float* ambient) {
  auto& context = this->TextureContext();
  this->Operations(context).SetLighting(direction, diffuse, ambient);
}

// OFFSET: 0x00568360
// FUNCTION: fn_02238
void CRendererDX9::SetClearColor(unsigned color) {
  this->clearColor = color;
}

void CRendererDX9::SetFog(int enabled) {
  this->fog.Set(enabled, this->device->caps, this->m_context->fog);
}

void CRendererDX9::ClearDepth(int clearColor) {
  auto& context = this->TextureContext();
  this->Operations(context).ClearDepth(clearColor);
}

void CRendererDX9::ClearWorld() {
  auto& context = this->TextureContext();
  this->Operations(context).ClearWorld(this->clearColor, this->fog.enabled, this->m_context->worldFlag, this->m_context->overlayColor);
}

bool CRendererDX9::Draw() {
  auto& context = this->TextureContext();
  auto operations = this->Operations(context);
  RendererFrameDX9 frame{*this->device,     operations,     this->commands, this->primitives,
                         this->lightmapped, this->vertices, this->queues,   this->m_context->retirement};

  return frame.Draw(
    &this->m_context->system, this->m_context->options, this->m_context->multitexture, this->twoPass, this->fog, this->viewport.aspect, this->m_context->fog
  );
}

HRESULT CRendererDX9::Present(int option) {
  RendererPresentation presentation{this->frame, this->lastTick, this->frames, this->fps, this->m_context->diagnostic};
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
  auto& context = this->TextureContext();
  this->Operations(context).DrawLightmappedPrimitive(primitive);
}

void CRendererDX9::DrawLightmappedPrimitive(LightmappedPrimitive* primitive) {
  auto& context = this->TextureContext();
  this->Operations(context).DrawLightmappedPrimitive(primitive);
}

void CRendererDX9::DrawPrimitive(RenderCommand* primitive) {
  auto& context = this->TextureContext();
  this->Operations(context).DrawPrimitive(primitive);
}

bool CRendererDX9::RenderToTexture(int width, int height, const std::list<RenderPrimitive*>& primitives, CTexture*& texture) {
  auto& context = this->TextureContext();
  return this->Operations(context).RenderToTexture(width, height, primitives, texture);
}

bool CRendererDX9::SaveBitmapResource(const char* filename, int format, CBitmapRes* bitmap) {
  std::unique_ptr<CTexture> overlay;

  if (bitmap) {
    const auto& image = bitmap->image;
    overlay.reset(g_texMgr.Upload(image.width, image.height, image.pixels, image.alpha != 0, 0, false, false, false));
  }

  auto& context = this->TextureContext();
  return this->Operations(context).SaveScreenshot(*this->device, &this->m_context->system, filename, format, overlay.get(), nullptr);
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
  auto& context = this->TextureContext();

  return this->Operations(context).SaveScreenshot(*this->device, &this->m_context->system, filename.c_str(), 1, overlay, nullptr);
}

void CRendererDX9::Submit(RenderCommand* command, std::uint16_t flags) {
  this->queues.Submit(command, flags, this->fog.supported != 0, this->m_context->fog.start, this->m_context->fog.end);
}

void CRendererDX9::Submit(LightmappedPrimitive* command, int queue) {
  this->queues.lightmapped.Submit(command, queue, this->fog.supported != 0, this->m_context->fog.start, this->m_context->fog.end);
}
