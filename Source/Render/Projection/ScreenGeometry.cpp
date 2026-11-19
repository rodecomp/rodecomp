#include "Render/Projection/ScreenGeometry.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// OFFSET: 0x0054E2B0
// FUNCTION: fn_01951
void DrawScreenLine(ScreenPoint start, ScreenPoint end, int thickness, CTexture* texture, std::uint32_t color) {
  if (SquareRoot((end.y - start.y) * (end.y - start.y) + (end.x - start.x) * (end.x - start.x)) == 0.0f) {
    return;
  }

  const float deltaX = end.x - start.x;
  const float deltaY = end.y - start.y;
  const float inverseLength = 1.0f / SquareRoot(deltaY * deltaY + deltaX * deltaX);
  float angle = static_cast<float>(std::acos(static_cast<double>(inverseLength * deltaX)));
  if (inverseLength * deltaY < 0.0f) {
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
  float currentY = startY;
  float nextX = dashX + currentX;
  float nextY = dashY + currentY;
  float distance = SquareRoot((nextY - startY) * (nextY - startY) + (nextX - startX) * (nextX - startX));
  if (!(length > distance)) {
    return;
  }

  const float halfDashX = dashX * g_depthAverageHalf;
  const float halfDashY = dashY * g_depthAverageHalf;
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
  RenderVertex* vertices = primitive->vertices;
  vertices[0].x = x0;
  vertices[0].diffuse = color;
  vertices[0].y = y0;
  vertices[0].z = 0.8f;
  vertices[0].rhw = 1.0f;
  vertices[0].u = 0.0f;
  vertices[0].v = 0.0f;
  vertices[1].x = x1;
  vertices[1].diffuse = color;
  vertices[1].y = y1;
  vertices[1].z = 0.8f;
  vertices[1].rhw = 1.0f;
  vertices[1].u = 1.0f;
  vertices[1].v = 0.0f;
  vertices[2].x = x2;
  vertices[2].diffuse = color;
  vertices[2].y = y2;
  vertices[2].z = 0.8f;
  vertices[2].rhw = 1.0f;
  vertices[2].u = 0.0f;
  vertices[2].v = 1.0f;
  vertices[3].x = x3;
  vertices[3].diffuse = color;
  vertices[3].y = y3;
  vertices[3].z = 0.8f;
  vertices[3].rhw = 1.0f;
  vertices[3].u = 1.0f;
  vertices[3].v = 1.0f;
  primitive->texture = texture;
  primitive->sourceBlend = 2;
  primitive->destinationBlend = 4;
  static_cast<CRendererDX9*>(g_renderer)->Submit(primitive, 513);
}

// OFFSET: 0x00915F60
// FUNCTION: fn_12581
__declspec(noinline) void DrawScreenQuad(ScreenPoint p0, ScreenPoint p1, ScreenPoint p2, ScreenPoint p3, CTexture* texture, std::uint32_t color) {
  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  RenderVertex* vertices = primitive->vertices;
  vertices[0].x = p0.x;
  vertices[0].diffuse = color;
  vertices[0].y = p0.y;
  vertices[0].z = 0.8f;
  vertices[0].rhw = 1.0f;
  vertices[0].u = 0.0f;
  vertices[0].v = 0.0f;
  vertices[1].x = p1.x;
  vertices[1].diffuse = color;
  vertices[1].y = p1.y;
  vertices[1].z = 0.8f;
  vertices[1].rhw = 1.0f;
  vertices[1].u = 1.0f;
  vertices[1].v = 0.0f;
  vertices[2].x = p2.x;
  vertices[2].diffuse = color;
  vertices[2].y = p2.y;
  vertices[2].z = 0.8f;
  vertices[2].rhw = 1.0f;
  vertices[2].u = 0.0f;
  vertices[2].v = 1.0f;
  vertices[3].x = p3.x;
  vertices[3].diffuse = color;
  vertices[3].y = p3.y;
  vertices[3].z = 0.8f;
  vertices[3].rhw = 1.0f;
  vertices[3].u = 1.0f;
  vertices[3].v = 1.0f;
  primitive->texture = texture;
  primitive->sourceBlend = 2;
  primitive->destinationBlend = 4;
  static_cast<CRendererDX9*>(g_renderer)->Submit(primitive, 513);
}
