#ifndef RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDWATERSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDWATERSTATE_H

#include <cstddef>
#include <cstdint>
#include <vector>

class CGndRes;
class CTexture;
class TextureCache;

struct GroundWaterSurface {
  /* 0000 */ float level;
  /* 0004 */ int type;
  /* 0008 */ float height;
  /* 000C */ float speed;
  /* 0010 */ float pitch;
  /* 0014 */ int animation;
  /* 0018 */ int frame;
  /* 001C */ float phase;
  /* 0020 */ CTexture* texture;
};

struct GroundWaterState {
  GroundWaterState();
  void Set(float level, int type, int animation, int pitch, int phase, float height);
  bool Import(const CGndRes& resource, TextureCache& textures);
  void Update(TextureCache& textures);
  float Level(int width, int height, int column, int row) const;

  float level;
  int animation;
  int pitch;
  int phase;
  int type;
  float height;
  int columns;
  int rows;
  std::vector<float> levels;
  std::vector<GroundWaterSurface> surfaces;
  bool partitioned;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GroundWaterSurface) == 36);
static_assert(offsetof(GroundWaterSurface, texture) == 0x20);
#endif

#endif
