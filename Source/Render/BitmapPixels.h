#ifndef RAGNAROK_SOURCE_RENDER_BITMAPPIXELS_H
#define RAGNAROK_SOURCE_RENDER_BITMAPPIXELS_H

#include "Render/SpritePixels.h"

#include <cstdint>
#include <windows.h>

class CSurface;
class SpriteAtlas;
class CRenderSystemBase;
class CTexture;

// Reconstructed contiguous CDCBitmap subobject at +4..+28 on x86. Texture
// cache invalidation belongs to the enclosing CDCBitmap, not this DIB owner.
struct BitmapPixels {
  /* 0000 */ std::uint32_t width;
  /* 0004 */ std::uint32_t height;
  /* 0008 */ HDC dc;
  /* 000C */ HBITMAP bitmap;
  /* 0010 */ HGDIOBJ previous;
  /* 0014 */ std::uint32_t* pixels;
  /* 0018 */ bool dirty;

  BitmapPixels(int width, int height);
  ~BitmapPixels();
  void Grow(std::uint32_t width, std::uint32_t height);
  bool GetDC(HDC* output);
  void ReleaseDC(HDC dc);
  void Fill(const RECT* rectangle, std::uint32_t color);
  void Copy(int x, int y, int width, int height, const std::uint32_t* source, bool transparent);
  void CopyFrom(int x, int y, int sourceX, int sourceY, int width, int height, const BitmapPixels& source);
  void BlendFrom(std::uint32_t x, std::uint32_t y, int width, int height, const BitmapPixels& source, bool alpha);
  void Brighten(int x, int y, int width, int height, int amount);
  void Dim(int x, int y, int width, int height, int percent);
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
  );
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
  );
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
  );
  void DrawRleSprite(
    int x,
    int y,
    std::span<const SpriteFrameCell> cells,
    const SpriteRleImage* const* const* images,
    const std::uint32_t* palette,
    int cellCount
  );
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
  );
  void ScaleCopy(
    int x,
    int y,
    int sourceWidth,
    int sourceHeight,
    int width,
    int height,
    const std::uint32_t* source,
    int option
  );
  void UploadTexture(CRenderSystemBase& system, CTexture*& texture);

private:
  void Create(int width, int height);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BitmapPixels) == 28);
#endif

#endif
