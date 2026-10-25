#ifndef RAGNAROK_SOURCE_RENDER_DX7_TEXTUREDX7_H
#define RAGNAROK_SOURCE_RENDER_DX7_TEXTUREDX7_H

#include "Render/Textures/Texture.h"
#include "Render/DX7/TextureAllocationDX7.h"

#include <ddraw.h>

class DeviceOwnerDX7;
class CRenderSystemBase;

struct TextureDX7Context {
  DeviceOwnerDX7& owner;
  RenderPrimitivePool& primitives;
  RenderQueues& queues;
  CRenderSystemBase* renderSystem;
  const TextureAllocationDX7& allocation;
  const bool& depthAlpha;
  const float& depthStart;
  const float& depthEnd;
};

class CTextureDX7 : public CTexture {
public:
  explicit CTextureDX7(TextureDX7Context& context);
  CTextureDX7(TextureDX7Context& context, int width, int height, int format);
  ~CTextureDX7() override;
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
  bool CopySurface(IDirectDrawSurface7* source, const RECT* rectangle);
  IDirectDrawSurface7* NativeSurface() const {
    return this->m_surface;
  }

protected:
  /* 0124 */ DeviceOwnerDX7* m_owner;
  /* 0128 */ IDirectDrawSurface7* m_surface;

  TextureDX7Context* m_context;
};

#endif
