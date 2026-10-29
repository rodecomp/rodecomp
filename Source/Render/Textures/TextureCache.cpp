#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Movies/BinkMovies.h"
#include "Contents/World/World.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/RenderSystemBase.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"

#include <cstring>
#include <string>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24639
int g_textureReduction = 1;

// GLOBAL: gl_24638
TextureCache g_texMgr;

// INLINED
bool TextureNameLess::operator()(const char* left, const char* right) const {
  return std::strcmp(left, right) < 0;
}

// OFFSET: 0x004074F5
// FUNCTION: fn_00152
TextureCache::TextureCache() {
}

// OFFSET: 0x005713C0
// FUNCTION: fn_02447
TextureCache::~TextureCache() {
  this->Clear();
}

// OFFSET: 0x00571790
// FUNCTION: fn_02453
CTexture* TextureCache::Default() {
  return GetRenderSystemHolder()->BackBufferTexture();
}

// OFFSET: 0x005715E0
// FUNCTION: fn_02449
CTexture* TextureCache::Upload(int width, int height, const std::uint32_t* pixels, int format, int option, bool fullSize, bool dynamic, bool renderTarget) {
  const int outputWidth = fullSize ? width : width / g_textureReduction;
  const int outputHeight = fullSize ? height : height / g_textureReduction;
  CTexture* texture = GetRenderSystemHolder()->CreateTexture(outputWidth, outputHeight, format, dynamic, renderTarget);

  if (texture) {
    texture->Upload(0, 0, width, height, pixels, option, 0);
    texture->unknown_0008 = (texture->unknown_0008 & 0xFF00u) | fullSize;
  }

  return texture;
}

// OFFSET: 0x00571660
// FUNCTION: fn_02450
CTexture* CreateTexture(std::uint32_t width, std::uint32_t height, std::int32_t format, bool dynamic, bool renderTarget) {
  return GetRenderSystemHolder()->CreateTexture(width, height, format, dynamic, renderTarget);
}

// OFFSET: 0x00571930
// FUNCTION: fn_02455
CTexture* TextureCache::Create(const char* name, int option, bool fullSize, bool dynamic, bool renderTarget) {
  if (std::strstr(name, ".bik")) {
    if (g_modeMgr.GetGameMode() && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene) {
      BinkMovies* movies = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->movies;

      if (movies) {
        NamedBinkMovie* movie = movies->Get(std::string(name), 1);

        if (movie) {
          const auto* handle = movie->decoder->Handle();
          CTexture* texture = GetRenderSystemHolder()->CreateTexture(handle->width, handle->height, TextureFormat::A8R8G8B8, false, false);

          if (texture) {
            return texture;
          }
        }
      }
    }
  } else {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));

    if (bitmap) {
      const auto& image = bitmap->image;
      CTexture* texture = this->Upload(
        image.width, image.height, image.pixels, image.alpha != 0 ? TextureFormat::A4R4G4B4 : TextureFormat::A1R5G5B5, option, fullSize, dynamic, renderTarget
      );
      ResourceMgr::Instance().Remove(bitmap);

      return texture;
    }
  }

  const int extent = 1 / g_textureReduction;
  CTexture* texture = GetRenderSystemHolder()->CreateTexture(extent, extent, TextureFormat::A1R5G5B5, false, false);

  if (texture) {
    const std::uint32_t transparent = 0;
    texture->Upload(0, 0, 1, 1, &transparent, option, 0);
    texture->unknown_0008 &= 0xFF00u;
  }

  return texture;
}

// OFFSET: 0x005717A0
// FUNCTION: fn_02454
CTexture* TextureCache::Load(const char* name, int option, bool fullSize, bool dynamic, bool renderTarget) {
  const auto found = this->m_textures.find(name);

  if (found != this->m_textures.end() && found->second) {
    found->second->unknown_0020 = timeGetTime();
    return found->second;
  }

  CTexture* texture = this->Create(name, option, fullSize, dynamic, renderTarget);

  if (!texture) {
    return this->Default();
  }

  strncpy_s(texture->m_name, sizeof(texture->m_name), name, _TRUNCATE);
  texture->unknown_0020 = timeGetTime();
  this->m_textures.emplace(texture->m_name, texture);

  return texture;
}

// OFFSET: 0x005716F0
// FUNCTION: fn_02452
void TextureCache::Remove(CTexture* texture) {
  if (texture->unknown_001C == 0 && texture != this->Default()) {
    const auto found = this->m_textures.find(texture->m_name);

    if (found != this->m_textures.end()) {
      delete found->second;
      this->m_textures.erase(found);
    }
  }
}

// OFFSET: 0x00571670
// FUNCTION: fn_02451
void TextureCache::Clear() {
  for (auto& [name, texture] : this->m_textures) {
    delete texture;
  }

  this->m_textures.clear();
}

// OFFSET: 0x00571B70
// FUNCTION: fn_02456
void TextureCache::Reload() {
  for (auto& [name, texture] : this->m_textures) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));

    if (!bitmap) {
      continue;
    }

    const auto& image = bitmap->image;

    if (texture) {
      if ((texture->unknown_0008 & 0xFF) == 1) {
        texture->Create(image.width, image.height, texture->m_format);
      } else {
        texture->Create(image.width / g_textureReduction, image.height / g_textureReduction, texture->m_format);
      }

      texture->Upload(0, 0, image.width, image.height, image.pixels, 0, 0);
    }

    ResourceMgr::Instance().Remove(bitmap);
  }
}

// OFFSET: 0x00571C50
// FUNCTION: fn_02457
void TextureCache::ClearUnused() {
  const unsigned threshold = timeGetTime() - g_resourceReuseDelay;

  for (auto entry = this->m_textures.begin(); entry != this->m_textures.end();) {
    CTexture* texture = entry->second;

    if (texture->unknown_001C != 0 || texture->unknown_0020 >= threshold) {
      ++entry;
    } else {
      delete texture;
      entry = this->m_textures.erase(entry);
    }
  }

  HeapCompact(GetProcessHeap(), 0);

  GetRenderSystemHolder()->RestoreDisplayMode();
}
