#ifndef RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDRENDERING_H
#define RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDRENDERING_H

#include <cstdint>

struct GroundQuad;
struct RenderProjection;
struct RenderQueues;
struct RenderVertex;
class LightmappedPrimitivePool;
struct ServiceOptions;
class RenderPrimitivePool;

struct GroundRenderContext {
  const RenderProjection& projection;
  RenderPrimitivePool& primitives;
  RenderQueues& queues;
  const int& width;
  const int& height;
  const std::uint32_t& frame;
  const bool& depthAlpha;
  const float& depthStart;
  const float& depthEnd;
};

struct GroundLightingContext {
  GroundRenderContext& render;
  LightmappedPrimitivePool& primitives;
  const ServiceOptions& options;
  const float* colorScale;
  const float* waterAmbient;
  const bool& overrideColor;
  const bool& multitexture;
};

bool GroundQuadVisible(const RenderVertex* vertices, const GroundRenderContext& context);
void DrawLightmappedGround(const float* matrix, const GroundQuad& quad, bool clip, GroundLightingContext& context);

void DrawGroundSide(const float* matrix, const GroundQuad& quad, bool clip, GroundRenderContext& context);

#endif
