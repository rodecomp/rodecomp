#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_ALPHARECTANGLE_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_ALPHARECTANGLE_H

#include <cstdint>

class RenderPrimitivePool;
class TextureCache;
struct RenderQueues;

void DrawAlphaRectangle(
  RenderPrimitivePool& primitives, RenderQueues& queues, int x, int y, int width, int height, std::uint32_t color, bool depthAlpha,
  float depthStart, float depthEnd
);

#endif
