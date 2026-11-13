#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/RenderSystemBase.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourceMgr.h"

#include <bit>
#include <windows.h>
#include <timeapi.h>

namespace {
  struct CellSize {
    unsigned width;
    unsigned height;
  };

  // STATIC: 0x010DD520
  // STATIC: 0x010DD524
  // GLOBAL: gl_13421
  constexpr CellSize cellSizes[25] = {
    {32, 32}, {32, 64}, {32, 128}, {32, 256}, {32, 512},
    {64, 32}, {64, 64}, {64, 128}, {64, 256}, {64, 512},
    {128, 32}, {128, 64}, {128, 128}, {128, 256}, {128, 512},
    {256, 32}, {256, 64}, {256, 128}, {256, 256}, {256, 512},
    {512, 32}, {512, 64}, {512, 128}, {512, 256}, {512, 512},
  };

  // INLINED
  unsigned Category(std::int16_t dimension) {
    if (dimension <= 32) {
      return 0;
    }

    if (dimension <= 64) {
      return 1;
    }

    if (dimension <= 128) {
      return 2;
    }

    if (dimension <= 256) {
      return 3;
    }

    return 4;
  }
}

// OFFSET: 0x0056F280
// FUNCTION: fn_02403
SpriteAtlas::~SpriteAtlas() {
  this->Clear();
  this->ClearSpare();
}

// OFFSET: 0x0056EA60
// FUNCTION: fn_02397
std::size_t SpriteAtlas::Hash::operator()(const Key& key) const {
  std::size_t seed = std::hash<const SpriteRleImage*>{}(key.image);
  seed ^= std::hash<const std::uint32_t*>{}(key.palette) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
  return seed;
}

// OFFSET: 0x0056FB00
// FUNCTION: fn_02407
void SpriteAtlas::Initialize(unsigned spareCount) {
  this->Clear();
  this->ClearSpare();
  this->m_spare.reserve(spareCount);

  for (unsigned i = 0; i < spareCount; ++i) {
    CTexture* texture = g_texMgr.CreateTexture(256, 256, TextureFormat::A1R5G5B5, false, false);
    if (!texture) {
      break;
    }

    texture->Retain();
    this->m_spare.push_back(texture);
  }
}

// OFFSET: 0x0056FD70
// FUNCTION: fn_02411
void SpriteAtlas::Clear() {
  for (int i = 0; i < 25; ++i) {
    std::forward_list<SpriteAtlasPage>& pages = this->m_classes[i];
    for (SpriteAtlasPage& page : pages) {
      CTexture* texture = page.texture;
      if (texture->refCount == 0) {
        delete texture;
      } else {
        this->m_spare.push_back(texture);
      }
    }

    pages.clear();
  }

  this->m_lookup.clear();
}

// OFFSET: 0x0056FC40
// FUNCTION: fn_02409
void SpriteAtlas::ClearSpare() {
  for (CTexture*& texture : this->m_spare) {
    texture->Release();
    delete texture;
  }

  this->m_spare.clear();
}

// OFFSET: 0x0056FBB0
// FUNCTION: fn_02408
CTexture* SpriteAtlas::Find(const SpriteRleImage* image, const std::uint32_t* palette, SpriteAtlasRecord& output) {
  auto found = this->m_lookup.find({image, palette});
  if (found != this->m_lookup.end()) {
    SpriteAtlasRecord& record = *found->second;
    record.lastUsed = timeGetTime();
    record.page->lastUsed = record.lastUsed;
    output = record;
    return record.page->texture;
  }

  return nullptr;
}

// OFFSET: 0x0056F830
// FUNCTION: fn_02406
SpriteAtlasPage* SpriteAtlas::Allocate(unsigned category, unsigned width, unsigned height) {
  std::forward_list<SpriteAtlasPage>& pages = this->m_classes[category];
  pages.emplace_front();
  SpriteAtlasPage& page = pages.front();

  CTexture* texture;
  if (width <= 256 && height <= 256) {
    if (this->m_spare.empty()) {
      texture = g_texMgr.CreateTexture(256, 256, TextureFormat::A1R5G5B5, false, false);
    } else {
      texture = this->m_spare.back();
      this->m_spare.pop_back();
    }
  } else {
    texture = g_texMgr.CreateTexture(width, height, TextureFormat::A1R5G5B5, false, false);
  }

  page.texture = texture;
  texture->Fill(nullptr, 0);
  page.cellWidth = cellSizes[category].width;
  page.cellHeight = cellSizes[category].height;
  const int columns = width / page.cellWidth;
  const int rows = height / page.cellHeight;
  page.records.resize(columns * rows);

  for (int y = 0; y < rows; ++y) {
    for (int x = 0; x < columns; ++x) {
      SpriteAtlasRecord& record = page.records[x + y * columns];
      record.x = x * page.cellWidth;
      record.y = y * page.cellHeight;
      record.u0 = static_cast<float>(record.x) / static_cast<float>(width);
      record.v0 = static_cast<float>(record.y) / static_cast<float>(height);
      record.page = &page;
    }
  }

  page.lastUsed = 0;
  return &page;
}

// OFFSET: 0x0056F410
// FUNCTION: fn_02404
CTexture* SpriteAtlas::Insert(const SpriteRleImage* image, const std::uint32_t* palette, SpriteAtlasRecord& output) {
  const std::uint32_t threshold = timeGetTime() - (g_resourceReuseDelay >> 2);
  float width = 256.0f;
  float height = 256.0f;
  if (image->width > 256) {
    width = (image->width / 256 + 1) * 256.0f;
  }

  if (image->height > 256) {
    height = (image->height / 256 + 1) * 256.0f;
  }

  const unsigned category = Category(image->width) * 5 + Category(image->height);
  SpriteAtlasRecord* selected = nullptr;
  bool found = false;

  for (SpriteAtlasPage& candidate : this->m_classes[category]) {
    for (SpriteAtlasRecord& record : candidate.records) {
      if (record.lastUsed < threshold) {
        selected = &record;
        found = true;
        break;
      }
    }

    if (found) {
      break;
    }
  }

  if (found) {
    this->m_lookup.erase({selected->image, selected->palette});
  } else {
    selected = &this->Allocate(category, static_cast<unsigned>(width), static_cast<unsigned>(height))->records[0];
  }

  SpriteAtlasRecord& record = *selected;
  record.image = image;
  record.palette = palette;
  float u1 = static_cast<float>(record.x + image->width) / width;
  if (g_serviceType != 10) {
    record.u1 = u1;
    record.v1 = static_cast<float>(record.y + image->height) / height;
  } else {
    if (image->width > 64) {
      u1 -= 0.005f;
    }

    record.u1 = u1;
    float v1 = static_cast<float>(record.y + image->height) / height;
    if (image->height > 64) {
      v1 -= 0.005f;
    }

    record.v1 = v1;
  }

  SpriteAtlasPage* page = record.page;
  page->texture->UploadSprite(record.x, record.y, page->cellWidth, page->cellHeight, image, palette);
  record.lastUsed = timeGetTime();
  page->lastUsed = record.lastUsed;
  this->m_lookup.insert({Key{record.image, record.palette}, &record});
  record.u1 -= 0.5 / page->texture->Width();
  record.v1 -= 0.5 / page->texture->Height();
  output = record;

  return page->texture;
}

// OFFSET: 0x0056FE60
// FUNCTION: fn_02412
void SpriteAtlas::ClearExpired(std::uint32_t reuseDelay) {
  const std::uint32_t threshold = timeGetTime() - reuseDelay;

  for (std::forward_list<SpriteAtlasPage>& pages : this->m_classes) {
    auto previous = pages.before_begin();

    for (auto it = pages.begin(); it != pages.end();) {
      if (it->lastUsed < threshold) {
        for (const SpriteAtlasRecord& record : it->records) {
          this->m_lookup.erase({record.image, record.palette});
        }

        CTexture* texture = it->texture;
        if (texture->refCount) {
          this->m_spare.push_back(texture);
        } else {
          delete texture;
        }

        it = pages.erase_after(previous);
      } else {
        previous = it;
        ++it;
      }
    }
  }
}
