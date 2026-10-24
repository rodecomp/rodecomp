#ifndef RAGNAROK_SOURCE_CONTENTS_WORLDPROXIMITY_H
#define RAGNAROK_SOURCE_CONTENTS_WORLDPROXIMITY_H

#include <cstdint>
#include <functional>

class CConnection;
struct PacketLengths;

// CWorld's byte at +0xF4 suppresses repeated proximity notifications. Exactly
// 1 means notified; other byte values take the inactive branch. The predicate
// is owned by this call and runs before the account ID is read or state changes.
// An empty predicate throws std::bad_function_call without changing the byte.
void UpdateWorldProximityNotification(
  unsigned char& notified,
  std::function<bool()> nearby,
  CConnection& connection,
  const PacketLengths& lengths,
  const std::uint32_t& accountId
);

#endif
