#ifndef RAGNAROK_SOURCE_CONTENTS_SPRITELAYERDRAWING_H
#define RAGNAROK_SOURCE_CONTENTS_SPRITELAYERDRAWING_H

#include "Contents/Actors/RenderObject.h"
#include "Render/SpriteQuad.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <utility>

class CActRes;
class CImfRes;
class CPc;
class CSprRes;
struct ActionFrame;
struct SpriteFrameCell;
struct SpriteRleImage;

struct SpriteLayerResources {
  /* 0000 */ CSprRes* sprite;
  /* 0004 */ CActRes* action;
};

struct SpriteLayerCell {
  /* 0000 */ const SpriteRleImage* image;
  /* 0004 */ const SpriteFrameCell* cell;
  /* 0008 */ int offsetY;
  /* 000C */ int offsetX;
};

struct SpriteLayerQuad {
  /* 0000 */ SpriteQuad quad;
  /* 0030 */ const std::uint32_t* palette;
  /* 0034 */ int blend;
};

class SpriteLayerRenderer {
public:
  // INLINED
  explicit SpriteLayerRenderer(CPc* pc) : pc(pc), position(), depthScale(1.0f), depthBias(0.0f) {
  }

  void Render(const float* matrix, int unknown0030, char unknown0034);
  void QueueQuad(int layer, const SpriteQuad* quad, const std::uint32_t* palette, int blend);
  void DrawLayerOutlines(std::uint32_t color);
  void DrawRobe(bool drawOnTop);
  int DrawBodyOutline(int layer);
  SpriteLayerCell ResolveCell(
    int layer, const SpriteFrameCell* cell, int cellIndex, CSprRes* sprite, CActRes* action, const ActionFrame* frame,
    const SpriteRleImage* image, int animation, int frameIndex, bool replaced
  );
  __declspec(noinline) std::pair<int, int> GetItemOffset(int layer);
  __declspec(noinline) const std::uint32_t* GetPalette(int layer, CSprRes* sprite, CActRes* action);
  __declspec(noinline) std::pair<bool, SpriteLayerResources> FindLayer(int layer);
  std::pair<bool, std::string> GetLayerSound(int layer, CActRes* action, int sound);
  void DrawLayer(
    int index, int layer, const ActionFrame* frame, CSprRes* sprite, CActRes* action, int animation, int frameIndex, bool replaced
  );
  void LoadLayers();

  /* 0000 */ float matrix[12];
  /* 0030 */ int unknown_0030;
  /* 0034 */ char unknown_0034;
  /* 0038 */ CPc* pc;
  /* 003C */ vector3d position;
  /* 0048 */ float x;
  /* 004C */ float y;
  /* 0050 */ float rhw;
  /* 0054 */ float distanceScale;
  /* 0058 */ float scale;
  /* 005C */ CImfRes* imf;
  /* 0060 */ int unknown_0060;
  /* 0064 */ int swingX;
  /* 0068 */ int swingY;
  /* 006C */ bool unknown_006C;
  /* 006D */ bool unknown_006D;
  /* 0070 */ std::map<int, SpriteLayerResources> layers;
  /* 0078 */ std::map<int, int> layerViews;
  /* 0080 */ RenderBounds bounds;
  /* 0094 */ bool unknown_0094;
  /* 0098 */ std::map<int, SpriteLayerQuad> quads;
  /* 00A0 */ float depthScale;
  /* 00A4 */ float depthBias;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SpriteLayerQuad) == 0x38);
static_assert(offsetof(SpriteLayerRenderer, pc) == 0x38);
static_assert(offsetof(SpriteLayerRenderer, layers) == 0x70);
static_assert(offsetof(SpriteLayerRenderer, layerViews) == 0x78);
static_assert(offsetof(SpriteLayerRenderer, bounds) == 0x80);
static_assert(offsetof(SpriteLayerRenderer, quads) == 0x98);
static_assert(sizeof(SpriteLayerRenderer) == 0xA8);
#endif

#endif
