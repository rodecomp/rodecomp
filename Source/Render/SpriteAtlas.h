#ifndef RAGNAROK_SOURCE_RENDER_SPRITEATLAS_H
#define RAGNAROK_SOURCE_RENDER_SPRITEATLAS_H

#include "Render/SpritePixels.h"

#include <cstdint>
#include <unordered_map>
#include <vector>

class CTexture;
class CRenderSystemBase;
struct SpriteAtlasPage;

struct SpriteAtlasRecord {
  const SpriteRleImage* image;
  const std::uint32_t* palette;
  std::uint32_t lastUsed;
  float u0;
  float v0;
  float u1;
  float v1;
  std::uint32_t x;
  std::uint32_t y;
  SpriteAtlasPage* page;
};

struct SpriteAtlasPage {
  CTexture* texture;
  std::uint32_t cellWidth;
  std::uint32_t cellHeight;
  std::vector<SpriteAtlasRecord> records;
  std::uint32_t lastUsed;
};

// Original renderer subobject+192:25 singly-linked page classes, native
// image/palette hash map and spare textures. Context is an original caller input.
class SpriteAtlas {
public:
  SpriteAtlas();
  ~SpriteAtlas();
  void Initialize(CRenderSystemBase& system, unsigned spareCount);
  void Clear();
  CTexture* Find(const SpriteRleImage* image, const std::uint32_t* palette, SpriteAtlasRecord& output);
  CTexture* Insert(
    CRenderSystemBase& system,
    const SpriteRleImage* image,
    const std::uint32_t* palette,
    SpriteAtlasRecord& output,
    int serviceType,
    std::uint32_t reuseDelay
  );

private:
  struct Node {
    Node* next;
    SpriteAtlasPage page;
  };
  struct Key {
    const SpriteRleImage* image;
    const std::uint32_t* palette;
    bool operator==(const Key&) const = default;
  };
  struct Hash {
    std::size_t operator()(const Key& key) const;
  };
  SpriteAtlasPage* Allocate(CRenderSystemBase& system, unsigned category, unsigned width, unsigned height);
  void ClearSpare();
  Node* m_classes[25];
  std::unordered_map<Key, SpriteAtlasRecord*, Hash> m_lookup;
  std::vector<CTexture*> m_spare;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SpriteAtlasRecord) == 40);
static_assert(sizeof(SpriteAtlasPage) == 28);
static_assert(sizeof(SpriteAtlas) == 144);
#endif

#endif
