#include "Render/RenderSystemBase.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"

#include <bit>
#include <windows.h>
#include <timeapi.h>

namespace {
  // STATIC: 0x010DD520 / 0x010DD524
  constexpr unsigned dimensions[5] = {32, 64, 128, 256, 512};

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

// OFFSET: 0x00560A40
// FUNCTION: fn_02161
SpriteAtlas::SpriteAtlas() : m_classes{} {
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
  const auto hashPointer = [](std::uintptr_t pointer) {
    std::uint32_t hash = 0x811C9DC5;

    for (unsigned i = 0; i < sizeof(pointer); ++i) {
      hash = (hash ^ ((pointer >> (i * 8)) & 255)) * 16777619u;
    }

    return hash;
  };
  const std::uint32_t first = hashPointer(reinterpret_cast<std::uintptr_t>(key.image));
  return first ^ (hashPointer(reinterpret_cast<std::uintptr_t>(key.palette)) + (first << 6) + (first >> 2) + 0x9E3779B9u);
}

// OFFSET: 0x0056FB00
// FUNCTION: fn_02407
void SpriteAtlas::Initialize(CRenderSystemBase& system, unsigned spareCount) {
  this->Clear();
  this->ClearSpare();
  if (this->m_spare.capacity() < spareCount) {
    this->m_spare.reserve(spareCount);
  }

  for (unsigned i = 0; i < spareCount; ++i) {
    CTexture* texture = system.CreateTexture(256, 256, TextureFormat::A1R5G5B5, false, false);
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
  for (Node*& head : this->m_classes) {
    for (Node* node = head; node; node = node->next) {
      CTexture* texture = node->page.texture;
      if (texture->unknown_001C) {
        this->m_spare.push_back(texture);
      } else {
        delete texture;
      }
    }

    while (head) {
      Node* next = head->next;
      delete head;
      head = next;
    }
  }

  this->m_lookup.clear();
}

// OFFSET: 0x0056FC40
// FUNCTION: fn_02409
void SpriteAtlas::ClearSpare() {
  for (CTexture* texture : this->m_spare) {
    texture->Release();
    delete texture;
  }

  this->m_spare.clear();
}

// OFFSET: 0x0056FBB0
// FUNCTION: fn_02408
CTexture* SpriteAtlas::Find(const SpriteRleImage* image, const std::uint32_t* palette, SpriteAtlasRecord& output) {
  auto found = this->m_lookup.find({image, palette});
  if (found == this->m_lookup.end()) {
    return nullptr;
  }

  SpriteAtlasRecord& record = *found->second;
  record.lastUsed = timeGetTime();
  record.page->lastUsed = record.lastUsed;
  output = record;

  return record.page->texture;
}

// OFFSET: 0x0056F830
// FUNCTION: fn_02406
SpriteAtlasPage* SpriteAtlas::Allocate(CRenderSystemBase& system, unsigned category, unsigned width, unsigned height) {
  Node* node = new Node{};
  node->next = this->m_classes[category];
  this->m_classes[category] = node;
  SpriteAtlasPage& page = node->page;

  if (width <= 256 && height <= 256 && !this->m_spare.empty()) {
    page.texture = this->m_spare.back();
    this->m_spare.pop_back();
  } else {
    page.texture = system.CreateTexture(width, height, TextureFormat::A1R5G5B5, false, false);
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
  const unsigned width = image->width > 256 ? (image->width / 256 + 1) * 256 : 256;
  const unsigned height = image->height > 256 ? (image->height / 256 + 1) * 256 : 256;
  const unsigned category = Category(image->width) * 5 + Category(image->height);
  SpriteAtlasRecord* selected = nullptr;

  for (Node* node = this->m_classes[category]; node && !selected; node = node->next) {
    for (SpriteAtlasRecord& record : node->page.records) {
      if (record.lastUsed < threshold) {
        selected = &record;
        this->m_lookup.erase({record.image, record.palette});
        break;
      }
    }
  }

  if (!selected) {
    selected = &this->Allocate(system, category, width, height)->records[0];
  }

  SpriteAtlasRecord& record = *selected;
  record.image = image;
  record.palette = palette;
  float u1 = static_cast<float>(record.x + image->width) / static_cast<float>(width);
  float v1 = static_cast<float>(record.y + image->height) / static_cast<float>(height);

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
