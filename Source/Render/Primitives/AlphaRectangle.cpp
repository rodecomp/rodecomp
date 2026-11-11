#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/TextureCache.h"

// OFFSET: 0x0054E460
// FUNCTION: fn_01952
void DrawAlphaRectangle(int x, int y, int width, int height, std::uint32_t color) {
  if ((color & 0xFF000000u) == 0) {
    return;
  }

  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  primitive->texture = g_texMgr.Default();
  const float left = static_cast<float>(x);
  const float top = static_cast<float>(y);
  const float right = static_cast<float>(x + width);
  const float bottom = static_cast<float>(y + height);
  primitive->quad[0].x = left;
  primitive->quad[0].y = top;
  primitive->quad[0].z = 0.000001f;
  primitive->quad[0].rhw = 0.999999f;
  primitive->quad[0].u = 0.0f;
  primitive->quad[0].v = 0.0f;
  primitive->quad[0].diffuse = color;
  primitive->quad[1].x = right;
  primitive->quad[1].y = top;
  primitive->quad[1].z = 0.000001f;
  primitive->quad[1].rhw = 0.999999f;
  primitive->quad[1].u = 0.0f;
  primitive->quad[1].v = 0.0f;
  primitive->quad[1].diffuse = color;
  primitive->quad[2].x = left;
  primitive->quad[2].y = bottom;
  primitive->quad[2].z = 0.000001f;
  primitive->quad[2].rhw = 0.999999f;
  primitive->quad[2].u = 0.0f;
  primitive->quad[2].v = 0.0f;
  primitive->quad[2].diffuse = color;
  primitive->quad[3].x = right;
  primitive->quad[3].y = bottom;
  primitive->quad[3].z = 0.000001f;
  primitive->quad[3].rhw = 0.999999f;
  primitive->quad[3].u = 0.0f;
  primitive->quad[3].v = 0.0f;
  primitive->quad[3].diffuse = color;
  g_renderer->queues.Submit(primitive, 513, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}
