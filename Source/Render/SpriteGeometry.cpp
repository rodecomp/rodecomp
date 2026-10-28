#include "Contents/Actors/SpriteDrawingPolicy.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"
#include "Render/Textures/SpritePixels.h"

#include <windows.h>

// GLOBAL: gl_24537
float g_tangent = 0.16f;
// GLOBAL: gl_24577
float g_cameraScaleY = 0.000009f;
// GLOBAL: gl_24578
float g_cameraScaleX = 0.000036f;
// GLOBAL: gl_24535
float g_heightScale = 1.34f;

// OFFSET: 0x0080D7B0
// FUNCTION: fn_10102
bool SpriteDrawingPolicy::ProjectCell(
  int unused, int layer, float scale, const SpriteFrameCell& cell, float x, float y, const float* position, float shift, float& bottomDepth,
  float& topDepth, const SpriteRleImage& image, SpriteCellQuad& quad, tagRECT& bounds, SpriteColor& color, int offsetX, int offsetY, int flags
) const {
  const float depthBias = scale / g_heightScale * g_cameraScaleX;
  const double scaleY = scale * cell.scaleY;
  const double scaleX = scale * cell.scaleX;
  const float top = (cell.y + offsetY) * scaleY + y;
  const float bottom = ((image.unknown_0006 + 1) * image.height + cell.y + offsetY) * scaleY + y + 1.0;
  float left;
  float right;

  if (cell.mirrored & 1) {
    left = ((image.unknown_0004 + 1) * image.width + cell.x + offsetX) * scaleX + x;
    right = (offsetX + cell.x) * scaleX + x + 1.0;

    if (top >= g_renderer->viewport.height || bottom <= 0.0f || right >= g_renderer->viewport.width || left <= 0.0f) {
      return false;
    }
  } else {
    left = (cell.x + offsetX) * scaleX + x;
    right = ((image.unknown_0004 + 1) * image.width + cell.x + offsetX) * scaleX + x + 1.0;

    if (top >= g_renderer->viewport.height || bottom <= 0.0f || left >= g_renderer->viewport.width || right <= 0.0f) {
      return false;
    }
  }

  bottomDepth = 1.0f / ((bottom - y) * g_tangent + position[2]) + (layer + 3) * g_cameraScaleY + depthBias;
  topDepth = 1.0f / ((top - y) * g_tangent + position[2]) + (layer + 3) * g_cameraScaleY + depthBias;
  quad.left = left + shift;
  quad.top = top;
  quad.bottom = bottom;
  quad.right = right + shift;
  quad.bottomDepth = bottomDepth;
  quad.topDepth = topDepth;
  quad.angle = static_cast<float>(cell.unknown_001C);
  bounds.left = static_cast<LONG>(left);
  bounds.top = static_cast<LONG>(top);
  bounds.right = static_cast<LONG>(right);
  bounds.bottom = static_cast<LONG>(bottom);
  color.alpha = static_cast<std::uint8_t>((cell.color >> 24) * 255.0f / 255.0f);
  color.red = static_cast<std::uint8_t>((cell.color & 0xFF) * 255.0f / 255.0f);
  color.green = static_cast<std::uint8_t>((cell.color >> 8 & 0xFF) * 255.0f / 255.0f);
  color.blue = static_cast<std::uint8_t>((cell.color >> 16 & 0xFF) * 255.0f / 255.0f);
  quad.color = color;

  return true;
}
