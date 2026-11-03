#ifndef RAGNAROK_SOURCE_RENDER_RENDERERDX7_H
#define RAGNAROK_SOURCE_RENDER_RENDERERDX7_H

#include "Render/Renderer.h"

#include <list>
#include <string>

class CTextureDX7;
class ResourceMgr;
struct IDirect3DDevice7;
struct BitmapImage;

class CRendererDX7 : public CRenderer {
public:
  CRendererDX7();

  /* 025C */ unsigned samples;
  /* 0260 */ unsigned time;
  /* 0264 */ unsigned rate;
  /* 0268 */ CTextureDX7* boundTexture;
  /* 026C */ CTextureDX7* boundLightmap;
  /* 0270 */ unsigned clearColor;
  /* 0274 */ IDirect3DDevice7* device;
  /* 0278 */ unsigned char unknown_0278[0xC];

  void SetCamera(const float* eye, const float* target, const float* up) override;
  void SetClearColor(unsigned color) override;

  void SetTextureFormat(unsigned format);
  void SetLighting(const float* direction, const float* diffuse, const float* ambient);
  void ClearDepth(int clearColor);
  void ClearWorld(unsigned color, int mode, unsigned char worldFlag, unsigned overlay);
  void DrawPrimitive(RenderCommand* primitive);
  void DrawLightmappedPrimitive(LightmappedPrimitive* primitive);
  void SetTextureMode(int mode);
  bool RenderToTexture(int width, int height, const std::list<RenderPrimitive*>& primitives, CTexture*& texture);
  bool SaveBitmap(const char* filename, const BitmapImage& overlay, int width, int height, RendererBackBuffer& backBuffer);
  bool SaveJpeg(const char* filename, const BitmapImage& overlay, int width, int height, RendererBackBuffer& backBuffer);
  bool SaveLogo(std::string filename, ResourceMgr& resources);
  void SetTexture(unsigned stage, CTextureDX7* texture);
  void Present(bool wait);
  float FrameRate() const;
  void DrawFPS();
  void DrawDepthSortedQueue();
  void DrawAlphaQueues();
  void DrawOpaqueQueues();
  void DrawGroundBaseQueue();
  void DrawAuxiliaryQueue();
  // INLINED
  void BindTexture(CTextureDX7* texture);
  // INLINED
  void BindLightmap(CTextureDX7* lightmap);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRendererDX7) == 0x284);
static_assert(offsetof(CRendererDX7, clearColor) == 0x0270);
static_assert(offsetof(CRendererDX7, boundTexture) == 0x0268);
static_assert(offsetof(CRendererDX7, boundLightmap) == 0x026C);
static_assert(offsetof(CRendererDX7, device) == 0x0274);
#endif

#endif
