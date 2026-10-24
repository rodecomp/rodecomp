#ifndef RAGNAROK_SOURCE_RENDER_TEXTURECACHE_H
#define RAGNAROK_SOURCE_RENDER_TEXTURECACHE_H

#include "Resource/EZeffectResource.h"

#include <map>

class ResourceMgr;
class CRenderSystemBase;

// Name override and movie lookup remain
// real owner dependencies. Movie success supplies its current frame extent;
// no decoded pixels are copied by original texture creation at this point.
class TextureSources {
public:
  virtual const char* Resolve(const char* name) = 0;
  virtual bool MovieSize(const char* name, int& width, int& height) = 0;
  // Renderer virtual slot +0x40: DX7 compacts its device; DX9 is empty.
  virtual void CleanupTextures() = 0;

protected:
  ~TextureSources() = default;
};

struct TextureNameLess {
  bool operator()(const char* left, const char* right) const;
};

// Ordered cache. Keys borrow the texture's inline name;
// the cache owns the textures, while the backend owns the default texture.
class TextureCache : public EffectTextures {
public:
  TextureCache(
    ResourceMgr& resources,
    CRenderSystemBase*& system,
    TextureSources& sources,
    const int& reduction,
    const unsigned& reuseDelay
  );
  ~TextureCache();
  CTexture* Load(const char* name, int option, bool fullSize, bool dynamic, bool renderTarget) override;
  CTexture* Default() override;
  CTexture* Create(const char* name, int option, bool fullSize, bool dynamic, bool renderTarget);
  CTexture* Upload(
    int width,
    int height,
    const std::uint32_t* pixels,
    int format,
    int option,
    bool fullSize,
    bool dynamic,
    bool renderTarget
  );
  void Remove(CTexture* texture);
  void Clear();
  void ClearUnused();

private:
  ResourceMgr& m_resources;
  CRenderSystemBase*& m_system;
  TextureSources& m_sources;
  const int& m_reduction;
  const unsigned& m_reuseDelay;
  std::map<const char*, CTexture*, TextureNameLess> m_textures;
};

#endif
