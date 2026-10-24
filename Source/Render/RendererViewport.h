#ifndef RAGNAROK_SOURCE_RENDER_RENDERERVIEWPORT_H
#define RAGNAROK_SOURCE_RENDER_RENDERERVIEWPORT_H

#include "Render/RenderProjection.h"

// Contiguous CRenderer fields +0x04..+0x37. Projection and clipping centers
// are distinct stored pairs even though this initialization sets them equally.
struct RendererViewport {
  float scaleX;
  float scaleY;
  float horizontal;
  float vertical;
  float aspect;
  float uiScaleX;
  float uiScaleY;
  int clipCenterX;
  int clipCenterY;
  int width;
  int height;
  int centerX;
  int centerY;

  void
  SetSize(int width, int height, const float& projectionScale, float& heightScale, float& tangent, float& cotangent);
  RenderProjection Projection() const;
};

static_assert(sizeof(RendererViewport) == 52);

#endif
