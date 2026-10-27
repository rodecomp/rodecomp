#ifndef RAGNAROK_SOURCE_RENDER_TEXTURES_SPRITEATLAS_H
#define RAGNAROK_SOURCE_RENDER_TEXTURES_SPRITEATLAS_H

#include "Render/Textures/SpritePixels.h"

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

class CTexture;
class CRenderSystemBase;
struct SpriteAtlasPage;

struct SpriteAtlasRecord {
  /* 0000 */ const SpriteRleImage* image;
  /* 0004 */ const std::uint32_t* palette;
  /* 0008 */ std::uint32_t lastUsed;
  /* 000C */ float u0;
  /* 0010 */ float v0;
  /* 0014 */ float u1;
  /* 0018 */ float v1;
  /* 001C */ std::uint32_t x;
  /* 0020 */ std::uint32_t y;
  /* 0024 */ SpriteAtlasPage* page;
};

struct SpriteAtlasPage {
  /* 0000 */ CTexture* texture;
  /* 0004 */ std::uint32_t cellWidth;
  /* 0008 */ std::uint32_t cellHeight;
  /* 000C */ std::vector<SpriteAtlasRecord> records;
  /* 0018 */ std::uint32_t lastUsed;
};

class SpriteAtlas {
public:
  SpriteAtlas();
  ~SpriteAtlas();
  void Initialize(CRenderSystemBase& system, unsigned spareCount);
  void Clear();
  CTexture* Find(const SpriteRleImage* image, const std::uint32_t* palette, SpriteAtlasRecord& output);
  CTexture* Insert(
    CRenderSystemBase& system, const SpriteRleImage* image, const std::uint32_t* palette, SpriteAtlasRecord& output, int serviceType, std::uint32_t reuseDelay
  );

private:
  struct Node {
    /* 0000 */ Node* next;
    /* 0004 */ SpriteAtlasPage page;
  };
  struct Key {
    /* 0000 */ const SpriteRleImage* image;
    /* 0004 */ const std::uint32_t* palette;
    bool operator==(const Key&) const = default;
  };
  struct Hash {
    std::size_t operator()(const Key& key) const;
  };
  SpriteAtlasPage* Allocate(CRenderSystemBase& system, unsigned category, unsigned width, unsigned height);
  void ClearSpare();
  /* 0000 */ Node* m_classes[25];
  /* 0064 */ std::unordered_map<Key, SpriteAtlasRecord*, Hash> m_lookup;
  /* 0084 */ std::vector<CTexture*> m_spare;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SpriteAtlasRecord) == 40);
static_assert(offsetof(SpriteAtlasRecord, palette) == 0x04);
static_assert(offsetof(SpriteAtlasRecord, lastUsed) == 0x08);
static_assert(offsetof(SpriteAtlasRecord, u0) == 0x0C);
static_assert(offsetof(SpriteAtlasRecord, v0) == 0x10);
static_assert(offsetof(SpriteAtlasRecord, u1) == 0x14);
static_assert(offsetof(SpriteAtlasRecord, v1) == 0x18);
static_assert(offsetof(SpriteAtlasRecord, x) == 0x1C);
static_assert(offsetof(SpriteAtlasRecord, y) == 0x20);
static_assert(offsetof(SpriteAtlasRecord, page) == 0x24);
static_assert(sizeof(SpriteAtlasPage) == 28);
static_assert(offsetof(SpriteAtlasPage, cellWidth) == 0x04);
static_assert(offsetof(SpriteAtlasPage, cellHeight) == 0x08);
static_assert(offsetof(SpriteAtlasPage, records) == 0x0C);
static_assert(offsetof(SpriteAtlasPage, lastUsed) == 0x18);
static_assert(sizeof(SpriteAtlas) == 144);
#endif

#endif
