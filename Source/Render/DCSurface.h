#ifndef RAGNAROK_SOURCE_RENDER_DCSURFACE_H
#define RAGNAROK_SOURCE_RENDER_DCSURFACE_H

#include "Render/DeviceContext.h"
#include "Render/RenderSystemBase.h"

#include <memory>

class CTexture;

std::shared_ptr<CSurface> AcquireBackBufferSurface(CRenderSystemBase* system);

struct ScreenDimensions {
  int height;
  int width;
};

inline ScreenDimensions DefaultResolution() {
  return ScreenDimensions{768, 1024};
}

struct RenderSystemHolder {
  CRenderSystemBase* system;

  std::int32_t Backend() const {
    return this->system ? this->system->Backend() : -1;
  }
  CTexture* BackBufferTexture() const {
    return this->system ? this->system->BackBufferTexture() : nullptr;
  }
  std::uint32_t Width() const {
    return this->system ? this->system->Width() : 0;
  }
  std::uint32_t Height() const {
    return this->system ? this->system->Height() : 0;
  }
  bool IsWindowed() const {
    return this->system ? this->system->IsWindowed() : false;
  }
  HRESULT TestDevice() const {
    return this->system ? this->system->TestDevice() : static_cast<HRESULT>(0x8000000E);
  }
  HRESULT Present(bool option) const {
    return this->system ? this->system->Present(option) : static_cast<HRESULT>(0x8000000E);
  }
};

__declspec(noinline) inline RenderSystemHolder* GetRenderSystemHolder() {
  static RenderSystemHolder holder;
  return &holder;
}

// RTTI CDCSurface. Owns a shared surface reference.
// Unused virtuals are established empty original bodies, not bring-up stubs.
class CDCSurface : public CDC {
public:
  explicit CDCSurface(std::shared_ptr<CSurface> surface);
  ~CDCSurface() override;
  void Grow(std::uint32_t width, std::uint32_t height) override;
  bool GetDC(HDC* output) override;
  void ReleaseDC(HDC dc) override;
  void Copy(int x, int y, int width, int height, const std::uint32_t* source, bool transparent) override;
  void ScaleCopy(
    int x,
    int y,
    int sourceWidth,
    int sourceHeight,
    int width,
    int height,
    const std::uint32_t* source,
    int option
  ) override;
  void CopyRegion(
    int x,
    int y,
    int sourceLeft,
    int sourceTop,
    int sourceRight,
    int sourceBottom,
    const std::uint32_t* source,
    int sourceWidth,
    int sourceHeight,
    bool transparent,
    bool alpha
  ) override;
  void CopyFrom(int x, int y, int sourceX, int sourceY, int width, int height, const CDCBitmap& source) override;
  void BlendFrom(std::uint32_t x, std::uint32_t y, int width, int height, const CDCBitmap& source) override;
  void DrawRleSprite(
    int x,
    int y,
    std::span<const SpriteFrameCell> cells,
    const SpriteRleImage* const* const* images,
    const std::uint32_t* palette,
    int cellCount
  ) override;
  void DrawSurface(
    int x,
    int y,
    CSurface* source,
    int sourceX,
    int sourceY,
    int sourceWidth,
    int sourceHeight,
    bool mirrored,
    int scaleX,
    int scaleY,
    int filter,
    int format
  ) override;
  void Fill(const RECT* rectangle, std::uint32_t color) override;
  void Draw(CRenderSystemBase& system, int x, int y, int width, int height, std::uint32_t color) override;
  void DrawScaledSprite(
    int x,
    int y,
    int width,
    int height,
    std::span<const SpriteFrameCell> cells,
    const SpriteRleImage* const* const* images,
    int cellCount,
    int offsetX,
    int offsetY,
    float scale,
    const std::uint32_t* palette,
    SpriteAtlas& atlas,
    CRenderSystemBase& system,
    int serviceType,
    std::uint32_t reuseDelay
  ) override;
  void DrawScaledSurface(
    int x,
    int y,
    int width,
    int height,
    CSurface* source,
    int sourceWidth,
    int sourceHeight,
    float scaleX,
    float scaleY,
    std::uint32_t color,
    bool mirrored,
    int offsetX,
    int offsetY,
    int sourceX,
    int sourceY,
    int format
  ) override;
  void Brighten(int x, int y, int width, int height, int amount) override;
  void Dim(int x, int y, int width, int height, int percent) override;

private:
  std::shared_ptr<CSurface> m_surface;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CDCSurface) == 12);
#endif

#endif
