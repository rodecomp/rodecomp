#ifndef RAGNAROK_SOURCE_RENDER_TEXTURES_TEXTURECACHE_H
#define RAGNAROK_SOURCE_RENDER_TEXTURES_TEXTURECACHE_H

#include <cstdint>
#include <map>

class CTexture;

struct TextureNameLess {
  bool operator()(const char* left, const char* right) const;
};

class TextureCache {
public:
  TextureCache();
  ~TextureCache();
  CTexture* Load(const char* name, int option, bool fullSize, bool dynamic, bool renderTarget);
  CTexture* Default();
  CTexture* Create(const char* name, int option, bool fullSize, bool dynamic, bool renderTarget);
  CTexture* Upload(int width, int height, const std::uint32_t* pixels, int format, int option, bool fullSize, bool dynamic, bool renderTarget);
  void Remove(CTexture* texture);
  void Clear();
  void ClearUnused();
  void Reload();

private:
  /* 0000 */ std::map<const char*, CTexture*, TextureNameLess> m_textures;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TextureCache) == 8);
#endif

extern TextureCache g_texMgr;
extern int g_textureReduction;

CTexture* CreateTexture(std::uint32_t width, std::uint32_t height, std::int32_t format, bool dynamic, bool renderTarget);

#endif
