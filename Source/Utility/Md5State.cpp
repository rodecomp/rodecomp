#include "Utility/Md5State.h"

#include <cstring>

// GLOBAL: gl_24504
static const unsigned char g_md5Padding[64] = {0x80};

#define MD5_ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define MD5_F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define MD5_G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define MD5_H(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_I(x, y, z) ((y) ^ ((x) | ~(z)))
#define MD5_SET(f, a, b, c, d, k, s, Ti) \
  t = (a) + f((b), (c), (d)) + X[k] + (Ti); \
  (a) = MD5_ROTATE_LEFT(t, s) + (b)

// OFFSET: 0x00C7E440
// FUNCTION: fn_20414
void MD5State::Append(const unsigned char* data, int nbytes) {
  const unsigned char* p = data;
  int left = nbytes;
  int offset = (this->count[0] >> 3) & 63;
  std::uint32_t nbits = static_cast<std::uint32_t>(nbytes << 3);

  if (nbytes <= 0) {
    return;
  }

  this->count[1] += nbytes >> 29;
  this->count[0] += nbits;
  if (this->count[0] < nbits) {
    this->count[1]++;
  }

  if (offset) {
    int copy = (offset + nbytes > 64 ? 64 - offset : nbytes);

    std::memcpy(this->buf + offset, p, copy);
    if (offset + copy < 64) {
      return;
    }
    p += copy;
    left -= copy;
    this->Process(this->buf);
  }

  for (; left >= 64; p += 64, left -= 64) {
    this->Process(p);
  }

  if (left) {
    std::memcpy(this->buf, p, left);
  }
}

// OFFSET: 0x00C7E530
// FUNCTION: fn_20415
void MD5State::Finish(unsigned char* digest) {
  unsigned char data[8];
  int i;

  for (i = 0; i < 8; ++i) {
    data[i] = static_cast<unsigned char>(this->count[i >> 2] >> ((i & 3) << 3));
  }
  this->Append(g_md5Padding, ((55 - (this->count[0] >> 3)) & 63) + 1);
  this->Append(data, 8);
  for (i = 0; i < 16; ++i) {
    digest[i] = static_cast<unsigned char>(this->abcd[i >> 2] >> ((i & 3) << 3));
  }
}

// OFFSET: 0x00C7E690
// FUNCTION: fn_20416
void MD5State::Init() {
  this->count[0] = this->count[1] = 0;
  this->abcd[0] = 0x67452301;
  this->abcd[1] = 0xEFCDAB89;
  this->abcd[2] = 0x98BADCFE;
  this->abcd[3] = 0x10325476;
}

// OFFSET: 0x00C7E6C0
// FUNCTION: fn_20417
void MD5State::Process(const unsigned char* data) {
  std::uint32_t a = this->abcd[0];
  std::uint32_t b = this->abcd[1];
  std::uint32_t c = this->abcd[2];
  std::uint32_t d = this->abcd[3];
  std::uint32_t t;
  std::uint32_t xbuf[16];
  const std::uint32_t* X;

  if (!(reinterpret_cast<std::uintptr_t>(data) & 3)) {
    X = reinterpret_cast<const std::uint32_t*>(data);
  } else {
    std::memcpy(xbuf, data, 64);
    X = xbuf;
  }

  MD5_SET(MD5_F, a, b, c, d, 0, 7, 0xD76AA478);
  MD5_SET(MD5_F, d, a, b, c, 1, 12, 0xE8C7B756);
  MD5_SET(MD5_F, c, d, a, b, 2, 17, 0x242070DB);
  MD5_SET(MD5_F, b, c, d, a, 3, 22, 0xC1BDCEEE);
  MD5_SET(MD5_F, a, b, c, d, 4, 7, 0xF57C0FAF);
  MD5_SET(MD5_F, d, a, b, c, 5, 12, 0x4787C62A);
  MD5_SET(MD5_F, c, d, a, b, 6, 17, 0xA8304613);
  MD5_SET(MD5_F, b, c, d, a, 7, 22, 0xFD469501);
  MD5_SET(MD5_F, a, b, c, d, 8, 7, 0x698098D8);
  MD5_SET(MD5_F, d, a, b, c, 9, 12, 0x8B44F7AF);
  MD5_SET(MD5_F, c, d, a, b, 10, 17, 0xFFFF5BB1);
  MD5_SET(MD5_F, b, c, d, a, 11, 22, 0x895CD7BE);
  MD5_SET(MD5_F, a, b, c, d, 12, 7, 0x6B901122);
  MD5_SET(MD5_F, d, a, b, c, 13, 12, 0xFD987193);
  MD5_SET(MD5_F, c, d, a, b, 14, 17, 0xA679438E);
  MD5_SET(MD5_F, b, c, d, a, 15, 22, 0x49B40821);

  MD5_SET(MD5_G, a, b, c, d, 1, 5, 0xF61E2562);
  MD5_SET(MD5_G, d, a, b, c, 6, 9, 0xC040B340);
  MD5_SET(MD5_G, c, d, a, b, 11, 14, 0x265E5A51);
  MD5_SET(MD5_G, b, c, d, a, 0, 20, 0xE9B6C7AA);
  MD5_SET(MD5_G, a, b, c, d, 5, 5, 0xD62F105D);
  MD5_SET(MD5_G, d, a, b, c, 10, 9, 0x02441453);
  MD5_SET(MD5_G, c, d, a, b, 15, 14, 0xD8A1E681);
  MD5_SET(MD5_G, b, c, d, a, 4, 20, 0xE7D3FBC8);
  MD5_SET(MD5_G, a, b, c, d, 9, 5, 0x21E1CDE6);
  MD5_SET(MD5_G, d, a, b, c, 14, 9, 0xC33707D6);
  MD5_SET(MD5_G, c, d, a, b, 3, 14, 0xF4D50D87);
  MD5_SET(MD5_G, b, c, d, a, 8, 20, 0x455A14ED);
  MD5_SET(MD5_G, a, b, c, d, 13, 5, 0xA9E3E905);
  MD5_SET(MD5_G, d, a, b, c, 2, 9, 0xFCEFA3F8);
  MD5_SET(MD5_G, c, d, a, b, 7, 14, 0x676F02D9);
  MD5_SET(MD5_G, b, c, d, a, 12, 20, 0x8D2A4C8A);

  MD5_SET(MD5_H, a, b, c, d, 5, 4, 0xFFFA3942);
  MD5_SET(MD5_H, d, a, b, c, 8, 11, 0x8771F681);
  MD5_SET(MD5_H, c, d, a, b, 11, 16, 0x6D9D6122);
  MD5_SET(MD5_H, b, c, d, a, 14, 23, 0xFDE5380C);
  MD5_SET(MD5_H, a, b, c, d, 1, 4, 0xA4BEEA44);
  MD5_SET(MD5_H, d, a, b, c, 4, 11, 0x4BDECFA9);
  MD5_SET(MD5_H, c, d, a, b, 7, 16, 0xF6BB4B60);
  MD5_SET(MD5_H, b, c, d, a, 10, 23, 0xBEBFBC70);
  MD5_SET(MD5_H, a, b, c, d, 13, 4, 0x289B7EC6);
  MD5_SET(MD5_H, d, a, b, c, 0, 11, 0xEAA127FA);
  MD5_SET(MD5_H, c, d, a, b, 3, 16, 0xD4EF3085);
  MD5_SET(MD5_H, b, c, d, a, 6, 23, 0x04881D05);
  MD5_SET(MD5_H, a, b, c, d, 9, 4, 0xD9D4D039);
  MD5_SET(MD5_H, d, a, b, c, 12, 11, 0xE6DB99E5);
  MD5_SET(MD5_H, c, d, a, b, 15, 16, 0x1FA27CF8);
  MD5_SET(MD5_H, b, c, d, a, 2, 23, 0xC4AC5665);

  MD5_SET(MD5_I, a, b, c, d, 0, 6, 0xF4292244);
  MD5_SET(MD5_I, d, a, b, c, 7, 10, 0x432AFF97);
  MD5_SET(MD5_I, c, d, a, b, 14, 15, 0xAB9423A7);
  MD5_SET(MD5_I, b, c, d, a, 5, 21, 0xFC93A039);
  MD5_SET(MD5_I, a, b, c, d, 12, 6, 0x655B59C3);
  MD5_SET(MD5_I, d, a, b, c, 3, 10, 0x8F0CCC92);
  MD5_SET(MD5_I, c, d, a, b, 10, 15, 0xFFEFF47D);
  MD5_SET(MD5_I, b, c, d, a, 1, 21, 0x85845DD1);
  MD5_SET(MD5_I, a, b, c, d, 8, 6, 0x6FA87E4F);
  MD5_SET(MD5_I, d, a, b, c, 15, 10, 0xFE2CE6E0);
  MD5_SET(MD5_I, c, d, a, b, 6, 15, 0xA3014314);
  MD5_SET(MD5_I, b, c, d, a, 13, 21, 0x4E0811A1);
  MD5_SET(MD5_I, a, b, c, d, 4, 6, 0xF7537E82);
  MD5_SET(MD5_I, d, a, b, c, 11, 10, 0xBD3AF235);
  MD5_SET(MD5_I, c, d, a, b, 2, 15, 0x2AD7D2BB);
  MD5_SET(MD5_I, b, c, d, a, 9, 21, 0xEB86D391);

  this->abcd[0] += a;
  this->abcd[1] += b;
  this->abcd[2] += c;
  this->abcd[3] += d;
}
