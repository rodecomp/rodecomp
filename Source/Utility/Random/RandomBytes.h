#ifndef RAGNAROK_SOURCE_UTILITY_RANDOM_RANDOMBYTES_H
#define RAGNAROK_SOURCE_UTILITY_RANDOM_RANDOMBYTES_H

#include <cstdint>
#include <span>

namespace utility {

  void FillRandomBytes(std::span<std::uint8_t> bytes, unsigned int seed);

}

void DeriveAuxiliaryKey(const std::uint8_t* digest, std::uint8_t* key);
void GenerateLoginDigest(std::span<std::uint8_t, 16> digest, std::span<const std::uint8_t> data);
void ComputeLoginDigest(std::span<std::uint8_t, 16> digest, std::span<const std::uint8_t> data);

#endif
