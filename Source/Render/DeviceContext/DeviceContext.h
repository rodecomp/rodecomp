#ifndef RAGNAROK_SOURCE_RENDER_DEVICECONTEXT_DEVICECONTEXT_H
#define RAGNAROK_SOURCE_RENDER_DEVICECONTEXT_DEVICECONTEXT_H

#include "Render/Textures/SpritePixels.h"

#include <windows.h>

class CSurface;
class CDCBitmap;
class CRenderSystemBase;
class SpriteAtlas;

class CDC {
public:
  virtual ~CDC();
  virtual void Grow(std::uint32_t width, std::uint32_t height) = 0;
  virtual bool GetDC(HDC* output) = 0;
  virtual void ReleaseDC(HDC dc) = 0;
  virtual void Copy(int x, int y, int width, int height, const std::uint32_t* source, bool transparent) = 0;
  virtual void ScaleCopy(int x, int y, int sourceWidth, int sourceHeight, int width, int height, const std::uint32_t* source, int option) = 0;
  virtual void CopyRegion(
    int x, int y, int sourceLeft, int sourceTop, int sourceRight, int sourceBottom, const std::uint32_t* source, int sourceWidth, int sourceHeight,
    bool transparent, bool alpha
  ) = 0;
  virtual void CopyFrom(int x, int y, int sourceX, int sourceY, int width, int height, const CDCBitmap& source) = 0;
  virtual void BlendFrom(std::uint32_t x, std::uint32_t y, int width, int height, const CDCBitmap& source) = 0;
  virtual void DrawRleSprite(
    int x, int y, std::span<const SpriteFrameCell> cells, const SpriteRleImage* const* const* images, const std::uint32_t* palette, int cellCount
  ) = 0;
  virtual void DrawSurface(
    int x, int y, CSurface* source, int sourceX, int sourceY, int sourceWidth, int sourceHeight, bool mirrored, int scaleX, int scaleY, int filter, int format
  ) = 0;
  virtual void Fill(const RECT* rectangle, std::uint32_t color) = 0;
  virtual void Draw(CRenderSystemBase& system, int x, int y, int width, int height, std::uint32_t color) = 0;
  virtual void DrawScaledSprite(
    int x, int y, int width, int height, std::span<const SpriteFrameCell> cells, const SpriteRleImage* const* const* images, int cellCount, int offsetX,
    int offsetY, float scale, const std::uint32_t* palette, SpriteAtlas& atlas, CRenderSystemBase& system, int serviceType, std::uint32_t reuseDelay
  ) = 0;
  virtual void DrawScaledSurface(
    int x, int y, int width, int height, CSurface* source, int sourceWidth, int sourceHeight, float scaleX, float scaleY, std::uint32_t color, bool mirrored,
    int offsetX, int offsetY, int sourceX, int sourceY, int format
  ) = 0;
  virtual void Brighten(int x, int y, int width, int height, int amount) = 0;
  virtual void Dim(int x, int y, int width, int height, int percent) = 0;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CDC) == 4);
#endif

#endif
