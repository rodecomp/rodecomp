#ifndef RAGNAROK_SOURCE_RENDER_TEXTURES_BITMAPTEXTURECACHE_H
#define RAGNAROK_SOURCE_RENDER_TEXTURES_BITMAPTEXTURECACHE_H

#include "Render/Textures/Texture.h"

#include <cstdint>
#include <list>

struct BitmapPixels;
class CRenderSystemBase;

class BitmapTextureCache {
public:
  BitmapTextureCache();
  ~BitmapTextureCache();
  void Clear();

protected:
  friend class CDCBitmap;
  /* 0020 */ std::list<CTexture*> m_textures;
  /* 0028 */ bool m_alpha;
};

// INLINED
inline void BitmapTextureCache::Clear() {
  for (CTexture* texture : this->m_textures) {
    delete texture;
  }

  this->m_textures.clear();
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BitmapTextureCache) == 12);
#endif

#endif
