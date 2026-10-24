#include "Client/ClientOptions.h"

#include <bit>

// OFFSET: 0x0091BE20
// FUNCTION: fn_12610
int ClientOptions::Get(int id) const {
  return std::bit_cast<int>(static_cast<std::uint32_t>(this->values.Get(id)));
}

// OFFSET: 0x005E7970
// FUNCTION: fn_03937
void ClientOptions::Set(int id, int value, int extra) {
  const std::uint64_t bits =
    static_cast<std::uint32_t>(value) | (static_cast<std::uint64_t>(static_cast<std::uint32_t>(extra)) << 32);
  this->values.Set(id, std::bit_cast<std::int64_t>(bits));
}
