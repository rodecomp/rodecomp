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

// FUNCTION: fn_00122
MD5::~MD5() {
}

// FUNCTION: fn_00123
const unsigned char* MD5::Decode(std::uint32_t* output, const unsigned char* input, std::uint32_t count) {
  do {
    *output++ = input[0] | ((input[1] | ((input[2] | (static_cast<std::uint32_t>(input[3]) << 8)) << 8)) << 8);
    input += 4;
  } while (--count != 0);
  return input;
}

// FUNCTION: fn_00124
int MD5::Initialize() {
  m_state[0] = 0x67452301;
  m_state[1] = 0xEFCDAB89;
  m_state[2] = 0x98BADCFE;
  m_state[3] = 0x10325476;
  m_byteCount[0] = 0;
  m_byteCount[1] = 0;
  return 0;
}

#define MD5_F1(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define MD5_F2(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))
#define MD5_F3(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_F4(x, y, z) ((y) ^ ((x) | ~(z)))
#define MD5_STEP(f, a, b, c, d, x, s, t) do { \
  (a) += f((b), (c), (d)) + (x) + (t); \
  (a) = (((a) << (s)) | ((a) >> (32 - (s)))) + (b); \
} while (0)

// FUNCTION: fn_00125
std::uint32_t* MD5::Transform(std::uint32_t* state, const std::uint32_t* input) {
  std::uint32_t a = state[0];
  std::uint32_t b = state[1];
  std::uint32_t c = state[2];
  std::uint32_t d = state[3];

  MD5_STEP(MD5_F1, a, b, c, d, input[0], 7, 0xD76AA478);
  MD5_STEP(MD5_F1, d, a, b, c, input[1], 12, 0xE8C7B756);
  MD5_STEP(MD5_F1, c, d, a, b, input[2], 17, 0x242070DB);
  MD5_STEP(MD5_F1, b, c, d, a, input[3], 22, 0xC1BDCEEE);
  MD5_STEP(MD5_F1, a, b, c, d, input[4], 7, 0xF57C0FAF);
  MD5_STEP(MD5_F1, d, a, b, c, input[5], 12, 0x4787C62A);
  MD5_STEP(MD5_F1, c, d, a, b, input[6], 17, 0xA8304613);
  MD5_STEP(MD5_F1, b, c, d, a, input[7], 22, 0xFD469501);
  MD5_STEP(MD5_F1, a, b, c, d, input[8], 7, 0x698098D8);
  MD5_STEP(MD5_F1, d, a, b, c, input[9], 12, 0x8B44F7AF);
  MD5_STEP(MD5_F1, c, d, a, b, input[10], 17, 0xFFFF5BB1);
  MD5_STEP(MD5_F1, b, c, d, a, input[11], 22, 0x895CD7BE);
  MD5_STEP(MD5_F1, a, b, c, d, input[12], 7, 0x6B901122);
  MD5_STEP(MD5_F1, d, a, b, c, input[13], 12, 0xFD987193);
  MD5_STEP(MD5_F1, c, d, a, b, input[14], 17, 0xA679438E);
  MD5_STEP(MD5_F1, b, c, d, a, input[15], 22, 0x49B40821);

  MD5_STEP(MD5_F2, a, b, c, d, input[1], 5, 0xF61E2562);
  MD5_STEP(MD5_F2, d, a, b, c, input[6], 9, 0xC040B340);
  MD5_STEP(MD5_F2, c, d, a, b, input[11], 14, 0x265E5A51);
  MD5_STEP(MD5_F2, b, c, d, a, input[0], 20, 0xE9B6C7AA);
  MD5_STEP(MD5_F2, a, b, c, d, input[5], 5, 0xD62F105D);
  MD5_STEP(MD5_F2, d, a, b, c, input[10], 9, 0x02441453);
  MD5_STEP(MD5_F2, c, d, a, b, input[15], 14, 0xD8A1E681);
  MD5_STEP(MD5_F2, b, c, d, a, input[4], 20, 0xE7D3FBC8);
  MD5_STEP(MD5_F2, a, b, c, d, input[9], 5, 0x21E1CDE6);
  MD5_STEP(MD5_F2, d, a, b, c, input[14], 9, 0xC33707D6);
  MD5_STEP(MD5_F2, c, d, a, b, input[3], 14, 0xF4D50D87);
  MD5_STEP(MD5_F2, b, c, d, a, input[8], 20, 0x455A14ED);
  MD5_STEP(MD5_F2, a, b, c, d, input[13], 5, 0xA9E3E905);
  MD5_STEP(MD5_F2, d, a, b, c, input[2], 9, 0xFCEFA3F8);
  MD5_STEP(MD5_F2, c, d, a, b, input[7], 14, 0x676F02D9);
  MD5_STEP(MD5_F2, b, c, d, a, input[12], 20, 0x8D2A4C8A);

  MD5_STEP(MD5_F3, a, b, c, d, input[5], 4, 0xFFFA3942);
  MD5_STEP(MD5_F3, d, a, b, c, input[8], 11, 0x8771F681);
  MD5_STEP(MD5_F3, c, d, a, b, input[11], 16, 0x6D9D6122);
  MD5_STEP(MD5_F3, b, c, d, a, input[14], 23, 0xFDE5380C);
  MD5_STEP(MD5_F3, a, b, c, d, input[1], 4, 0xA4BEEA44);
  MD5_STEP(MD5_F3, d, a, b, c, input[4], 11, 0x4BDECFA9);
  MD5_STEP(MD5_F3, c, d, a, b, input[7], 16, 0xF6BB4B60);
  MD5_STEP(MD5_F3, b, c, d, a, input[10], 23, 0xBEBFBC70);
  MD5_STEP(MD5_F3, a, b, c, d, input[13], 4, 0x289B7EC6);
  MD5_STEP(MD5_F3, d, a, b, c, input[0], 11, 0xEAA127FA);
  MD5_STEP(MD5_F3, c, d, a, b, input[3], 16, 0xD4EF3085);
  MD5_STEP(MD5_F3, b, c, d, a, input[6], 23, 0x04881D05);
  MD5_STEP(MD5_F3, a, b, c, d, input[9], 4, 0xD9D4D039);
  MD5_STEP(MD5_F3, d, a, b, c, input[12], 11, 0xE6DB99E5);
  MD5_STEP(MD5_F3, c, d, a, b, input[15], 16, 0x1FA27CF8);
  MD5_STEP(MD5_F3, b, c, d, a, input[2], 23, 0xC4AC5665);

  MD5_STEP(MD5_F4, a, b, c, d, input[0], 6, 0xF4292244);
  MD5_STEP(MD5_F4, d, a, b, c, input[7], 10, 0x432AFF97);
  MD5_STEP(MD5_F4, c, d, a, b, input[14], 15, 0xAB9423A7);
  MD5_STEP(MD5_F4, b, c, d, a, input[5], 21, 0xFC93A039);
  MD5_STEP(MD5_F4, a, b, c, d, input[12], 6, 0x655B59C3);
  MD5_STEP(MD5_F4, d, a, b, c, input[3], 10, 0x8F0CCC92);
  MD5_STEP(MD5_F4, c, d, a, b, input[10], 15, 0xFFEFF47D);
  MD5_STEP(MD5_F4, b, c, d, a, input[1], 21, 0x85845DD1);
  MD5_STEP(MD5_F4, a, b, c, d, input[8], 6, 0x6FA87E4F);
  MD5_STEP(MD5_F4, d, a, b, c, input[15], 10, 0xFE2CE6E0);
  MD5_STEP(MD5_F4, c, d, a, b, input[6], 15, 0xA3014314);
  MD5_STEP(MD5_F4, b, c, d, a, input[13], 21, 0x4E0811A1);
  MD5_STEP(MD5_F4, a, b, c, d, input[4], 6, 0xF7537E82);
  MD5_STEP(MD5_F4, d, a, b, c, input[11], 10, 0xBD3AF235);
  MD5_STEP(MD5_F4, c, d, a, b, input[2], 15, 0x2AD7D2BB);
  MD5_STEP(MD5_F4, b, c, d, a, input[9], 21, 0xEB86D391);

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
  return state;
}

#undef MD5_STEP
#undef MD5_F4
#undef MD5_F3
#undef MD5_F2
#undef MD5_F1

// FUNCTION: fn_00126
void MD5::Update(const unsigned char* input, std::uint32_t length) {
  std::uint32_t buffered = m_byteCount[0];
  m_byteCount[0] = buffered + length;
  if (m_byteCount[0] < buffered) {
    ++m_byteCount[1];
  }
  buffered &= 63;

  if (64 - buffered > length) {
    std::memcpy(m_buffer + buffered, input, length);
    return;
  }

  if (buffered != 0) {
    std::memcpy(m_buffer + buffered, input, 64 - buffered);
    Decode(m_words, m_buffer, 16);
    Transform(m_state, m_words);
    input += 64 - buffered;
    length += buffered - 64;
  }

  while (length >= 64) {
    Decode(m_words, input, 16);
    Transform(m_state, m_words);
    input += 64;
    length -= 64;
  }

  if (length != 0) {
    std::memcpy(m_buffer, input, length);
  }
}

// FUNCTION: fn_00127
unsigned char* MD5::Finalize() {
  const std::uint32_t buffered = m_byteCount[0] & 63;
  unsigned char* padding = m_buffer + buffered;
  *padding++ = 0x80;
  std::uint32_t remaining = 63 - buffered;

  if (remaining < 8) {
    std::memset(padding, 0, remaining);
    Decode(m_words, m_buffer, 16);
    Transform(m_state, m_words);
    padding = m_buffer;
    remaining = 64;
  }

  std::memset(padding, 0, remaining - 8);
  Decode(m_words, m_buffer, 14);
  m_words[14] = m_byteCount[0] << 3;
  m_words[15] = (m_byteCount[1] << 3) | (m_byteCount[0] >> 29);
  Transform(m_state, m_words);

  unsigned char* digest = reinterpret_cast<unsigned char*>(m_state);
  return digest;
}

// FUNCTION: fn_00128
unsigned char* MD5::DigestBuffer(const unsigned char* input, std::uint32_t length, unsigned char* digest) {
  Initialize();
  Update(input, length);
  Finalize();
  std::memcpy(digest, m_state, 16);
  return digest;
}

// FUNCTION: fn_00129
std::uint32_t MD5::HashBuffer(const unsigned char* input, std::uint32_t length) {
  unsigned char digest[16] = {};
  DigestBuffer(input, length, digest);
  return FoldDigest(digest);
}

// FUNCTION: fn_00130
std::uint32_t MD5::HashString(const char* text) {
  unsigned char digest[16] = {};
  DigestBuffer(reinterpret_cast<const unsigned char*>(text), static_cast<std::uint32_t>(std::strlen(text)), digest);
  return FoldDigest(digest);
}

// FUNCTION: fn_00131
std::uint32_t MD5::HashString(const wchar_t* text) {
  char converted[260];
  WideCharToMultiByte(CP_ACP, 0, text, -1, converted, 260, nullptr, nullptr);
  return HashString(converted);
}

// FUNCTION: fn_00132
std::uint32_t MD5::HashFile(const wchar_t* filename) {
  if (filename == nullptr || *filename == 0) {
    return 0;
  }

  HANDLE file = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file == INVALID_HANDLE_VALUE) {
    return 0;
  }

  std::uint32_t result = 0;
  LARGE_INTEGER size;
  if (!GetFileSizeEx(file, &size)) {
    return result;
  }
  if (size.QuadPart > 500000000) {
    size.QuadPart = 500000000;
  }
  if (size.QuadPart == 0) {
    return 0;
  }

  HANDLE mapping = CreateFileMappingW(file, nullptr, PAGE_READONLY, size.HighPart, size.LowPart, nullptr);
  if (mapping != nullptr) {
    const unsigned char* view = static_cast<const unsigned char*>(MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, size.LowPart));
    if (view != nullptr) {
      unsigned char digest[16] = {};
      DigestBuffer(view, size.LowPart, digest);
      result = FoldDigest(digest);
      UnmapViewOfFile(view);
    }
    CloseHandle(mapping);
  }
  CloseHandle(file);
  return result;
}

// FUNCTION: fn_00133
std::uint32_t MD5::HashStringLower(const char* text) {
  char lowered[260];
  strcpy_s(lowered, 260, text);
  _strlwr_s(lowered, 260);
  return HashString(lowered);
}

// FUNCTION: fn_00134
std::uint32_t MD5::HashStringLower(const wchar_t* text) {
  wchar_t lowered[260];
  wcscpy_s(lowered, 260, text);
  _wcslwr_s(lowered, 260);
  return HashString(lowered);
}

// FUNCTION: fn_00135
std::uint32_t MD5::HashFile(const char* filename) {
  wchar_t converted[260];
  MultiByteToWideChar(CP_ACP, 0, filename, -1, converted, 260);
  return HashFile(converted);
}
