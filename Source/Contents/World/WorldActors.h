#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDACTORS_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_WORLDACTORS_H

#include <cstdint>
#include <list>

class CRenderObject;

struct WorldActors {
  const std::list<CRenderObject*>& actors;
  CRenderObject* const& localPlayer;
  const std::uint32_t& localAccountId;

  CRenderObject* Find(std::uint32_t accountId) const;

  bool TargetPosition(const float* origin, std::uint32_t accountId, float* output) const;
};

#endif
