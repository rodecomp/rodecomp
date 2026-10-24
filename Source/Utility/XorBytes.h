#ifndef RAGNAROK_SOURCE_UTILITY_XORBYTES_H
#define RAGNAROK_SOURCE_UTILITY_XORBYTES_H

#include <cstdint>

namespace utility {

  // Reconstructed semantic name; the original symbol is unavailable.
  void XorBytes(std::uint8_t* bytes, std::uint64_t length, std::uint64_t key);

}

#endif
