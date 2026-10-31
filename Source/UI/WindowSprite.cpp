#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/UIWindow.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00A7A0C0
// FUNCTION: fn_16922
void UIWindow::AddSpriteCellOverlay(
  int x, int y, int, int, const SpriteRleImage* image, const SpriteFrameCell* cell, float scale, float angle, std::uint32_t color,
  const std::uint32_t* palette, float extentScale
) {
  x += this->geometry.x;
  y += this->geometry.y;

  SpriteAtlasRecord record{};
  RenderPrimitive* primitive = g_renderer->primitives.Acquire();

  float left = static_cast<float>(cell->x) * scale * cell->scaleX;
  float right = static_cast<float>((image->unknown_0004 + 1) * image->width + cell->x - 1) * scale * cell->scaleX * extentScale;
  float top = static_cast<float>(cell->y) * scale * cell->scaleY;
  float bottom = static_cast<float>((image->unknown_0006 + 1) * image->height - 1 + cell->y) * scale * cell->scaleY * extentScale;

  CTexture* texture;
  if (cell->imageType == 0) {
    texture = g_renderer->atlas.Find(image, palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
    }
  } else {
    texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
    record.u1 = static_cast<float>(image->width) / static_cast<float>(texture->TextureWidth());
    record.v1 = static_cast<float>(image->height) / static_cast<float>(texture->TextureHeight());
  }
  primitive->texture = texture;

  RenderVertex* vertices = primitive->quad;
  if (angle != 0.0f) {
    const float cosine = ClientCosine(static_cast<int>(angle));
    const float sine = ClientSine(static_cast<int>(angle));
    const int farX = (image->unknown_0004 + 1) * image->width + cell->x;
    const float startX = static_cast<float>((cell->mirrored & 1) ? farX : cell->x) * scale + static_cast<float>(x);
    const float endX = static_cast<float>((cell->mirrored & 1) ? cell->x : farX) * scale + static_cast<float>(x) + 1.0f;
    const float upper = static_cast<float>(cell->y) * scale + static_cast<float>(y);
    const float lower = static_cast<float>((image->unknown_0006 + 1) * image->height + cell->y) * scale + static_cast<float>(y) + 1.0f;
    const float halfWidth = (endX - startX) * 0.5f;
    const float halfHeight = (lower - upper) * 0.5f;
    const float centerX = startX + halfWidth;
    const float centerY = upper + halfHeight;
    const float minX = -halfWidth;
    const float minY = -halfHeight;

    vertices[0].x = minX * cosine - minY * sine + centerX + 0.5f;
    vertices[0].y = minY * cosine + minX * sine + centerY + 0.5f;
    vertices[1].x = halfWidth * cosine - minY * sine + centerX + 0.5f;
    vertices[1].y = minY * cosine + halfWidth * sine + centerY + 0.5f;
    vertices[2].x = minX * cosine - halfHeight * sine + centerX + 0.5f;
    vertices[2].y = halfHeight * cosine + minX * sine + centerY + 0.5f;
    vertices[3].x = halfWidth * cosine - halfHeight * sine + centerX + 0.5f;
    vertices[3].y = halfHeight * cosine + halfWidth * sine + centerY + 0.5f;
  } else {
    if (cell->mirrored & 1) {
      const float swap = left;
      left = right;
      right = swap;
    }
    left += static_cast<float>(x);
    right += static_cast<float>(x);
    top += static_cast<float>(y);
    bottom += static_cast<float>(y);

    vertices[0].x = left + 0.5f;
    vertices[0].y = top + 0.5f;
    vertices[1].x = right + 0.5f;
    vertices[1].y = top + 0.5f;
    vertices[2].x = left + 0.5f;
    vertices[2].y = bottom + 0.5f;
    vertices[3].x = right + 0.5f;
    vertices[3].y = bottom + 0.5f;
  }

  for (RenderVertex& vertex : primitive->quad) {
    vertex.z = 0.000001f;
    vertex.rhw = 0.999999f;
    vertex.diffuse = color;
  }
  vertices[0].u = record.u0;
  vertices[0].v = record.v0;
  vertices[1].u = record.u1;
  vertices[1].v = record.v0;
  vertices[2].u = record.u0;
  vertices[2].v = record.v1;
  vertices[3].u = record.u1;
  vertices[3].v = record.v1;

  this->m_overlayPrimitives.push_back(primitive);
}

// OFFSET: 0x00A7A5C0
// FUNCTION: fn_16923
void UIWindow::AddRotatedSpriteCellOverlay(
  int x, int y, int width, int height, const SpriteRleImage* image, const SpriteFrameCell* cell, const std::uint32_t* palette
) {
  if (!width || !height || !image || !cell || !palette) {
    return;
  }

  for (UIWindow* window = this; window; window = window->parent) {
    x += window->geometry.x;
    y += window->geometry.y;
  }

  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  SpriteAtlasRecord record{};

  CTexture* texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
  if (!texture) {
    texture = g_renderer->atlas.Find(image, palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
    }
  } else {
    record.u0 = 0.0f;
    record.v0 = 0.0f;
    record.u1 = static_cast<float>(texture->ImageWidth()) / static_cast<float>(texture->TextureWidth());
    record.v1 = static_cast<float>(texture->ImageHeight()) / static_cast<float>(texture->TextureHeight());
  }
  primitive->texture = texture;

  const int left = static_cast<int>(cell->x * static_cast<double>(cell->scaleX) + 1.0);
  const int right = static_cast<int>(((static_cast<std::int16_t>(image->unknown_0004) + 1) * image->width + cell->x) * static_cast<double>(cell->scaleX));
  const int top = static_cast<int>(cell->y * static_cast<double>(cell->scaleY));
  const int startX = (cell->mirrored & 1) ? right : left;
  const int endX = (cell->mirrored & 1) ? left : right;
  const float halfWidth = static_cast<float>(endX - startX) * 0.5f;
  const int bottom =
    static_cast<int>(((static_cast<std::int16_t>(image->unknown_0006) + 1) * image->height + cell->y) * static_cast<double>(cell->scaleY) + 1.0);
  const float halfHeight = static_cast<float>(bottom - top) * 0.5f;

  const float corners[4][2] = {
    {-halfWidth, -halfHeight},
    {halfWidth, -halfHeight},
    {-halfWidth, halfHeight},
    {halfWidth, halfHeight},
  };
  const float centerX = static_cast<float>(x + startX) + halfWidth;
  const float centerY = static_cast<float>(y + top) + halfHeight;
  const float angle = static_cast<float>(cell->unknown_001C) / 180.0f * 3.141592f;

  const unsigned char* rgba = reinterpret_cast<const unsigned char*>(&cell->color);
  RenderVertex* vertices = primitive->quad;
  for (unsigned int i = 0; i < 4; ++i) {
    const float cornerX = corners[i][0];
    const float cornerY = corners[i][1];
    vertices[i].x = ClientCos(angle) * cornerX - ClientSin(angle) * cornerY + centerX + 0.5f;
    vertices[i].y = ClientCos(angle) * cornerY + ClientSin(angle) * cornerX + centerY + 0.5f;
    vertices[i].z = 0.000001f;
    vertices[i].rhw = 0.999999f;
    vertices[i].diffuse = (((((rgba[3] << 8) | rgba[0]) << 8) | rgba[1]) << 8) | rgba[2];
  }
  vertices[0].u = record.u0;
  vertices[0].v = record.v0;
  vertices[1].u = record.u1;
  vertices[1].v = record.v0;
  vertices[2].u = record.u0;
  vertices[2].v = record.v1;
  vertices[3].u = record.u1;
  vertices[3].v = record.v1;

  for (UIWindow* window = this->parent; window; window = window->parent) {
    if (!window->parent) {
      window->m_overlayPrimitives.push_back(primitive);
    }
  }
}
