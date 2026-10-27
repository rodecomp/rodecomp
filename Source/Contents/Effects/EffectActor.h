#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTACTOR_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_EFFECTACTOR_H

#include "Contents/Effects/EffectAttachments.h"
#include "Contents/Actors/RenderObject.h"

#include <cstddef>

struct EffectActorState {
  EffectActorState();
  ~EffectActorState();
  EffectAttachments Attachments();

  /* 0138 */ int unknown_0138;
  /* 013C */ std::uint32_t unknown_013C;
  /* 0140 */ std::uint32_t unknown_0140;
  /* 0144 */ std::list<RagEffectState*> effects;
  /* 014C */ RagEffectState* first;
  /* 0150 */ RagEffectState* second;
};

struct EffectActor : CRenderObject {
  EffectActor(ResourceMgr& resources, JobNames& names);
  bool EffectVisible() const;

  /* 0138 */ EffectActorState state;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EffectActorState) == 28);
static_assert(sizeof(EffectActor) == 0x158);
static_assert(offsetof(EffectActor, state) == 0x138);
#endif

#endif
