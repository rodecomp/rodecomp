#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERQUEUES_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERQUEUES_H

#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"

#include <cstdint>
#include <vector>

enum RenderSubmitFlags : std::uint16_t {
  RSF_ALPHA = 0x1,
  RSF_ADDITIVE = 0x2,
  RSF_CUSTOM_BLEND = 0x4,
  RSF_TOPMOST = 0x8,
  RSF_ALPHA_REF_HIGH = 0x10,
  RSF_SECONDARY = 0x20,
  RSF_DEPTH_SORTED = 0x100,
  RSF_NO_DEPTH = 0x200,
  RSF_PRE_OPAQUE = 0x800,
  RSF_POST_OPAQUE = 0x1000,
};

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
