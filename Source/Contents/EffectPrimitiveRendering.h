#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTPRIMITIVERENDERING_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTPRIMITIVERENDERING_H

struct EffectPrimitiveState;
struct RenderObjectState;
struct RenderProjection;
struct RenderQueues;
class RenderCommandPool;
struct EffectVisibilityContext;

// Borrowed original renderer/camera owners. This operation does not replace
// the particle dispatcher's visibility gates or the renderer's queue drain.
struct EffectPrimitiveRenderContext {
  const RenderProjection& projection;
  const float* cameraMatrix;
  RenderQueues& queues;
  RenderCommandPool& commands;
  const bool& depthAlpha;
  const float& depthStart;
  const float& depthEnd;
};

void DrawEffectPrimitive15(
  EffectPrimitiveState& particle,
  const RenderObjectState& render,
  EffectPrimitiveRenderContext& context
);

// Type-15 path through the particle dispatcher's parent visibility gate.
void RenderEffectPrimitive15(
  EffectPrimitiveState& particle,
  const RenderObjectState& render,
  EffectPrimitiveRenderContext& context,
  const EffectVisibilityContext& visibility
);

#endif
