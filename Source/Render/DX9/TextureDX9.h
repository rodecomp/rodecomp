#ifndef RAGNAROK_SOURCE_RENDER_DX9_TEXTUREDX9_H
#define RAGNAROK_SOURCE_RENDER_DX9_TEXTUREDX9_H

#include "Render/Textures/Texture.h"

#include <d3d9.h>

class D3DX9Runtime;
struct RenderQueues;
class RenderPrimitivePool;

struct TextureDX9Context {
  IDirect3D9* direct3D;
  IDirect3DDevice9* device;
  UINT adapter;
  D3DFORMAT displayFormat;
  D3DCAPS9 caps;
  bool dynamicTextures;
  D3DX9Runtime* d3dx;
  RenderPrimitivePool* primitives;
  RenderQueues* queues;
  bool depthAlpha;
  float depthStart;
  float depthEnd;
};

class CTextureDX9 : public CTexture {
public:
  explicit CTextureDX9(TextureDX9Context& context);
  CTextureDX9(TextureDX9Context& context, int width, int height, int format, bool dynamic, bool renderTarget);

  CTextureDX9(TextureDX9Context& context, IDirect3DTexture9* texture);
  ~CTextureDX9() override;
  bool Resize(int width, int height) override;
  SurfaceLock Lock() override;
  void Unlock() override;
  bool GetDC(HDC* output) override;
  void ReleaseDC(HDC dc) override;
  void Upload(int x, int y, int width, int height, const std::uint32_t* pixels, int option, int sourceWidth) override;
  void Fill(const RECT* rectangle, std::uint32_t color) override;
  void Draw(int x, int y, int width, int height, std::uint32_t color) override;
  void Stretch(int x, int y, int width, int height) override;
  void Update(const RECT* rectangle) override;
  void UploadSprite(int x, int y, int width, int height, const SpriteRleImage* image, const std::uint32_t* palette) override;
  bool CopyTexture(CTexture* source, int sourceX, int sourceY, int sourceWidth, int sourceHeight, int x, int y, int width, int height) override;
  bool Create(int width, int height, int format) override;
  // INLINED
  IDirect3DTexture9* NativeTexture() const {
    return this->m_texture;
  }

protected:
  /* 0124 */ TextureDX9Context* m_context;
  /* 0128 */ bool m_renderTargetUsage;
  /* 0129 */ bool m_dynamic;
  /* 012A */ bool m_allowDynamic;
  /* 012B */ bool m_renderTarget;
  /* 012C */ IDirect3DTexture9* m_texture;
  /* 0130 */ IDirect3DSurface9* m_surface;
  /* 0134 */ D3DSURFACE_DESC m_description;

private:
  void Filter();
  void ClearSurface(IDirect3DSurface9* surface, std::uint32_t color);
  bool UploadSurface(IDirect3DSurface9* surface, int x, int y, int width, int height, const std::uint32_t* pixels, bool blackTransparent, int sourceWidth);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CTextureDX9) == 340);
#endif

#endif
