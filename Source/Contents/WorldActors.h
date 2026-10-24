#ifndef RAGNAROK_SOURCE_CONTENTS_WORLDACTORS_H
#define RAGNAROK_SOURCE_CONTENTS_WORLDACTORS_H

#include <cstdint>
#include <list>

class CRenderObject;

// Borrowed CWorld actor list at +0x10, local actor at +0x2C, and client account
// ID. The list and local actor remain owned by the world; the local actor and
// list entries are nonnull, and the first matching account ID wins.
struct WorldActors {
  const std::list<CRenderObject*>& actors;
  CRenderObject* const& localPlayer;
  const std::uint32_t& localAccountId;

  CRenderObject* Find(std::uint32_t accountId) const;

  // Returns the local/matching actor position, or false with the position
  // reflected away from the local player through origin when no actor exists.
  // Successful copies are sequential; fallback inputs are read before output.
  bool TargetPosition(const float* origin, std::uint32_t accountId, float* output) const;
};

#endif
