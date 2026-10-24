#ifndef RAGNAROK_SOURCE_RENDER_ALPHARECTANGLE_H
#define RAGNAROK_SOURCE_RENDER_ALPHARECTANGLE_H

#include <cstdint>

class RenderPrimitivePool;
class TextureCache;
struct RenderQueues;

// Original global renderer/texture-cache dependencies are borrowed. A zero
// alpha byte returns before acquiring a pooled primitive or default texture.
void DrawAlphaRectangle(
  RenderPrimitivePool& primitives,
  TextureCache& textures,
  RenderQueues& queues,
  int x,
  int y,
  int width,
  int height,
  std::uint32_t color,
  bool depthAlpha,
  float depthStart,
  float depthEnd
);

#endif
