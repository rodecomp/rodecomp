#ifndef RAGNAROK_SOURCE_CONTENTS_RAGEFFECTINITIALIZATION_H
#define RAGNAROK_SOURCE_CONTENTS_RAGEFFECTINITIALIZATION_H

struct RagEffectState;
struct RenderObjectState;
struct EffectActor;
struct ServiceOptions;
class ResourceMgr;

// Whole CRagEffect initialization entry, independent of its still-unrecovered
// update/render virtuals. Parent state borrows the actual render-base owner.
void InitializeRagEffect(
  RagEffectState& effect,
  RenderObjectState& render,
  EffectActor* parent,
  const RenderObjectState& parentState,
  int id,
  float x,
  float y,
  float z,
  ServiceOptions& options,
  ResourceMgr& resources
);

// Resource selection inlined in the same entry. Returns the exact STR name
// and writes only the per-effect fields changed by the selected case.
void SelectRagEffectResource(RagEffectState& effect, ServiceOptions& options, char* name);

#endif
