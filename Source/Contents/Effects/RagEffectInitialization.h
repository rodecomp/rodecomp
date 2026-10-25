#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECTINITIALIZATION_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_RAGEFFECTINITIALIZATION_H

struct RagEffectState;
struct RenderObjectState;
struct EffectActor;
struct ServiceOptions;
class ResourceMgr;

void InitializeRagEffect(
  RagEffectState& effect, RenderObjectState& render, EffectActor* parent, const RenderObjectState& parentState, int id, float x, float y, float z,
  ServiceOptions& options, ResourceMgr& resources
);

#endif
