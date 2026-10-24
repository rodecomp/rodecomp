#ifndef RAGNAROK_SOURCE_RENDER_RENDERQUEUES_H
#define RAGNAROK_SOURCE_RENDER_RENDERQUEUES_H

#include "Render/LightmappedPrimitive.h"
#include "Render/RenderPrimitive.h"

#include <vector>

struct DepthPrimitive {
  float depth;
  RenderCommand* primitive;
};

// Recovered renderer fields +0x15C..+0x24B. Byte suffixes retain unknown
// original queue meanings; commands and their vertex storage remain borrowed.
struct RenderQueues {
  std::vector<RenderCommand*> queue_015C;
  std::vector<RenderCommand*> queue_0168;
  std::vector<RenderCommand*> queue_0174;
  std::vector<DepthPrimitive> queue_0180;
  std::vector<DepthPrimitive> queue_018C;
  std::vector<DepthPrimitive> queue_0198;
  std::vector<DepthPrimitive> queue_01A4;
  std::vector<RenderCommand*> queue_01B0;
  std::vector<DepthPrimitive> queue_01BC;
  std::vector<DepthPrimitive> queue_01C8;
  std::vector<DepthPrimitive> queue_01D4;
  std::vector<DepthPrimitive> queue_01E0;
  std::vector<RenderCommand*> queue_01EC;
  LightmappedQueues lightmapped;
  std::vector<RenderCommand*> queue_0210;
  std::vector<DepthPrimitive> queue_021C;
  std::vector<DepthPrimitive> queue_0228;
  std::vector<DepthPrimitive> queue_0234;
  std::vector<LightmappedPrimitive*> queue_0240;

  RenderQueues();
  void Submit(RenderCommand* primitive, std::uint16_t flags, bool depthAlpha, float depthStart, float depthEnd);
  void SubmitTriangle(
    RenderTriangle& triangle,
    std::uint16_t flags,
    class RenderCommandPool& pool,
    bool depthAlpha,
    float depthStart,
    float depthEnd
  );
  void Clear();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(DepthPrimitive) == 8);
static_assert(sizeof(RenderQueues) == 240);
#endif

#endif
