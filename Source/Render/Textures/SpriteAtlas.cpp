#include "Render/RenderSystemBase.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

#include <bit>
#include <windows.h>
#include <timeapi.h>

namespace {
  // STATIC: 0x010DD520
  // STATIC: 0x010DD524
  constexpr unsigned dimensions[5] = {32, 64, 128, 256, 512};

  // INLINED
  unsigned Category(int dimension) {
    if (dimension > 256) {
      return 4;
    }

    if (dimension > 128) {
      return 3;
    }

    if (dimension > 64) {
      return 2;
    }

    if (dimension > 32) {
      return 1;
    }

    return 0;
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
  if (this->m_spare.capacity() < spareCount) {
    this->m_spare.reserve(spareCount);
  }

  for (unsigned i = 0; i < spareCount; ++i) {
    CTexture* texture = CreateTexture(256, 256, TextureFormat::A1R5G5B5, false, false);
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
  for (std::forward_list<SpriteAtlasPage>& pages : this->m_classes) {
    for (SpriteAtlasPage& page : pages) {
      CTexture* texture = page.texture;
      if (texture->unknown_001C) {
        this->m_spare.push_back(texture);
      } else {
        delete texture;
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

  if (width <= 256 && height <= 256) {
    if (this->m_spare.empty()) {
      page.texture = CreateTexture(256, 256, TextureFormat::A1R5G5B5, false, false);
    } else {
      page.texture = this->m_spare.back();
      this->m_spare.pop_back();
    }
  } else {
    page.texture = CreateTexture(width, height, TextureFormat::A1R5G5B5, false, false);
  }

  page.texture->Fill(nullptr, 0);
  page.cellWidth = dimensions[category / 5];
  page.cellHeight = dimensions[category % 5];
  const unsigned columns = width / page.cellWidth;
  const unsigned rows = height / page.cellHeight;
  page.records.resize(columns * rows);

  for (unsigned y = 0; y < rows; ++y) {
    for (unsigned x = 0; x < columns; ++x) {
      SpriteAtlasRecord& record = page.records[x + y * columns];
      record.x = x * page.cellWidth;
      record.y = y * page.cellHeight;
      record.u0 = static_cast<float>(record.x) / static_cast<float>(width);
      record.v0 = static_cast<float>(record.y) / static_cast<float>(height);
      record.page = &page;
    }
  }

  return &page;
}

// OFFSET: 0x0056F410
// FUNCTION: fn_02404
CTexture* SpriteAtlas::Insert(
  CRenderSystemBase& system, const SpriteRleImage* image, const std::uint32_t* palette, SpriteAtlasRecord& output, int serviceType, std::uint32_t reuseDelay
) {
  const std::uint32_t threshold = timeGetTime() - (reuseDelay >> 2);
  const float width = image->width > 256 ? (image->width / 256 + 1) * 256.0f : 256.0f;
  const float height = image->height > 256 ? (image->height / 256 + 1) * 256.0f : 256.0f;
  const unsigned category = Category(image->width) * 5 + Category(image->height);
  SpriteAtlasRecord* selected = nullptr;

  std::forward_list<SpriteAtlasPage>& pages = this->m_classes[category];
  for (auto page = pages.begin(); page != pages.end() && !selected; ++page) {
    for (SpriteAtlasRecord& record : page->records) {
      if (record.lastUsed < threshold) {
        selected = &record;
        this->m_lookup.erase({record.image, record.palette});
        break;
      }
    }
  }

  if (!selected) {
    selected = &this->Allocate(category, static_cast<unsigned>(width), static_cast<unsigned>(height))->records[0];
  }

  SpriteAtlasRecord& record = *selected;
  record.image = image;
  record.palette = palette;
  float u1 = static_cast<float>(record.x + image->width) / width;
  float v1 = static_cast<float>(record.y + image->height) / height;

  if (serviceType == 10) {
    if (image->width > 64) {
      u1 -= 0.005f;
    }

    if (image->height > 64) {
      v1 -= 0.005f;
    }
  }

  record.u1 = u1;
  record.v1 = v1;
  CTexture* texture = record.page->texture;
  texture->UploadSprite(record.x, record.y, record.page->cellWidth, record.page->cellHeight, image, palette);
  record.lastUsed = timeGetTime();
  record.page->lastUsed = record.lastUsed;
  this->m_lookup.emplace(Key{image, palette}, &record);
  record.u1 = static_cast<float>(record.u1 - 0.5 / texture->Width());
  record.v1 = static_cast<float>(record.v1 - 0.5 / texture->Height());
  output = record;

  return texture;
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
        if (texture->unknown_001C) {
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
