#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVERENDERING_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVERENDERING_H

struct EffectPrimitiveState;
struct RenderObjectState;
struct RenderProjection;
struct RenderQueues;
class RenderCommandPool;
struct EffectVisibilityContext;

struct EffectPrimitiveRenderContext {
  const RenderProjection& projection;
  const float* cameraMatrix;
  RenderQueues& queues;
  RenderCommandPool& commands;
  const bool& depthAlpha;
  const float& depthStart;
  const float& depthEnd;
};

void DrawEffectPrimitive15(EffectPrimitiveState& particle, const RenderObjectState& render, EffectPrimitiveRenderContext& context);
void RenderEffectPrimitive15(
  EffectPrimitiveState& particle, const RenderObjectState& render, EffectPrimitiveRenderContext& context, const EffectVisibilityContext& visibility
);

#endif
