#include "Utility/MD5.h"

#include <cstring>
#include <cwchar>
#include <windows.h>

namespace {
  // INLINED
  inline std::uint32_t FoldDigest(const unsigned char* digest) {
    return ((digest[0] ^ digest[1] ^ digest[2] ^ digest[3]) << 8)
      | (digest[4] ^ digest[5] ^ digest[6] ^ digest[7])
      | ((digest[8] ^ digest[9] ^ digest[10] ^ digest[11]) << 16)
      | (static_cast<std::uint32_t>(digest[12] ^ digest[13] ^ digest[14] ^ digest[15]) << 24);
  }
}

// FUNCTION: fn_00121
MD5::MD5() {
}




#define MD5_F1(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define MD5_F2(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))
#define MD5_F3(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_F4(x, y, z) ((y) ^ ((x) | ~(z)))
#define MD5_STEP(f, a, b, c, d, x, s, t) do { \
  (a) += f((b), (c), (d)) + (x) + (t); \
  (a) = (((a) << (s)) | ((a) >> (32 - (s)))) + (b); \
} while (0)


#undef MD5_STEP
#undef MD5_F4
#undef MD5_F3
#undef MD5_F2
#undef MD5_F1










