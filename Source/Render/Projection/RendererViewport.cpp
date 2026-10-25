#include "Render/Projection/RendererViewport.h"

#include <bit>
#include <cmath>

// OFFSET: 0x0055F6A0
// FUNCTION: fn_02118
void RendererViewport::SetSize(int columns, int rows, const float& projectionScale, float& heightScale, float& tangent, float& cotangent) {
  this->height = rows;
  this->centerX = columns / 2;
  this->width = columns;
  this->centerY = rows / 2;

  const float aspect = static_cast<float>(rows) / static_cast<float>(columns);
  this->aspect = aspect;
  const float narrow = static_cast<float>(std::tan(0.13089966773986816));
  this->scaleY = static_cast<float>(rows / 2) / narrow;
  const float wide = static_cast<float>(std::tan(1.4398963451385498));
  this->clipCenterY = rows / 2;
  this->clipCenterX = columns / 2;
  this->vertical = wide;
  this->horizontal = wide * aspect;
  this->scaleX = (static_cast<float>(columns / 2) / narrow) * aspect;

  this->uiScaleX = static_cast<float>(columns) / 640.0f;
  const float height = (static_cast<float>(rows) * 1.34f) / 768.0f;
  this->uiScaleY = static_cast<float>(rows) / 730.0f;
  heightScale = height;
  tangent = projectionScale / this->uiScaleY;
  cotangent = projectionScale / this->uiScaleY;
}

RenderProjection RendererViewport::Projection() const {
  return {this->scaleX, this->scaleY, this->centerX, this->centerY};
}
