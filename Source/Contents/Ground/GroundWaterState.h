#ifndef RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDWATERSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDWATERSTATE_H

#include <cstdint>
#include <vector>

class CGndRes;
class CTexture;
class TextureCache;

struct GroundWaterSurface {
  float level;
  int type;
  float height;
  float speed;
  float pitch;
  int animation;
  int frame;
  float phase;
  CTexture* texture;
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
#endif

#endif
