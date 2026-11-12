#ifndef RAGNAROK_SOURCE_RENDER_PROJECTION_SCREENGEOMETRY_H
#define RAGNAROK_SOURCE_RENDER_PROJECTION_SCREENGEOMETRY_H

#include <cstdint>

class CTexture;
class RenderPrimitivePool;
struct RenderQueues;

struct ScreenPoint {
  /* 0000 */ float x;
  /* 0004 */ float y;
};

__declspec(noinline) void DrawScreenQuad(
  float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, CTexture* texture, std::uint32_t color
);
void DrawScreenLine(ScreenPoint start, ScreenPoint end, int thickness, CTexture* texture, std::uint32_t color);
void DrawScreenQuad(ScreenPoint p0, ScreenPoint p1, ScreenPoint p2, ScreenPoint p3, CTexture* texture, std::uint32_t color);

#endif
