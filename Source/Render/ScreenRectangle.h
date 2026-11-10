#ifndef RAGNAROK_SOURCE_RENDER_SCREENRECTANGLE_H
#define RAGNAROK_SOURCE_RENDER_SCREENRECTANGLE_H

#include <cstdint>

class CTexture;
class RenderPrimitivePool;
class TextureCache;
struct RenderQueues;

void DrawBlendedRectangle(int x, int y, int width, int height, std::uint32_t color);
void DrawTexturedRectangle(int x, int y, int width, int height, std::uint32_t color, CTexture* texture);
void DrawTextureRegion(float x, float y, float width, float height, std::uint32_t color, CTexture* texture);

#endif
