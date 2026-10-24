#ifndef RAGNAROK_SOURCE_RENDER_DCBITMAP_H
#define RAGNAROK_SOURCE_RENDER_DCBITMAP_H

#include "Render/BitmapPixels.h"
#include "Render/BitmapTextureCache.h"
#include "Render/DeviceContext.h"

class CDCBitmap : public CDC {
public:
  CDCBitmap(int width, int height);
  ~CDCBitmap() override;
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
  void UploadTexture(CRenderSystemBase& system, CTexture*& texture);
  void SetAlpha(bool alpha) {
    this->m_cache.m_alpha = alpha;
  }

protected:
  /* 0004 */ BitmapPixels m_bitmap;
  /* 0020 */ BitmapTextureCache m_cache;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CDCBitmap) == 44);
#endif

CDCBitmap* CreateBitmapDC(int width, int height, bool fill);

#endif
