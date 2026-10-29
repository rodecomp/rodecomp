#ifndef RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTYMEMBEREFFECTS_H
#define RAGNAROK_SOURCE_CONTENTS_EFFECTS_PARTYMEMBEREFFECTS_H

#include <cstdint>
#include <list>

struct EffectActor;
class CRenderObject;
struct ServiceOptions;
class ResourceMgr;
class LuaScript;

struct PartyEffectMode {
  const int& playerOption48;
  const int& playerOption54;
  std::list<EffectActor*>& actors;
  std::list<CRenderObject*>& objects;
  ResourceMgr& resources;
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
