#ifndef RAGNAROK_SOURCE_UTILITY_COMPRESSION_H
#define RAGNAROK_SOURCE_UTILITY_COMPRESSION_H

#include <cstdint>
#include <span>

namespace utility {

  int DecompressLz4(std::span<const std::uint8_t> input, void* output, std::uint32_t outputSize);
  int DecompressZlib(std::span<const std::uint8_t> input, void* output, std::uint32_t outputSize);

}

#endif
