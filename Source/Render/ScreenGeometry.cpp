#include "Render/ScreenGeometry.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// OFFSET: 0x0054E2B0
// FUNCTION: fn_01951
void DrawScreenLine(
  RenderPrimitivePool& primitives, RenderQueues& queues, float startX, float startY, float endX, float endY, int thickness, CTexture* texture,
  std::uint32_t color, bool depthAlpha, float depthStart, float depthEnd
) {
  if (SquareRoot((endY - startY) * (endY - startY) + (endX - startX) * (endX - startX)) == 0.0f) {
    return;
  }

  const float deltaX = endX - startX;
  const float deltaY = endY - startY;
  const float inverseLength = 1.0f / SquareRoot(deltaY * deltaY + deltaX * deltaX);
  float angle = static_cast<float>(std::acos(static_cast<double>(inverseLength * deltaX)));
  if (inverseLength * deltaY < 0.0f) {
    angle = 6.283184f - angle;
  }

  angle += 1.570796f;
  const float width = static_cast<float>(thickness);
  const float offsetY = std::sin(angle) * width;
  const float offsetX = std::cos(angle) * width;
  DrawScreenQuad(
    primitives, queues, startX - offsetX, startY - offsetY, startX + offsetX, startY + offsetY, endX - offsetX, endY - offsetY, endX + offsetX,
    endY + offsetY, texture, color, depthAlpha, depthStart, depthEnd
  );
}

// OFFSET: 0x0054DB40
// FUNCTION: fn_01944
void DrawScreenQuad(
  RenderPrimitivePool& primitives, RenderQueues& queues, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, CTexture* texture,
  std::uint32_t color, bool depthAlpha, float depthStart, float depthEnd
) {
  RenderPrimitive* primitive = primitives.Acquire();
  RenderVertex* vertices = primitive->vertices;
  vertices[0].x = x0;
  vertices[0].y = y0;
  vertices[0].z = 0.8f;
  vertices[0].rhw = 1.0f;
  vertices[0].diffuse = color;
  vertices[0].u = 0.0f;
  vertices[0].v = 0.0f;
  vertices[1].x = x1;
  vertices[1].y = y1;
  vertices[1].z = 0.8f;
  vertices[1].rhw = 1.0f;
  vertices[1].diffuse = color;
  vertices[1].u = 1.0f;
  vertices[1].v = 0.0f;
  vertices[2].x = x2;
  vertices[2].y = y2;
  vertices[2].z = 0.8f;
  vertices[2].rhw = 1.0f;
  vertices[2].diffuse = color;
  vertices[2].u = 0.0f;
  vertices[2].v = 1.0f;
  vertices[3].x = x3;
  vertices[3].y = y3;
  vertices[3].z = 0.8f;
  vertices[3].rhw = 1.0f;
  vertices[3].diffuse = color;
  vertices[3].u = 1.0f;
  vertices[3].v = 1.0f;
  primitive->texture = texture;
  primitive->unknown_0018 = 2;
  primitive->unknown_001C = 4;
  queues.Submit(primitive, 513, depthAlpha, depthStart, depthEnd);
}

// OFFSET: 0x00915F60
// FUNCTION: fn_12581
void DrawScreenQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, CTexture* texture, std::uint32_t color) {
  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  RenderVertex* vertices = primitive->vertices;
  vertices[0].x = x0;
  vertices[0].y = y0;
  vertices[0].z = 0.8f;
  vertices[0].rhw = 1.0f;
  vertices[0].diffuse = color;
  vertices[0].u = 0.0f;
  vertices[0].v = 0.0f;
  vertices[1].x = x1;
  vertices[1].y = y1;
  vertices[1].z = 0.8f;
  vertices[1].rhw = 1.0f;
  vertices[1].diffuse = color;
  vertices[1].u = 1.0f;
  vertices[1].v = 0.0f;
  vertices[2].x = x2;
  vertices[2].y = y2;
  vertices[2].z = 0.8f;
  vertices[2].rhw = 1.0f;
  vertices[2].diffuse = color;
  vertices[2].u = 0.0f;
  vertices[2].v = 1.0f;
  vertices[3].x = x3;
  vertices[3].y = y3;
  vertices[3].z = 0.8f;
  vertices[3].rhw = 1.0f;
  vertices[3].diffuse = color;
  vertices[3].u = 1.0f;
  vertices[3].v = 1.0f;
  primitive->texture = texture;
  primitive->unknown_0018 = 2;
  primitive->unknown_001C = 4;
  g_renderer->queues.Submit(primitive, 513, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}
