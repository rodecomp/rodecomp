#ifndef RAGNAROK_SOURCE_RENDER_DX9_RENDERERDX9_H
#define RAGNAROK_SOURCE_RENDER_DX9_RENDERERDX9_H

#include "Render/Renderer.h"
#include "Render/DX9/RendererDX9Operations.h"
#include "Render/DX9/RendererPresentation.h"
#include "Render/DX9/TextureDX9.h"

class DeviceOwnerDX9;
class D3DX9Runtime;
class TextureCache;
class TextureSources;
struct EffectRetirement;
struct ServiceOptions;

struct RendererDX9Context {
  CRenderSystemBase& system;
  D3DX9Runtime& d3dx;
  TextureCache& textures;
  TextureSources& sources;
  EffectRetirement& retirement;
  const ServiceOptions& options;
  const unsigned char& multitexture;
  FogConfiguration& fog;
  const unsigned char& worldFlag;
  const unsigned& overlayColor;
  RenderDiagnostic& diagnostic;
};

class CRendererDX9 : public CRenderer {
public:
  CRendererDX9(DeviceOwnerDX9& device, RendererDX9Context& context);
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
  void DrawAuxiliaryPrimitive(LightmappedPrimitive* primitive) override;
  void DrawLightmappedPrimitive(LightmappedPrimitive* primitive) override;
  void DrawPrimitive(RenderCommand* primitive) override;
  bool RenderToTexture(int width, int height, const std::list<RenderPrimitive*>& primitives, CTexture*& texture) override;
  bool SaveBitmap(const char* filename, CBitmapRes* overlay) override;
  bool SaveJpeg(const char* filename, CBitmapRes* overlay) override;
  void CleanupTextures() override;
  bool SaveLogo(std::string filename) override;
  bool LockBackBuffer() override;
  HRESULT UnlockBackBuffer() override;

  TextureDX9Context& TextureContext();
  void Submit(RenderCommand* command, std::uint16_t flags);
  void Submit(LightmappedPrimitive* command, int queue);

  /* 025C */ DeviceOwnerDX9* device;
  /* 0260 */ IDirect3DDevice9* nativeDevice;
  /* 0264 */ unsigned clearColor;
  /* 0268 */ unsigned lastTick;
  /* 026C */ unsigned frames;
  /* 0270 */ float fps;

private:
  RendererDX9Context* m_context;
  TextureDX9Context m_textureContext;
  RendererDX9Operations Operations(TextureDX9Context& textureContext);
  bool SaveBitmapResource(const char* filename, int format, CBitmapRes* overlay);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CRendererDX9) == 0x3D4);
#endif

#endif
