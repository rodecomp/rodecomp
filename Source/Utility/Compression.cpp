#include "Utility/Compression.h"
#include "Utility/Lz4.h"

#include <zlib.h>

namespace utility {

  // OFFSET: 0x0057C870
  // FUNCTION: fn_02605
  int DecompressLz4(std::span<const std::uint8_t> input, void* output, std::uint32_t outputSize) {
    const int result = LZ4_decompress_safe(reinterpret_cast<const char*>(input.data()), static_cast<char*>(output), static_cast<int>(input.size()), static_cast<int>(outputSize));
    return result < 0 ? -1 : result;
  }

  // OFFSET: 0x0057C8A0
  // FUNCTION: fn_46730
  int DecompressZlib(std::span<const std::uint8_t> input, void* output, std::uint32_t outputSize) {
    uLongf length = outputSize;
    const int result = uncompress(static_cast<Bytef*>(output), &length, input.data(), input.size());
    return result != Z_OK ? -1 : static_cast<int>(length);
  }

}
