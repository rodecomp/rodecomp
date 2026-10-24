#ifndef RAGNAROK_SOURCE_CONTENTS_PARTYMEMBEREFFECTS_H
#define RAGNAROK_SOURCE_CONTENTS_PARTYMEMBEREFFECTS_H

#include <cstdint>
#include <list>

struct EffectActor;
class CRenderObject;
struct JobNames;
struct ServiceOptions;
class ResourceMgr;
class LuaScript;

// Borrowed original active-mode/player fields and owning mode lists. The
// enclosing mode supplies actual owners; this does not construct a fake mode.
struct PartyEffectMode {
  const int& playerOption48;
  const int& playerOption54;
  std::list<EffectActor*>& actors;
  std::list<CRenderObject*>& objects;
  ResourceMgr& resources;
  JobNames& names;
  ServiceOptions& options;
  LuaScript& effectScript;
  int& lastEffectId;
};

struct PartyMemberEffects {
  PartyEffectMode* const& activeMode;

  bool Enabled() const;
  void EnsurePartyEffect(std::uint32_t accountId);
};

#endif
