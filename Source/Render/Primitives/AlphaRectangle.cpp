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

  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  primitive->texture = g_texMgr.Default();
  primitive->quad[0].x = static_cast<float>(x);
  primitive->quad[0].y = static_cast<float>(y);
  primitive->quad[0].z = 0.000001f;
  primitive->quad[0].rhw = 0.999999f;
  primitive->quad[0].u = 0.0f;
  primitive->quad[0].v = 0.0f;
  primitive->quad[0].diffuse = color;
  primitive->quad[1].x = static_cast<float>(x + width);
  primitive->quad[1].y = static_cast<float>(y);
  primitive->quad[1].z = 0.000001f;
  primitive->quad[1].rhw = 0.999999f;
  primitive->quad[1].u = 0.0f;
  primitive->quad[1].v = 0.0f;
  primitive->quad[1].diffuse = color;
  primitive->quad[2].x = static_cast<float>(x);
  primitive->quad[2].y = static_cast<float>(y + height);
  primitive->quad[2].z = 0.000001f;
  primitive->quad[2].rhw = 0.999999f;
  primitive->quad[2].u = 0.0f;
  primitive->quad[2].v = 0.0f;
  primitive->quad[2].diffuse = color;
  primitive->quad[3].x = static_cast<float>(x + width);
  primitive->quad[3].y = static_cast<float>(y + height);
  primitive->quad[3].z = 0.000001f;
  primitive->quad[3].rhw = 0.999999f;
  primitive->quad[3].u = 0.0f;
  primitive->quad[3].v = 0.0f;
  primitive->quad[3].diffuse = color;
  g_renderer->Submit(primitive, 513);
}
