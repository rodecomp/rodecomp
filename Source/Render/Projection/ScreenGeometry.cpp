#include "Render/Projection/ScreenGeometry.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// INLINED
static void SetScreenQuadVertex(RenderVertex& vertex, float x, float y, std::uint32_t color, float u, float v) {
  vertex.x = x;
  vertex.diffuse = color;
  vertex.y = y;
  vertex.z = 0.8f;
  vertex.rhw = 1.0f;
  vertex.u = u;
  vertex.v = v;
}

// OFFSET: 0x0054E2B0
// FUNCTION: fn_01951
void DrawScreenLine(ScreenPoint start, ScreenPoint end, int thickness, CTexture* texture, std::uint32_t color) {
  if (0.0f == SquareRoot((end.y - start.y) * (end.y - start.y) + (end.x - start.x) * (end.x - start.x))) {
    return;
  }

  ScreenPoint direction = {end.x - start.x, end.y - start.y};
  NormalizePoint(direction);
  float angle = static_cast<float>(std::acos(static_cast<double>(direction.x)));
  if (direction.y < 0.0f) {
    angle = 6.283184f - angle;
  }

  angle += 1.570796f;
  const float width = static_cast<float>(thickness);
  const float offsetY = ClientSin(angle) * width;
  const float offsetX = ClientCos(angle) * width;
  const ScreenPoint p0 = {start.x - offsetX, start.y - offsetY};
  const ScreenPoint p1 = {start.x + offsetX, start.y + offsetY};
  const ScreenPoint p2 = {end.x - offsetX, end.y - offsetY};
  const ScreenPoint p3 = {end.x + offsetX, end.y + offsetY};
  DrawScreenQuad(p0, p1, p2, p3, texture, color);
}

// OFFSET: 0x0054E6D0
// FUNCTION: fn_45915
void DrawDashedScreenLine(
  float startX, float startY, float endX, float endY, int dashLength, int thickness, CTexture* texture, std::uint32_t color
) {
  const float length = SquareRoot((endY - startY) * (endY - startY) + (endX - startX) * (endX - startX));
  if (0.0f == length) {
    return;
  }

  const float deltaX = endX - startX;
  const float deltaY = endY - startY;
  const float inverseLength = 1.0f / SquareRoot(deltaY * deltaY + deltaX * deltaX);
  const float directionX = inverseLength * deltaX;
  const float directionY = inverseLength * deltaY;
  float angle = static_cast<float>(std::acos(static_cast<double>(directionX)));
  if (0.0f > directionY) {
    angle = 6.283184f - angle;
  }

  angle += 1.570796f;
  const float width = static_cast<float>(thickness);
  const float sideY = ClientSin(angle) * width;
  const float sideX = ClientCos(angle) * width;
  const float dash = static_cast<float>(dashLength);
  const float dashY = dash * directionY;
  const float dashX = dash * directionX;
  float currentX = startX;
  float nextX = dashX + currentX;
  float currentY = startY;
  float nextY = dashY + currentY;
  float distance = SquareRoot((nextY - startY) * (nextY - startY) + (nextX - startX) * (nextX - startX));
  if (!(length > distance)) {
    return;
  }

  const float halfDashY = dashY * g_depthAverageHalf;
  const float halfDashX = dashX * g_depthAverageHalf;
  do {
    DrawScreenQuad(currentX, currentY, currentX + sideX, currentY + sideY, nextX, nextY, nextX + sideX, nextY + sideY, texture, color);

    currentX = halfDashX + nextX;
    currentY = halfDashY + nextY;
    nextX = dashX + currentX;
    nextY = dashY + currentY;
    distance = SquareRoot((nextY - startY) * (nextY - startY) + (nextX - startX) * (nextX - startX));
  } while (length > distance);
}

// OFFSET: 0x0054DB40
// FUNCTION: fn_01944
void DrawScreenQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, CTexture* texture, std::uint32_t color) {
  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  SetScreenQuadVertex(primitive->vertices[0], x0, y0, color, 0.0f, 0.0f);
  SetScreenQuadVertex(primitive->vertices[1], x1, y1, color, 1.0f, 0.0f);
  SetScreenQuadVertex(primitive->vertices[2], x2, y2, color, 0.0f, 1.0f);
  SetScreenQuadVertex(primitive->vertices[3], x3, y3, color, 1.0f, 1.0f);
  primitive->texture = texture;
  primitive->sourceBlend = 2;
  primitive->destinationBlend = 4;
  g_renderer->Submit(primitive, 513);
}

// OFFSET: 0x00915F60
// FUNCTION: fn_12581
__declspec(noinline) void DrawScreenQuad(ScreenPoint p0, ScreenPoint p1, ScreenPoint p2, ScreenPoint p3, CTexture* texture, std::uint32_t color) {
  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  RenderVertex* vertices = primitive->vertices;
  SetScreenQuadVertex(vertices[0], p0.x, p0.y, color, 0.0f, 0.0f);
  SetScreenQuadVertex(vertices[1], p1.x, p1.y, color, 1.0f, 0.0f);
  SetScreenQuadVertex(vertices[2], p2.x, p2.y, color, 0.0f, 1.0f);
  SetScreenQuadVertex(vertices[3], p3.x, p3.y, color, 1.0f, 1.0f);
  primitive->texture = texture;
  primitive->sourceBlend = 2;
  primitive->destinationBlend = 4;
  g_renderer->Submit(primitive, 513);
}
