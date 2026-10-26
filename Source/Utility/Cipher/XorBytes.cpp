#include "Utility/Cipher/XorBytes.h"

namespace utility {
  // OFFSET: 0x005A45E0
  // FUNCTION: fn_03149
  void XorBytes(std::uint8_t* bytes, std::uint64_t length, std::uint64_t key) {
    for (std::uint64_t i = 0; i < length; ++i) {
      bytes[i] ^= static_cast<std::uint8_t>(key >> (8 * (i & 7)));
    }
  }
}
