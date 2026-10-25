#ifndef RAGNAROK_SOURCE_RESOURCE_SPRITES_SPRITERESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_SPRITES_SPRITERESOURCE_H

#include "Render/Textures/SpritePixels.h"
#include "Resource/Resource.h"

#include <vector>

class ResourceBytes;

struct SpriteResourceContext {
  ResourceBytes& file;
  CRenderSystemBase*& renderSystem;
  const int& halfWidth;
  const int& halfHeight;
};

class CSprRes : public CRes {
public:
  explicit CSprRes(SpriteResourceContext& context);
  ~CSprRes() override;
  CSprRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;

  /* 0110 */ std::uint32_t palette[256];
  /* 0510 */ std::vector<SpriteRleImage*> images[2];
  /* 0528 */ int indexedCount;

private:
  SpriteResourceContext* m_context;
};

std::uint8_t* EncodeSpriteRuns(const std::uint8_t* pixels, int width, int height, std::uint16_t& bytes);
std::uint8_t* ReduceSprite(const std::uint8_t* pixels, int width, int height, int halfWidth, int halfHeight);
void ConvertSpritePalette(std::uint32_t* output, const std::uint8_t* input, int count);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSprRes) == 1328);
static_assert(sizeof(SpriteRleImage) == 16);
#endif

#endif
