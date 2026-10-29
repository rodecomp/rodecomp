#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_SPRITEDRAWINGPOLICY_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_SPRITEDRAWINGPOLICY_H

class LuaScript;
struct RenderPrimitive;
struct SpriteCellQuad;
struct SpriteColor;
struct SpriteFrameCell;
struct SpriteRleImage;
struct tagRECT;
struct vector3d;

struct AccessoryLayerDepth {
  const int& bottomAccessory;
  const int& topAccessory;
  const int& middleAccessory;

  void OffsetLayerDepth(int layer, SpriteCellQuad& quad, float bottomDepth, float topDepth) const;
};

struct SpriteDrawingPolicy {
  LuaScript& script;
  const unsigned char& newDrawOnTop;
  int& argument00BC;
  const int& argument0018;
  const int& argument0010;
  const int& argument0040;
  const int& argument0044;
  bool& helmRobe;
  const vector3d& position;
  const unsigned char& riding;
  const float& offsetX;
  const float& offsetY;
  const int& shiftX;
  const int& shiftY;

  bool DrawOnTop() const;
  bool SetRobe(int robe);
  bool ProjectCell(
    int unused, int layer, float scale, const SpriteFrameCell& cell, float x, float y, const float* position, float shift, float& bottomDepth,
    float& topDepth, const SpriteRleImage& image, SpriteCellQuad& quad, tagRECT& bounds, SpriteColor& color, int offsetX, int offsetY, int flags
  ) const;
  void OffsetLayerDepth(int layer, SpriteCellQuad& quad, float bottomDepth, float topDepth) const;
  RenderPrimitive* DrawShadow(const float* matrix, float scale) const;
};

#endif
