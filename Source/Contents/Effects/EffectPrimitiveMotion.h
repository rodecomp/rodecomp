#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVEMOTION_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTPRIMITIVEMOTION_H

#include "Contents/Ground/TerrainHeight.h"

struct EffectPrimitiveState;
struct RenderVertex;
struct matrix;
struct vector3d;
struct RenderObjectState;

struct EffectMotionContext {
  const RenderObjectState& parentRender;
  const float& cameraPitch;
  const float& cameraYaw;
  const TerrainHeight& ground;
};

bool MoveEffectPrimitive(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context);
bool Prim3DCasting4(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context);
bool Prim3DCasting5(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context);
bool Prim3DCylinder(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context);
bool Prim3DCylinderHalf(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context);
void AdvanceEffectPrimitive(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context);
bool UpdateEffectPrimitive15(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context, int updateTick);

RenderVertex TransformVert(const matrix& view, const vector3d& position);

#endif
