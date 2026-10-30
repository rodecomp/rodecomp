#include "Utility/Random/RandomBytes.h"

#include <random>

namespace utility {
  // OFFSET: 0x0061CBE0
  // FUNCTION: fn_04427
  void FillRandomBytes(std::span<std::uint8_t> bytes, unsigned int seed) {
    std::mt19937 engine(seed);
    std::uniform_int_distribution<int> distribution(0, 255);
    for (std::uint8_t& byte : bytes) {
      byte = static_cast<std::uint8_t>(distribution(engine));
    }
  }
}
