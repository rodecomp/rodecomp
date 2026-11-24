#include "Render/DeviceContext/DCBitmap.h"
#include "Render/Textures/BitmapPixels.h"
#include "Render/RenderSystemBase.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

#include <algorithm>

// OFFSET: 0x00548380
// FUNCTION: fn_01854
void CDCBitmap::UpdateCache() {
  const bool alpha = this->m_alpha;
  if (!this->m_bitmap.dirty) {
    return;
  }

  if (alpha == true) {
    if (this->m_textures.empty()) {
      CTexture* texture = g_texMgr.CreateTexture(this->m_bitmap.width, this->m_bitmap.height, TextureFormat::A8R8G8B8, false, false);
      if (texture) {
        this->m_textures.push_back(texture);
      }
    }

    for (auto it = this->m_textures.begin(); it != this->m_textures.end(); ++it) {
      (*it)->Upload(0, 0, this->m_bitmap.width, this->m_bitmap.height, this->m_bitmap.pixels, 0, this->m_bitmap.width);
    }
  } else {
    if (this->m_textures.empty()) {
      for (unsigned y = 0; y < this->m_bitmap.height; y += 256) {
        for (unsigned x = 0; x < this->m_bitmap.width; x += 256) {
          this->m_textures.push_back(
            g_texMgr.CreateTexture(std::min(this->m_bitmap.width - x, 256u), std::min(this->m_bitmap.height - y, 256u), TextureFormat::A1R5G5B5, false, false)
          );
        }
      }
    }

    const std::int32_t count = static_cast<std::int32_t>(this->m_bitmap.width * this->m_bitmap.height * 4);
    unsigned char* image = reinterpret_cast<unsigned char*>(this->m_bitmap.pixels) + 3;
    for (std::int32_t i = 0; i < count; i += 4) {
      image[i] = 255;
    }

    auto texture = this->m_textures.begin();
    for (unsigned y = 0; y < this->m_bitmap.height; y += 256) {
      for (unsigned x = 0; x < this->m_bitmap.width; x += 256) {
        (*texture)->Upload(
          0, 0, this->m_bitmap.width - x > 256 ? 256 : this->m_bitmap.width - x, this->m_bitmap.height - y > 256 ? 256 : this->m_bitmap.height - y,
          this->m_bitmap.pixels + this->m_bitmap.width * y + x, 0, this->m_bitmap.width
        );
        ++texture;
      }
    }
  }

  this->m_bitmap.dirty = false;
}

// OFFSET: 0x00547B80
// FUNCTION: fn_01846
void CDCBitmap::Draw(int x, int y, int width, int height, std::uint32_t color) {
  if (!this->m_bitmap.pixels) {
    return;
  }

  this->UpdateCache();
  if (this->m_textures.empty()) {
    return;
  }

  if (this->m_alpha == true) {
    for (auto texture = this->m_textures.begin(); texture != this->m_textures.end(); ++texture) {
      (*texture)->Draw(x, y, width, height, color);
    }
  } else {
    auto texture = this->m_textures.begin();
    for (int row = 0; row < height; row += 256) {
      for (int column = 0; column < width; column += 256) {
        (*texture)->Draw(x + column, y + row, width - column > 256 ? 256 : width - column, height - row > 256 ? 256 : height - row, color);
        ++texture;
      }
    }
  }
}
