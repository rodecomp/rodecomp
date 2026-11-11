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
  float right = static_cast<float>((static_cast<std::int16_t>(image->doubleWidth) + 1) * image->width + cell->x - 1) * scale * cell->scaleX * extentScale;
  float top = static_cast<float>(cell->y) * scale * cell->scaleY;
  float bottom = static_cast<float>((static_cast<std::int16_t>(image->doubleHeight) + 1) * image->height - 1 + cell->y) * scale * cell->scaleY * extentScale;

  record.u0 = record.v0 = 0.0f;
  CTexture* texture;
  if (cell->imageType == 0) {
    texture = g_renderer->atlas.Find(image, palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
    }
  } else {
    texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
    record.u1 = static_cast<float>(image->width) / static_cast<float>(texture->TextureWidth());
    record.v1 = static_cast<float>(image->height) / static_cast<float>(texture->TextureHeight());
  }
  primitive->texture = texture;

  float x0;
  float y0;
  float x1;
  float y1;
  float x2;
  float y2;
  float x3;
  float y3;
  if (angle != 0.0f) {
    const float cosine = ClientCosine(static_cast<int>(angle));
    const float sine = ClientSine(static_cast<int>(angle));
    const float upper = static_cast<float>(cell->y) * scale + static_cast<float>(y);
    const float lower = static_cast<float>((static_cast<std::int16_t>(image->doubleHeight) + 1) * image->height + cell->y) * scale + static_cast<float>(y) + 1.0f;
    const int width = image->width;
    float startX;
    float endX;
    if (cell->mirrored & 1) {
      startX = static_cast<float>((static_cast<std::int16_t>(image->doubleWidth) + 1) * width + cell->x) * scale + static_cast<float>(x);
      endX = static_cast<float>(cell->x) * scale + static_cast<float>(x) + 1.0f;
    } else {
      startX = static_cast<float>(cell->x) * scale + static_cast<float>(x);
      endX = static_cast<float>((static_cast<std::int16_t>(image->doubleWidth) + 1) * width + cell->x) * scale + static_cast<float>(x) + 1.0f;
    }
    const float halfWidth = (endX - startX) * 0.5f;
    const float halfHeight = (lower - upper) * 0.5f;
    const float centerX = startX + halfWidth;
    const float centerY = upper + halfHeight;
    x0 = (-halfWidth) * cosine - (-halfHeight) * sine + centerX;
    y0 = (-halfWidth) * sine + (-halfHeight) * cosine + centerY;
    x1 = halfWidth * cosine - (-halfHeight) * sine + centerX;
    y1 = halfWidth * sine + (-halfHeight) * cosine + centerY;
    x2 = (-halfWidth) * cosine - halfHeight * sine + centerX;
    y2 = (-halfWidth) * sine + halfHeight * cosine + centerY;
    x3 = halfWidth * cosine - halfHeight * sine + centerX;
    y3 = halfWidth * sine + halfHeight * cosine + centerY;
  } else {
    if (!(cell->mirrored & 1)) {
      x0 = left;
      x1 = right;
    } else {
      x0 = right;
      x1 = left;
    }
    x0 += static_cast<float>(x);
    x1 += static_cast<float>(x);
    y0 = static_cast<float>(y) + top;
    y2 = static_cast<float>(y) + bottom;
    y1 = y0;
    x2 = x0;
    x3 = x1;
    y3 = y2;
  }

  RenderVertex* vertices = primitive->quad;
  vertices[0].x = x0 + 0.5f;
  vertices[0].y = y0 + 0.5f;
  vertices[1].x = x1 + 0.5f;
  vertices[1].y = y1 + 0.5f;
  vertices[2].x = x2 + 0.5f;
  vertices[2].y = y2 + 0.5f;
  vertices[3].x = x3 + 0.5f;
  vertices[3].y = y3 + 0.5f;

  vertices[0].z = 0.000001f;
  vertices[0].rhw = 0.999999f;
  vertices[0].u = record.u0;
  vertices[0].diffuse = color;
  vertices[0].v = record.v0;
  vertices[1].z = 0.000001f;
  vertices[1].rhw = 0.999999f;
  vertices[1].u = record.u1;
  vertices[1].diffuse = color;
  vertices[1].v = record.v0;
  vertices[2].z = 0.000001f;
  vertices[2].rhw = 0.999999f;
  vertices[2].u = record.u0;
  vertices[2].diffuse = color;
  vertices[2].v = record.v1;
  vertices[3].z = 0.000001f;
  vertices[3].rhw = 0.999999f;
  vertices[3].u = record.u1;
  vertices[3].diffuse = color;
  vertices[3].v = record.v1;

  std::vector<RenderPrimitive*>& overlays = this->m_overlayPrimitives;
  overlays.push_back(primitive);
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

  CTexture* texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
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
  const int right = static_cast<int>(((static_cast<std::int16_t>(image->doubleWidth) + 1) * image->width + cell->x) * static_cast<double>(cell->scaleX));
  const int top = static_cast<int>(cell->y * static_cast<double>(cell->scaleY));
  const int startX = (cell->mirrored & 1) ? right : left;
  const int endX = (cell->mirrored & 1) ? left : right;
  const float halfWidth = static_cast<float>(endX - startX) * 0.5f;
  const int bottom =
    static_cast<int>(((static_cast<std::int16_t>(image->doubleHeight) + 1) * image->height + cell->y) * static_cast<double>(cell->scaleY) + 1.0);
  const float halfHeight = static_cast<float>(bottom - top) * 0.5f;

  const float corners[4][2] = {
    {-halfWidth, -halfHeight},
    {halfWidth, -halfHeight},
    {-halfWidth, halfHeight},
    {halfWidth, halfHeight},
  };
  const float centerX = static_cast<float>(x + startX) + halfWidth;
  const float centerY = static_cast<float>(y + top) + halfHeight;
  const float angle = static_cast<float>(cell->rotation) / 180.0f * 3.141592f;

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
