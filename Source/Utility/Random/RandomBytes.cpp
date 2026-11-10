#include "Utility/Random/RandomBytes.h"

#include <random>

namespace {
  struct RandomByteSource {
    /* 0000 */ std::mt19937 m_engine;
    /* 1388 */ std::uniform_int_distribution<int> m_distribution;

    // INLINED
    explicit RandomByteSource(unsigned int seed) : m_engine(seed), m_distribution(0, 255) {
    }

    // INLINED
    std::uint8_t Next() {
      return static_cast<std::uint8_t>(this->m_distribution(this->m_engine));
    }
  };
}

namespace utility {
  // OFFSET: 0x0061CBE0
  // FUNCTION: fn_04427
  void FillRandomBytes(std::span<std::uint8_t> bytes, unsigned int seed) {
    RandomByteSource source(seed);
    for (std::uint8_t& byte : bytes) {
      byte = source.Next();
    }
  }
}
