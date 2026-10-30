#ifndef RAGNAROK_SOURCE_UTILITY_RANDOM_RANDOMBYTES_H
#define RAGNAROK_SOURCE_UTILITY_RANDOM_RANDOMBYTES_H

#include <cstdint>
#include <span>

namespace utility {

  void FillRandomBytes(std::span<std::uint8_t> bytes, unsigned int seed);

}

#endif
