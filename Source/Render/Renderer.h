#ifndef RAGNAROK_SOURCE_RENDER_RENDERER_H
#define RAGNAROK_SOURCE_RENDER_RENDERER_H

#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderCommandPool.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Primitives/RenderVertexPool.h"
#include "Render/RendererBackBuffer.h"
#include "Render/DX9/RendererFogDX9.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Textures/SpriteAtlas.h"

#include <string>
#include <new>

class CRenderSystemBase;
class CBitmapRes;

class CRenderer {
public:
  CRenderer();
  virtual ~CRenderer();
  virtual void SetCamera(const float* eye, const float* target, const float* up) = 0;
  virtual void SetLighting(const float* direction, const float* diffuse, const float* ambient) = 0;
  virtual void SetClearColor(unsigned color) = 0;
  virtual void SetFog(int enabled) = 0;
  virtual void ClearDepth(int clearColor) = 0;
  virtual void ClearWorld() = 0;
  virtual bool Draw() = 0;
  virtual void Present(bool option) = 0;
  virtual float FPS() const = 0;
  virtual void CaptureBackBuffer() = 0;
  virtual void RestoreBackBuffer() = 0;
  virtual void DrawAuxiliaryPrimitive(LightmappedCommand* primitive) = 0;
  virtual void DrawLightmappedPrimitive(LightmappedPrimitive* primitive) = 0;
  virtual void DrawPrimitive(RenderCommand* primitive) = 0;
  virtual bool RenderToTexture(int width, int height, const std::list<RenderCommand*>& primitives, CTexture*& texture) = 0;
  virtual int SaveBitmap(const char* filename, CBitmapRes* overlay) = 0;
  virtual int SaveJpeg(const char* filename, CBitmapRes* overlay) = 0;
  virtual void CleanupTextures() = 0;
  virtual bool SaveLogo(std::string filename) = 0;
  virtual int LockBackBuffer() = 0;
  virtual HRESULT UnlockBackBuffer() = 0;
  void SetSize(int width, int height);
  void Project(const float* position, const float* matrix, RenderVertex& output) const;
  void Project(const float* position, const float* matrix, LightmappedVertex& output) const;
  void Ray(int x, int y, const float* origin, const float* matrix, float* output) const;
  RenderCommand* AcquireCommand();
  RenderPrimitive* AcquirePrimitive();
  float* ProjectViewport(const float* position, const float* matrix, float* x, float* y, float* reciprocalDepth);
  float* ProjectViewportCentered(
    const float* position, const float* matrix, float* x, float* y, float* reciprocalDepth, float width, float height
  );
  float ScaleDistance(float distance) const;
  float UiScaleX() const;
  float UiScaleY() const;
  std::uint32_t BlendColor(std::uint32_t base, std::uint32_t overlay);
  void Submit(RenderCommand* primitive, std::uint16_t flags);
  void SubmitTriangle(RenderTriangle& triangle, std::uint16_t flags);
  void SubmitAuxiliaryPrimitive(LightmappedCommand* primitive);
  void DrawBattleFieldIcon();
  void DrawHourlyBanner();
  void DrawOverlays();
  void DrawOverlays(CRenderSystemBase& system);
  __declspec(noinline) void ClearFrame();
  void ReserveQueues(unsigned count);
  void ClearQueues();
  int DrawBackBufferText(int x, int y, const char* text, COLORREF color);

  /* 0004 */ RendererViewport viewport;
  /* 0038 */ unsigned frame;
  /* 003C */ float viewDirectionX;
  /* 0040 */ float viewDirectionY;
  /* 0044 */ float viewDirectionZ;
  /* 0048 */ unsigned char twoPass;
  /* 0049 */ unsigned char padding_0049[3];
  /* 004C */ int unknown_004C;
  /* 0050 */ RenderCommandPool commands;
  /* 0070 */ RenderPrimitivePool primitives;
  /* 0090 */ LightmappedPrimitivePool lightmapped;
  /* 00B0 */ RenderVertexPool vertices;
  /* 00C0 */ SpriteAtlas atlas;
  /* 0150 */ RendererFogDX9 fog;
  /* 015C */ std::vector<RenderCommand*> queue_015C;
  /* 0168 */ std::vector<RenderCommand*> queue_0168;
  /* 0174 */ std::vector<RenderCommand*> queue_0174;
  /* 0180 */ std::vector<DepthPrimitive> queue_0180;
  /* 018C */ std::vector<DepthPrimitive> queue_018C;
  /* 0198 */ std::vector<DepthPrimitive> queue_0198;
  /* 01A4 */ std::vector<DepthPrimitive> queue_01A4;
  /* 01B0 */ std::vector<RenderCommand*> queue_01B0;
  /* 01BC */ std::vector<DepthPrimitive> queue_01BC;
  /* 01C8 */ std::vector<DepthPrimitive> queue_01C8;
  /* 01D4 */ std::vector<DepthPrimitive> queue_01D4;
  /* 01E0 */ std::vector<DepthPrimitive> queue_01E0;
  /* 01EC */ std::vector<RenderCommand*> queue_01EC;
  /* 01F8 */ std::vector<LightmappedPrimitive*> lightmappedFirst;
  /* 0204 */ std::vector<LightmappedPrimitive*> lightmappedSecond;
  /* 0210 */ std::vector<RenderCommand*> queue_0210;
  /* 021C */ std::vector<DepthPrimitive> queue_021C;
  /* 0228 */ std::vector<DepthPrimitive> queue_0228;
  /* 0234 */ std::vector<DepthPrimitive> queue_0234;
  /* 0240 */ std::vector<LightmappedCommand*> queue_0240;
  /* 024C */ unsigned textureFormat;
  /* 0250 */ RendererBackBuffer backBuffer;

  CRenderer(const CRenderer&) = delete;
  CRenderer& operator=(const CRenderer&) = delete;
};

extern CRenderer* g_renderer;

int ScaleByScreenHeight(int value);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRenderer) == 0x25C);
static_assert(offsetof(CRenderer, padding_0049) == 0x0049);
static_assert(offsetof(CRenderer, commands) == 0x0050);
static_assert(offsetof(CRenderer, primitives) == 0x0070);
static_assert(offsetof(CRenderer, lightmapped) == 0x0090);
static_assert(offsetof(CRenderer, vertices) == 0x00B0);
static_assert(offsetof(CRenderer, atlas) == 0x00C0);
static_assert(offsetof(CRenderer, fog) == 0x0150);
static_assert(offsetof(CRenderer, queue_015C) == 0x015C);
static_assert(offsetof(CRenderer, lightmappedFirst) == 0x01F8);
static_assert(offsetof(CRenderer, queue_0240) == 0x0240);
static_assert(offsetof(CRenderer, backBuffer) == 0x0250);
#endif

// INLINED
inline void CRenderer::ReserveQueues(unsigned count) {
  this->lightmappedFirst.reserve(count);
  this->queue_015C.reserve(count);
  this->queue_0168.reserve(count);
  this->queue_0174.reserve(count);
  this->queue_0180.reserve(count);
  this->queue_018C.reserve(count);
  this->queue_0198.reserve(count);
  this->queue_01A4.reserve(count);
  this->queue_01B0.reserve(count);
  this->queue_01BC.reserve(count);
  this->queue_01C8.reserve(count);
  this->queue_01D4.reserve(count);
  this->queue_01E0.reserve(count);
  this->queue_01EC.reserve(count);
  this->lightmappedSecond.reserve(count);
  this->queue_0210.reserve(count);
  this->queue_021C.reserve(count);
  this->queue_0228.reserve(count);
  this->queue_0234.reserve(count);
  this->queue_0240.reserve(count);
}

// INLINED
inline void CRenderer::ClearQueues() {
  this->queue_015C.clear();
  this->queue_0168.clear();
  this->queue_0174.clear();
  this->queue_0180.clear();
  this->queue_018C.clear();
  this->queue_0198.clear();
  this->queue_01A4.clear();
  this->queue_01B0.clear();
  this->queue_01BC.clear();
  this->queue_01C8.clear();
  this->queue_01D4.clear();
  this->queue_01E0.clear();
  this->queue_01EC.clear();
  this->lightmappedFirst.clear();
  this->lightmappedSecond.clear();
  this->queue_0210.clear();
  this->queue_021C.clear();
  this->queue_0228.clear();
  this->queue_0234.clear();
  this->queue_0240.clear();
}

// OFFSET: 0x00CA63D0
// FUNCTION: fn_20958
inline RenderCommand* CRenderer::AcquireCommand() {
  return this->commands.Acquire();
}

#endif
