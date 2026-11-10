#ifndef RAGNAROK_SOURCE_RENDER_TEXTURES_BITMAPPIXELS_H
#define RAGNAROK_SOURCE_RENDER_TEXTURES_BITMAPPIXELS_H

#include "Render/Textures/SpritePixels.h"

#include <cstdint>
#include <windows.h>

class CSurface;
class SpriteAtlas;
class CRenderSystemBase;
class CTexture;

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
  void Dim(int x, int y, int width, int height, int percent);
  void CopyRegion(
    int x, int y, int sourceLeft, int sourceTop, int sourceRight, int sourceBottom, const std::uint32_t* source, int sourceWidth, int sourceHeight,
    bool transparent, bool alpha
  );

private:
  void Create(int width, int height);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BitmapPixels) == 28);
#endif

#endif
