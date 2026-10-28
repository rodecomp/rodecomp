#ifndef RAGNAROK_SOURCE_RENDER_SCREENGEOMETRY_H
#define RAGNAROK_SOURCE_RENDER_SCREENGEOMETRY_H

#include <cstdint>

class CTexture;
class RenderPrimitivePool;
struct RenderQueues;

void DrawScreenQuad(
  RenderPrimitivePool& primitives, RenderQueues& queues, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, CTexture* texture,
  std::uint32_t color, bool depthAlpha, float depthStart, float depthEnd
);
void DrawScreenLine(
  RenderPrimitivePool& primitives, RenderQueues& queues, float startX, float startY, float endX, float endY, int thickness, CTexture* texture,
  std::uint32_t color, bool depthAlpha, float depthStart, float depthEnd
);

#endif
