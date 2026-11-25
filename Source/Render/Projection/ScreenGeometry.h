#ifndef RAGNAROK_SOURCE_RENDER_PROJECTION_SCREENGEOMETRY_H
#define RAGNAROK_SOURCE_RENDER_PROJECTION_SCREENGEOMETRY_H

#include "Utility/Math/AffineMatrix.h"

#include <cstdint>

class CTexture;
class RenderPrimitivePool;
struct RenderQueues;

struct ScreenPoint {
  // INLINED
  ScreenPoint() {
  }
  // INLINED
  ScreenPoint(float x, float y) : x(x), y(y) {
  }

  /* 0000 */ float x;
  /* 0004 */ float y;
};

// INLINED
inline float PointDistance(const ScreenPoint& from, const ScreenPoint& to) {
  return SquareRoot((to.x - from.x) * (to.x - from.x) + (to.y - from.y) * (to.y - from.y));
}

// INLINED
inline void NormalizePoint(ScreenPoint& point) {
  const float inverse = 1.0f / SquareRoot(point.x * point.x + point.y * point.y);
  point.x = inverse * point.x;
  point.y = inverse * point.y;
}

__declspec(noinline) void DrawScreenQuad(
  float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, CTexture* texture, std::uint32_t color
);
void DrawScreenLine(ScreenPoint start, ScreenPoint end, int thickness, CTexture* texture, std::uint32_t color);
void DrawDashedScreenLine(
  float startX, float startY, float endX, float endY, int dashLength, int thickness, CTexture* texture, std::uint32_t color
);
void DrawScreenQuad(ScreenPoint p0, ScreenPoint p1, ScreenPoint p2, ScreenPoint p3, CTexture* texture, std::uint32_t color);

#endif
