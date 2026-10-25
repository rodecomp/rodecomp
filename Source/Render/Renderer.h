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

class CRenderSystemBase;
class CBitmapRes;

class CRenderer {
public:
  explicit CRenderer(CRenderSystemBase& system);
  virtual ~CRenderer();
  virtual void SetCamera(const float* eye, const float* target, const float* up) = 0;
  virtual void SetLighting(const float* direction, const float* diffuse, const float* ambient) = 0;
  virtual void SetClearColor(unsigned color) = 0;
  virtual void SetFog(int enabled) = 0;
  virtual void ClearDepth(int clearColor) = 0;
  virtual void ClearWorld() = 0;
  virtual bool Draw() = 0;
  virtual HRESULT Present(int option) = 0;
  virtual float FPS() const = 0;
  virtual bool CaptureBackBuffer() = 0;
  virtual bool RestoreBackBuffer() = 0;
  virtual void DrawAuxiliaryPrimitive(LightmappedPrimitive* primitive) = 0;
  virtual void DrawLightmappedPrimitive(LightmappedPrimitive* primitive) = 0;
  virtual void DrawPrimitive(RenderCommand* primitive) = 0;
  virtual bool RenderToTexture(int width, int height, const std::list<RenderPrimitive*>& primitives, CTexture*& texture) = 0;
  virtual bool SaveBitmap(const char* filename, CBitmapRes* overlay) = 0;
  virtual bool SaveJpeg(const char* filename, CBitmapRes* overlay) = 0;
  virtual void CleanupTextures() = 0;
  virtual bool SaveLogo(std::string filename) = 0;
  virtual bool LockBackBuffer() = 0;
  virtual HRESULT UnlockBackBuffer() = 0;

  /* 0004 */ RendererViewport viewport;
  /* 0038 */ unsigned frame;
  /* 003C */ int unknown_003C;
  /* 0040 */ int unknown_0040;
  /* 0044 */ int unknown_0044;
  /* 0048 */ unsigned char twoPass;
  unsigned char unknown_0049[3];
  /* 004C */ int unknown_004C;
  /* 0050 */ RenderCommandPool commands;
  /* 0070 */ RenderPrimitivePool primitives;
  /* 0090 */ LightmappedPrimitivePool lightmapped;
  /* 00B0 */ RenderVertexPool vertices;
  /* 00C0 */ SpriteAtlas atlas;
  /* 0150 */ RendererFogDX9 fog;
  /* 015C */ RenderQueues queues;
  /* 024C */ unsigned unknown_024C;
  /* 0250 */ RendererBackBuffer backBuffer;

  CRenderer(const CRenderer&) = delete;
  CRenderer& operator=(const CRenderer&) = delete;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRenderer) == 0x25C);
#endif

#endif
