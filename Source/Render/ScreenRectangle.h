#ifndef RAGNAROK_SOURCE_RENDER_SCREENRECTANGLE_H
#define RAGNAROK_SOURCE_RENDER_SCREENRECTANGLE_H

#include <cstdint>

class CTexture;
class RenderPrimitivePool;
class TextureCache;
struct RenderQueues;

void DrawBlendedRectangle(
  RenderPrimitivePool& primitives, TextureCache& textures, RenderQueues& queues, int x, int y, int width, int height, std::uint32_t color, bool depthAlpha,
  float depthStart, float depthEnd
);
void DrawTexturedRectangle(
  RenderPrimitivePool& primitives, TextureCache& textures, RenderQueues& queues, int x, int y, int width, int height, std::uint32_t color, CTexture* texture,
  bool depthAlpha, float depthStart, float depthEnd
);
void DrawTextureRegion(
  RenderPrimitivePool& primitives, TextureCache& textures, RenderQueues& queues, float x, float y, float width, float height, std::uint32_t color,
  CTexture* texture, bool depthAlpha, float depthStart, float depthEnd
);

#endif
