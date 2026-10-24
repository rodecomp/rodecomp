#ifndef RAGNAROK_SOURCE_RENDER_BITMAPTEXTURECACHE_H
#define RAGNAROK_SOURCE_RENDER_BITMAPTEXTURECACHE_H

#include <cstdint>
#include <list>

struct BitmapPixels;
class CTexture;
class CRenderSystemBase;

// CDCBitmap's contiguous list/alpha subobject at +32/+40 on x86.
class BitmapTextureCache {
public:
  BitmapTextureCache();
  ~BitmapTextureCache();
  void Clear();
  void Update(BitmapPixels& bitmap, CRenderSystemBase& system);
  void Draw(BitmapPixels& bitmap, CRenderSystemBase& system, int x, int y, int width, int height, std::uint32_t color);

protected:
  friend class CDCBitmap;
  std::list<CTexture*> m_textures;
  bool m_alpha;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BitmapTextureCache) == 12);
#endif

#endif
