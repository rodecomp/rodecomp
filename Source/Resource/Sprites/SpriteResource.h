#ifndef RAGNAROK_SOURCE_RESOURCE_SPRITES_SPRITERESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_SPRITES_SPRITERESOURCE_H

#include "Render/Textures/SpritePixels.h"
#include "Resource/Resource.h"

#include <vector>

class ResourceBytes;

class CSprRes : public CRes {
public:
  CSprRes();
  CSprRes(const char* extension, const char* prefix);
  ~CSprRes() override;
  CSprRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;
  std::uint8_t* EncodeSpriteRuns(const std::uint8_t* pixels, int width, int height, std::uint16_t& bytes);
  std::uint8_t* ReduceSprite(const std::uint8_t* pixels, int width, int height, int halfWidth, int halfHeight);

  /* 0110 */ std::uint32_t palette[256];
  /* 0510 */ std::vector<SpriteRleImage*> images[2];
  /* 0528 */ int indexedCount;

};

void ConvertSpritePalette(std::uint32_t* output, const std::uint8_t* input, int count);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSprRes) == 1324);
static_assert(sizeof(SpriteRleImage) == 16);
#endif

#endif
