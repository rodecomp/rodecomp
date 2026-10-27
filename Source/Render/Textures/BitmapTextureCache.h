#ifndef RAGNAROK_SOURCE_RENDER_TEXTURES_BITMAPTEXTURECACHE_H
#define RAGNAROK_SOURCE_RENDER_TEXTURES_BITMAPTEXTURECACHE_H

#include <cstdint>
#include <list>

struct BitmapPixels;
class CTexture;
class CRenderSystemBase;

class BitmapTextureCache {
public:
  BitmapTextureCache();
  ~BitmapTextureCache();
  void Clear();
  void Update(BitmapPixels& bitmap, CRenderSystemBase& system);
  void Draw(BitmapPixels& bitmap, CRenderSystemBase& system, int x, int y, int width, int height, std::uint32_t color);

protected:
  friend class CDCBitmap;
  /* 0020 */ std::list<CTexture*> m_textures;
  /* 0028 */ bool m_alpha;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BitmapTextureCache) == 12);
#endif

#endif
