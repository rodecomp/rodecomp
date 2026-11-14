#ifndef RAGNAROK_SOURCE_RENDER_DX9_RENDERERDX9_H
#define RAGNAROK_SOURCE_RENDER_DX9_RENDERERDX9_H

#include "Render/Renderer.h"
#include <list>
#include <vector>
#include "Render/DX9/TextureDX9.h"

class DeviceOwnerDX9;

class CRendererDX9 : public CRenderer {
public:
  explicit CRendererDX9(DeviceOwnerDX9& device);
  ~CRendererDX9() override;
  void SetCamera(const float* eye, const float* target, const float* up) override;
  void SetLighting(const float* direction, const float* diffuse, const float* ambient) override;
  void SetClearColor(unsigned color) override;
  void SetFog(int enabled) override;
  void ClearDepth(int clearColor) override;
  void ClearWorld() override;
  bool Draw() override;
  HRESULT Present(int option) override;
  float FPS() const override;
  bool CaptureBackBuffer() override;
  bool RestoreBackBuffer() override;
  void DrawAuxiliaryPrimitive(LightmappedCommand* primitive) override;
  void DrawLightmappedPrimitive(LightmappedPrimitive* primitive) override;
  void DrawPrimitive(RenderCommand* primitive) override;
  bool RenderToTexture(int width, int height, const std::list<RenderPrimitive*>& primitives, CTexture*& texture) override;
  bool SaveBitmap(const char* filename, CBitmapRes* overlay) override;
  bool SaveJpeg(const char* filename, CBitmapRes* overlay) override;
  void CleanupTextures() override;
  bool SaveLogo(std::string filename) override;
  int LockBackBuffer() override;
  HRESULT UnlockBackBuffer() override;

  void SetTextureMode(int mode);
  void DrawDepthQueue(bool sort, std::vector<DepthPrimitive>& queue);
  void DrawOpaqueQueues(RenderQueues& queues);
  void DrawGroundQueues();
  void DrawAlphaQueues(RenderQueues& queues, unsigned char twoPass);
  void DrawAuxiliaryQueue();
  void DrawBaseQueue();
  void DrawQueues();
  void ApplyFog();
  bool SaveScreenshot(const char* filename, int format, CTexture* overlay, const tagRECT* rectangle);
  void Submit(RenderCommand* command, std::uint16_t flags);
  void Submit(LightmappedPrimitive* command, int queue);

  /* 025C */ DeviceOwnerDX9* device;
  /* 0260 */ IDirect3DDevice9* nativeDevice;
  /* 0264 */ unsigned clearColor;
  /* 0268 */ unsigned lastTick;
  /* 026C */ unsigned frames;
  /* 0270 */ float fps;

private:
  bool SaveBitmapResource(const char* filename, int format, CBitmapRes* overlay);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRendererDX9) == 0x274);
#endif

#endif
