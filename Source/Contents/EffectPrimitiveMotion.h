#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTPRIMITIVEMOTION_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTPRIMITIVEMOTION_H

#include "Contents/TerrainHeight.h"

struct EffectPrimitiveState;
struct RenderObjectState;

struct EffectMotionContext {
  const RenderObjectState& parentRender;
  const float& cameraPitch;
  const float& cameraYaw;
  const TerrainHeight& ground;
};

bool MoveEffectPrimitive(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context);

// Type-15 branch of CEffectPrim::Update, not the full type dispatcher.
bool UpdateEffectPrimitive15(
  EffectPrimitiveState& particle,
  RenderObjectState& render,
  EffectMotionContext& context,
  int updateTick
);

#endif
