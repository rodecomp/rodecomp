#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERQUEUES_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_RENDERQUEUES_H

#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"

#include <vector>

struct DepthPrimitive {
  /* 0000 */ float depth;
  /* 0004 */ RenderCommand* primitive;
};

struct RenderQueues {
  /* 015C */ std::vector<RenderCommand*> queue_015C;
  /* 0168 */ std::vector<RenderCommand*> queue_0168;
  /* 0174 */ std::vector<RenderCommand*> queue_0174;
  /* 0180 */ std::vector<DepthPrimitive> queue_0180;
  /* 018C */ std::vector<DepthPrimitive> queue_018C;
  /* 0198 */ std::vector<DepthPrimitive> queue_0198;
  /* 01A4 */ std::vector<DepthPrimitive> queue_01A4;
  /* 01B0 */ std::vector<RenderCommand*> queue_01B0;
  /* 01BC */ std::vector<DepthPrimitive> queue_01BC;
  /* 01C8 */ std::vector<DepthPrimitive> queue_01C8;
  /* 01D4 */ std::vector<DepthPrimitive> queue_01D4;
  /* 01E0 */ std::vector<DepthPrimitive> queue_01E0;
  /* 01EC */ std::vector<RenderCommand*> queue_01EC;
  /* 01F8 */ LightmappedQueues lightmapped;
  /* 0210 */ std::vector<RenderCommand*> queue_0210;
  /* 021C */ std::vector<DepthPrimitive> queue_021C;
  /* 0228 */ std::vector<DepthPrimitive> queue_0228;
  /* 0234 */ std::vector<DepthPrimitive> queue_0234;
  /* 0240 */ std::vector<LightmappedPrimitive*> queue_0240;

  RenderQueues();
  void Submit(RenderCommand* primitive, std::uint16_t flags, bool depthAlpha, float depthStart, float depthEnd);
  void SubmitTriangle(RenderTriangle& triangle, std::uint16_t flags, class RenderCommandPool& pool, bool depthAlpha, float depthStart, float depthEnd);
  void Clear();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DepthPrimitive) == 8);
static_assert(sizeof(RenderQueues) == 240);
static_assert(offsetof(DepthPrimitive, primitive) == 0x0004);
static_assert(offsetof(RenderQueues, lightmapped) + 0x015C == 0x01F8);
static_assert(offsetof(RenderQueues, queue_0240) + 0x015C == 0x0240);
#endif

#endif
