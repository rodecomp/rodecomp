#include "Render/BitmapPixels.h"
#include "Render/BitmapTextureCache.h"
#include "Render/RenderSystemBase.h"
#include "Render/Texture.h"

#include <algorithm>

// OFFSET: 0x00545790
// FUNCTION: fn_01832
BitmapTextureCache::BitmapTextureCache() : m_alpha(false) {
}

// OFFSET: 0x00545B20
// FUNCTION: fn_01836
BitmapTextureCache::~BitmapTextureCache() {
  this->Clear();
}

// OFFSET: 0x00547A80
// FUNCTION: fn_01845
void BitmapTextureCache::Clear() {
  for (CTexture* texture : this->m_textures) {
    delete texture;
  }
  this->m_textures.clear();
}

// OFFSET: 0x00548380
// FUNCTION: fn_01854
void BitmapTextureCache::Update(BitmapPixels& bitmap, CRenderSystemBase& system) {
  if (!bitmap.dirty) {
    return;
  }
  if (this->m_alpha) {
    if (this->m_textures.empty()) {
      CTexture* texture = system.CreateTexture(bitmap.width, bitmap.height, 4, false, false);
      if (texture) {
        this->m_textures.push_back(texture);
      }
    }
    for (CTexture* texture : this->m_textures) {
      texture->Upload(0, 0, bitmap.width, bitmap.height, bitmap.pixels, 0, bitmap.width);
    }
  } else {
    if (this->m_textures.empty()) {
      for (unsigned y = 0; y < bitmap.height; y += 256) {
        for (unsigned x = 0; x < bitmap.width; x += 256) {
          this->m_textures.push_back(
            system.CreateTexture(std::min(256u, bitmap.width - x), std::min(256u, bitmap.height - y), 0, false, false)
          );
        }
      }
    }
    const std::int32_t bytes = static_cast<std::int32_t>(4u * bitmap.width * bitmap.height);
    for (std::int32_t offset = 0; offset < bytes; offset += 4) {
      reinterpret_cast<unsigned char*>(bitmap.pixels)[offset + 3] = 255;
    }
    auto texture = this->m_textures.begin();
    for (unsigned y = 0; y < bitmap.height; y += 256) {
      for (unsigned x = 0; x < bitmap.width; x += 256) {
        (*texture++)
          ->Upload(
            0, 0, std::min(256u, bitmap.width - x), std::min(256u, bitmap.height - y),
            bitmap.pixels + x + y * bitmap.width, 0, bitmap.width
          );
      }
    }
  }
  bitmap.dirty = false;
}

// OFFSET: 0x00547B80
// FUNCTION: fn_01846
void BitmapTextureCache::Draw(
  BitmapPixels& bitmap,
  CRenderSystemBase& system,
  int x,
  int y,
  int width,
  int height,
  std::uint32_t color
) {
  if (!bitmap.pixels) {
    return;
  }
  this->Update(bitmap, system);
  if (this->m_textures.empty()) {
    return;
  }
  if (this->m_alpha) {
    for (CTexture* texture : this->m_textures) {
      texture->Draw(x, y, width, height, color);
    }
  } else {
    auto texture = this->m_textures.begin();
    int remainingHeight = height;
    for (int row = 0; row < height; row += 256, remainingHeight -= 256) {
      int remainingWidth = width;
      for (int column = 0; column < width; column += 256, remainingWidth -= 256) {
        (*texture++)->Draw(x + column, y + row, std::min(256, remainingWidth), std::min(256, remainingHeight), color);
      }
    }
  }
}
