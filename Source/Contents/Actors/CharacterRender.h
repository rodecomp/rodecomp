#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_CHARACTERRENDER_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_CHARACTERRENDER_H

#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <map>

class CActRes;
struct CharacterRenderDrawItem;
struct RenderPrimitive;
struct SpriteCellQuad;
struct SpriteColor;
struct SpriteFrameCell;
struct SpriteRleImage;
struct tagRECT;

class CCharacterRender {
public:
  CCharacterRender(int width, int renderHeight);
  virtual ~CCharacterRender();

  void RenderRobe(
    std::list<RenderPrimitive*>& primitives, bool drawOnTop, const void*, int& layer, const float* position, float x, float y, float scale,
    float shift, float, CActRes* baseAct
  );
  void RenderRobeCommands(
    std::list<CharacterRenderDrawItem>& commands, bool drawOnTop, const void*, int& layer, const float* position, float x, float y, float scale,
    float shift, float
  );
  std::list<CharacterRenderDrawItem> BuildDrawList(unsigned char trackBounds, char* boundsUpdated);
  bool DrawOnTop() const;
  bool SetRobe(int robe);
  bool ProjectCell(
    int unused, int layer, float scale, const SpriteFrameCell& cell, float x, float y, const float* position, float shift, float& bottomDepth,
    float& topDepth, const SpriteRleImage& image, SpriteCellQuad& quad, tagRECT& bounds, SpriteColor& color, int offsetX, int offsetY, int flags
  ) const;
  void OffsetLayerDepth(int layer, SpriteCellQuad& quad, float bottomDepth, float topDepth) const;
  void OffsetAccessoryLayerDepth(int layer, SpriteCellQuad& quad, float bottomDepth, float topDepth) const;
  RenderPrimitive* DrawShadow(const float* matrix, float scale) const;

  /* 0004 */ vector3d position;
  /* 0010 */ int job;
  /* 0014 */ int headJob;
  /* 0018 */ int appearance[4];
  /* 0028 */ int lowerHeadgear;
  /* 002C */ int upperHeadgear;
  /* 0030 */ int middleHeadgear;
  /* 0034 */ int lowerHeadgearLocation;
  /* 0038 */ int upperHeadgearLocation;
  /* 003C */ int middleHeadgearLocation;
  /* 0040 */ int actionIndex;
  /* 0044 */ int frameIndex;
  /* 0048 */ int madogearType;
  /* 004C */ unsigned char riding;
  /* 0050 */ int bodyCostume;
  /* 0054 */ float viewportOffsetX;
  /* 0058 */ float viewportOffsetY;
  /* 005C */ int bodyBlend;
  /* 0060 */ int bodyCellX;
  /* 0064 */ int bodyCellY;
  /* 0068 */ int viewportWidth;
  /* 006C */ int viewportHeight;
  /* 0070 */ float cameraDistance;
  /* 0074 */ float cameraPitch;
  /* 0078 */ float cameraYaw;
  /* 007C */ float padding_007C;
  /* 0080 */ float padding_0080;
  /* 0084 */ float padding_0084;
  /* 0088 */ float cameraUpX;
  /* 008C */ float cameraUpY;
  /* 0090 */ int cameraUpZ;
  /* 0094 */ int renderBounds[5];
  /* 00A8 */ float screenX;
  /* 00AC */ float screenY;
  /* 00B0 */ unsigned char drawShadow;
  /* 00B4 */ std::map<int, int> layerViews;
  /* 00BC */ int robe;
  /* 00C0 */ bool isHelmRobe;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CCharacterRender, viewportWidth) == 0x68);
static_assert(offsetof(CCharacterRender, cameraUpY) == 0x8C);
static_assert(offsetof(CCharacterRender, layerViews) == 0xB4);
static_assert(sizeof(CCharacterRender) == 0xC4);
#endif

#endif
