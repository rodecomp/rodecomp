#ifndef RAGNAROK_SOURCE_CONTENTS_GROUNDWATERSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_GROUNDWATERSTATE_H

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

// C3dGround fields +0x38..+0x4F and C3dGround15 +0x88..+0xA8.
// Surface texture pointers borrow the name-keyed cache, not owned atlases.
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
  std::vector<std::uint32_t> levels;
  std::vector<GroundWaterSurface> surfaces;
  bool partitioned;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GroundWaterSurface) == 36);
#endif

#endif
