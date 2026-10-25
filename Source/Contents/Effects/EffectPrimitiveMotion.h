#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVEMOTION_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVEMOTION_H

#include "Contents/Ground/TerrainHeight.h"

struct EffectPrimitiveState;
struct RenderObjectState;

struct EffectMotionContext {
  const RenderObjectState& parentRender;
  const float& cameraPitch;
  const float& cameraYaw;
  const TerrainHeight& ground;
};

bool MoveEffectPrimitive(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context);
bool UpdateEffectPrimitive15(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context, int updateTick);

#endif
