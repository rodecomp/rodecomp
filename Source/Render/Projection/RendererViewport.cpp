#include "Render/Drawing/SpriteGeometry.h"
#include "Render/Renderer.h"

#include <bit>
#include <cmath>

// OFFSET: 0x0055F6A0
// FUNCTION: fn_02118
void CRenderer::SetSize(int columns, int rows) {
  this->viewport.height = rows;
  this->viewport.centerX = columns / 2;
  this->viewport.width = columns;
  this->viewport.centerY = rows / 2;

  const float aspect = static_cast<float>(rows) / static_cast<float>(columns);
  this->viewport.aspect = aspect;
  const float narrow = static_cast<float>(std::tan(0.13089966773986816));
  this->viewport.scaleY = static_cast<float>(rows / 2) / narrow;
  const float wide = static_cast<float>(std::tan(1.4398963451385498));
  this->viewport.clipCenterY = rows / 2;
  this->viewport.clipCenterX = columns / 2;
  this->viewport.vertical = wide;
  this->viewport.scaleX = (static_cast<float>(columns / 2) / narrow) * aspect;
  this->viewport.horizontal = wide * aspect;

  this->viewport.uiScaleX = static_cast<float>(columns) / 640.0f;
  const float height = (static_cast<float>(rows) * 1.34f) / 768.0f;
  this->viewport.uiScaleY = static_cast<float>(rows) / 480.0f;
  g_heightScale = height;
  g_tangent = g_projectionScale / this->viewport.uiScaleY;
  g_cotangent = g_projectionScale / this->viewport.uiScaleY;
}
