#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_SPRITEDRAWINGPOLICY_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_SPRITEDRAWINGPOLICY_H

class LuaScript;
struct SpriteCellQuad;
struct SpriteColor;
struct SpriteFrameCell;
struct SpriteRleImage;
struct tagRECT;

struct SpriteDrawingPolicy {
  LuaScript& script;
  const unsigned char& newDrawOnTop;
  int& argument00BC;
  const int& argument0018;
  const int& argument0010;
  const int& argument0040;
  const int& argument0044;
  bool& helmRobe;

  bool DrawOnTop() const;
  bool SetRobe(int robe);
  bool ProjectCell(
    int unused, int layer, float scale, const SpriteFrameCell& cell, float x, float y, const float* position, float shift, float& bottomDepth,
    float& topDepth, const SpriteRleImage& image, SpriteCellQuad& quad, tagRECT& bounds, SpriteColor& color, int offsetX, int offsetY, int flags
  ) const;
};

#endif
