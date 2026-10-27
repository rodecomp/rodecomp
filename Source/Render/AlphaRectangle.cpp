#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Textures/TextureCache.h"

// OFFSET: 0x0054E460
// FUNCTION: fn_01952
void DrawAlphaRectangle(
  RenderPrimitivePool& primitives, TextureCache& textures, RenderQueues& queues, int x, int y, int width, int height, std::uint32_t color, bool depthAlpha,
  float depthStart, float depthEnd
) {
  if ((color & 0xFF000000u) == 0) {
    return;
  }

  RenderPrimitive* primitive = primitives.Acquire();
  primitive->texture = textures.Default();
  const float left = static_cast<float>(x);
  const float top = static_cast<float>(y);
  const float right = static_cast<float>(x + width);
  const float bottom = static_cast<float>(y + height);
  primitive->quad[0].x = left;
  primitive->quad[0].y = top;
  primitive->quad[1].x = right;
  primitive->quad[1].y = top;
  primitive->quad[2].x = left;
  primitive->quad[2].y = bottom;
  primitive->quad[3].x = right;
  primitive->quad[3].y = bottom;
  for (RenderVertex& vertex : primitive->quad) {
    vertex.z = 0.000001f;
    vertex.rhw = 0.999999f;
    vertex.diffuse = color;
    vertex.u = 0.0f;
    vertex.v = 0.0f;
  }
  queues.Submit(primitive, 513, depthAlpha, depthStart, depthEnd);
}
