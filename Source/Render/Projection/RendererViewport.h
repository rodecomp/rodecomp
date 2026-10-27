#ifndef RAGNAROK_SOURCE_RENDER_PROJECTION_RENDERERVIEWPORT_H
#define RAGNAROK_SOURCE_RENDER_PROJECTION_RENDERERVIEWPORT_H

#include <cstddef>
#include "Render/Projection/RenderProjection.h"

struct RendererViewport {
  /* 0004 */ float scaleX;
  /* 0008 */ float scaleY;
  /* 000C */ float horizontal;
  /* 0010 */ float vertical;
  /* 0014 */ float aspect;
  /* 0018 */ float uiScaleX;
  /* 001C */ float uiScaleY;
  /* 0020 */ int clipCenterX;
  /* 0024 */ int clipCenterY;
  /* 0028 */ int width;
  /* 002C */ int height;
  /* 0030 */ int centerX;
  /* 0034 */ int centerY;

  void SetSize(int width, int height, const float& projectionScale, float& heightScale, float& tangent, float& cotangent);
  RenderProjection Projection() const;
};

static_assert(sizeof(RendererViewport) == 52);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(RendererViewport, width) == 0x24);
static_assert(offsetof(RendererViewport, centerY) == 0x30);
#endif

#endif
