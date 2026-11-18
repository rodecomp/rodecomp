#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERQUEUES_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERQUEUES_H

#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"

#include <vector>

struct DepthPrimitive {
  /* 0000 */ float depth;
  /* 0004 */ RenderCommand* primitive;

  auto operator<=>(const DepthPrimitive&) const = default;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DepthPrimitive) == 8);
static_assert(offsetof(DepthPrimitive, primitive) == 0x0004);
#endif

#endif
