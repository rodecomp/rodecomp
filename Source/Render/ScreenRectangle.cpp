#include "Render/ScreenRectangle.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

// OFFSET: 0x0054E590
void DrawBlendedRectangle(
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
    vertex.rhw = 0.000001f;
    vertex.diffuse = color;
    vertex.u = 0.0f;
    vertex.v = 0.0f;
  }
  primitive->unknown_0018 = 5;
  primitive->unknown_001C = 6;
  queues.Submit(primitive, 517, depthAlpha, depthStart, depthEnd);
}

// OFFSET: 0x0054ED40
void DrawTexturedRectangle(
  RenderPrimitivePool& primitives, TextureCache& textures, RenderQueues& queues, int x, int y, int width, int height, std::uint32_t color, CTexture* texture,
  bool depthAlpha, float depthStart, float depthEnd
) {
  if ((color & 0xFF000000u) == 0) {
    return;
  }

  RenderPrimitive* primitive = primitives.Acquire();
  primitive->texture = texture ? texture : textures.Default();
  const float left = static_cast<float>(x);
  const float top = static_cast<float>(y);
  const float right = static_cast<float>(x + width);
  const float bottom = static_cast<float>(y + height);
  primitive->quad[0].x = left;
  primitive->quad[0].y = top;
  primitive->quad[0].u = 0.0f;
  primitive->quad[0].v = 0.0f;
  primitive->quad[1].x = right;
  primitive->quad[1].y = top;
  primitive->quad[1].u = 1.0f;
  primitive->quad[1].v = 0.0f;
  primitive->quad[2].x = left;
  primitive->quad[2].y = bottom;
  primitive->quad[2].u = 0.0f;
  primitive->quad[2].v = 1.0f;
  primitive->quad[3].x = right;
  primitive->quad[3].y = bottom;
  primitive->quad[3].u = 1.0f;
  primitive->quad[3].v = 1.0f;
  for (RenderVertex& vertex : primitive->quad) {
    vertex.z = 0.000001f;
    vertex.rhw = 0.999999f;
    vertex.diffuse = color;
  }
  queues.Submit(primitive, 513, depthAlpha, depthStart, depthEnd);
}

// OFFSET: 0x0054EFD0
void DrawTextureRegion(
  RenderPrimitivePool& primitives, TextureCache& textures, RenderQueues& queues, float x, float y, float width, float height, std::uint32_t color,
  CTexture* texture, bool depthAlpha, float depthStart, float depthEnd
) {
  if ((color & 0xFF000000u) == 0) {
    return;
  }

  RenderPrimitive* primitive = primitives.Acquire();
  primitive->texture = texture ? texture : textures.Default();
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
  queues.Submit(primitive, 513, depthAlpha, depthStart, depthEnd);
}
