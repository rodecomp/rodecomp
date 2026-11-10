#include "Render/ScreenRectangle.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

// OFFSET: 0x0054E590
// FUNCTION: fn_01953
void DrawBlendedRectangle(int x, int y, int width, int height, std::uint32_t color) {
  if ((color & 0xFF000000u) == 0) {
    return;
  }

  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  primitive->texture = g_texMgr.Default();
  primitive->quad[0].x = static_cast<float>(x);
  primitive->quad[0].y = static_cast<float>(y);
  primitive->quad[0].z = 0.000001f;
  primitive->quad[0].rhw = 0.000001f;
  primitive->quad[0].u = 0.0f;
  primitive->quad[0].v = 0.0f;
  primitive->quad[0].diffuse = color;
  primitive->quad[1].x = static_cast<float>(x + width);
  primitive->quad[1].y = static_cast<float>(y);
  primitive->quad[1].z = 0.000001f;
  primitive->quad[1].rhw = 0.000001f;
  primitive->quad[1].u = 0.0f;
  primitive->quad[1].v = 0.0f;
  primitive->quad[1].diffuse = color;
  primitive->quad[2].x = static_cast<float>(x);
  primitive->quad[2].y = static_cast<float>(y + height);
  primitive->quad[2].z = 0.000001f;
  primitive->quad[2].rhw = 0.000001f;
  primitive->quad[2].u = 0.0f;
  primitive->quad[2].v = 0.0f;
  primitive->quad[2].diffuse = color;
  primitive->quad[3].x = static_cast<float>(x + width);
  primitive->quad[3].y = static_cast<float>(y + height);
  primitive->quad[3].z = 0.000001f;
  primitive->quad[3].rhw = 0.000001f;
  primitive->quad[3].u = 0.0f;
  primitive->quad[3].v = 0.0f;
  primitive->quad[3].diffuse = color;
  primitive->sourceBlend = 5;
  primitive->destinationBlend = 6;
  g_renderer->queues.Submit(primitive, 517, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}

// OFFSET: 0x0054ED40
// FUNCTION: fn_01954
void DrawTexturedRectangle(int x, int y, int width, int height, std::uint32_t color, CTexture* texture) {
  if ((color & 0xFF000000u) == 0) {
    return;
  }

  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  primitive->texture = texture ? texture : g_texMgr.Default();
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
  primitive->quad[1].u = 1.0f;
  primitive->quad[1].v = 0.0f;
  primitive->quad[1].diffuse = color;
  primitive->quad[2].x = static_cast<float>(x);
  primitive->quad[2].y = static_cast<float>(y + height);
  primitive->quad[2].z = 0.000001f;
  primitive->quad[2].rhw = 0.999999f;
  primitive->quad[2].u = 0.0f;
  primitive->quad[2].v = 1.0f;
  primitive->quad[2].diffuse = color;
  primitive->quad[3].x = static_cast<float>(x + width);
  primitive->quad[3].y = static_cast<float>(y + height);
  primitive->quad[3].z = 0.000001f;
  primitive->quad[3].rhw = 0.999999f;
  primitive->quad[3].u = 1.0f;
  primitive->quad[3].v = 1.0f;
  primitive->quad[3].diffuse = color;
  g_renderer->queues.Submit(primitive, 513, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}

// OFFSET: 0x0054EFD0
// FUNCTION: fn_01955
void DrawTextureRegion(float x, float y, float width, float height, std::uint32_t color, CTexture* texture) {
  if ((color & 0xFF000000u) == 0) {
    return;
  }

  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  primitive->texture = texture ? texture : g_texMgr.Default();
  primitive->quad[0].x = x;
  primitive->quad[0].y = y;
  primitive->quad[0].u = g_textureOffsetX / static_cast<float>(texture->TextureWidth());
  primitive->quad[0].v = g_textureOffsetY / static_cast<float>(texture->TextureHeight());
  primitive->quad[1].x = x + width;
  primitive->quad[1].y = y;
  primitive->quad[1].u = (static_cast<float>(texture->Width()) + g_textureOffsetX) / static_cast<float>(texture->TextureWidth());
  primitive->quad[1].v = g_textureOffsetY / static_cast<float>(texture->TextureHeight());
  primitive->quad[2].x = x;
  primitive->quad[2].y = y + height;
  primitive->quad[2].u = g_textureOffsetX / static_cast<float>(texture->TextureWidth());
  primitive->quad[2].v = (static_cast<float>(texture->Height()) + g_textureOffsetY) / static_cast<float>(texture->TextureHeight());
  primitive->quad[3].x = x + width;
  primitive->quad[3].y = y + height;
  primitive->quad[3].u = (static_cast<float>(texture->Width()) + g_textureOffsetX) / static_cast<float>(texture->TextureWidth());
  primitive->quad[3].v = (static_cast<float>(texture->Height()) + g_textureOffsetY) / static_cast<float>(texture->TextureHeight());
  for (RenderVertex& vertex : primitive->quad) {
    vertex.z = 0.000001f;
    vertex.rhw = 0.999999f;
    vertex.diffuse = color;
  }
  g_renderer->queues.Submit(primitive, 513, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}
